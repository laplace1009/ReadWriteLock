#include "ReadWriteLock.h"
#include "Types.h"
#include "Macro.h"
#include <thread>
#include <chrono>

// юс╫ц thread_local id
std::atomic<uint32_t> LThreadId = 1;

void ReadWriteLock::WriteLock()
{
	const uint32 lockOwnerId = mLockFlag.load() & WRITE_LOCK_MASK >> 16;
	if (lockOwnerId == LThreadId) {
		mWriteLockCount++;
		return;
	}

	while (true)
	{
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		for (size_t spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = 0;
			if (mLockFlag.compare_exchange_strong(OUT expected, 1))
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

void ReadWriteLock::WriteUnlock()
{
	
}

void ReadWriteLock::ReadLock()
{
	if ((mLockFlag & WRITE_LOCK_MASK) >> 16 == LThreadId) {
		mLockFlag++;
		return;
	}

	while (true)
	{
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		
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

void ReadWriteLock::ReadUnlock()
{
	return;
}
