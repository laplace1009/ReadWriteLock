#include "ReadWriteLock.h"
#include "Types.h"
#include "Macro.h"
#include <thread>
#include <chrono>

// 임시 thread_local id
std::atomic<uint32_t> LThreadId = 1;

auto ReadWriteLock::WriteLock() -> void
{
	const uint32 lockOwnerId = (mLockFlag.load() & WRITE_LOCK_MASK) >> 16;
	if (lockOwnerId == LThreadId) {
		mWriteLockCount++;
		return;
	}

	// desired 부분 수정
	const int32 desired = (LThreadId << 16) & WRITE_LOCK_MASK;

	// tick 측정 시작 부분 루프 밖으로 이동
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	while (true)
	{
		
		for (size_t spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = 0;
			// desired 부분 수정
			if (mLockFlag.compare_exchange_strong(OUT expected, desired))
			{
				mLockFlag++;
				return;
			}
		}
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

		std::chrono::duration<double, std::milli> elapsed = end - start;
		if (static_cast<uint32>(elapsed.count()) > MAX_TICK_COUNT) CRASH("Lock Time out");

		std::this_thread::yield();
	}
}

auto ReadWriteLock::WriteUnlock() -> void
{
	ASSERT_CRASH((mLockFlag & WRITE_LOCK_MASK) >> 16 == LThreadId);

	const int32 writeCount = --mWriteLockCount;
	if (writeCount == 0) mLockFlag.store(0);
}

auto ReadWriteLock::ReadLock() -> void
{
	if ((mLockFlag & WRITE_LOCK_MASK) >> 16 == LThreadId) {
		mLockFlag++;
		return;
	}

	// tick 측정 시작 부분 루프 밖으로 이동
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	while (true)
	{	
		for (size_t spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = mLockFlag.load() & READ_LOCK_MASK;
			if (mLockFlag.compare_exchange_strong(OUT expected, expected + 1)) return;
		}

		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

		std::chrono::duration<double, std::milli> elapsed = end - start;
		if (static_cast<uint32>(elapsed.count()) > MAX_TICK_COUNT) CRASH("Lock Time out");

		std::this_thread::yield();
	}
}

auto ReadWriteLock::ReadUnlock() -> void
{
	const int32 lockFlag = mLockFlag.fetch_sub(1) & READ_LOCK_MASK;
	if (lockFlag == 0) CRASH('Invalid Unlock');
}
