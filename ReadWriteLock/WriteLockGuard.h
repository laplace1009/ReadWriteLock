#pragma once
#include "ReadWriteLock.h"

class WriteLockGuard
{
public:
	WriteLockGuard(ReadWriteLock* lock)
		: mLock(lock)
	{
		mLock->WriteLock();
	}

	~WriteLockGuard()
	{
		mLock->WriteUnlock();
	}

private:
	ReadWriteLock* mLock;
};

