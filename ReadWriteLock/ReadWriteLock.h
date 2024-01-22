#pragma once
#include <atomic>
#include <thread>

class ReadWriteLock
{
	constexpr static uint32 MAX_TICK_COUNT = 10000.0;
	constexpr static uint32 MAX_SPIN_COUNT = 4000;
	constexpr static uint32 WRITE_LOCK_MASK = 0XFFFF'0000;
	constexpr static uint32 READ_LOCK_MASK = 0X0000'FFFF;

public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> mLockFlag;
	size_t mWriteLockCount;
};

