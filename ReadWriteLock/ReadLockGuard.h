#pragma once
#include "ReadWriteLock.h"

class ReadLockGuard
{
public:
	ReadLockGuard(ReadWriteLock* lock)
		: mLock(lock)
	{
		mLock->ReadLock();
	}

	~ReadLockGuard()
	{
		mLock->ReadUnlock();
	}

private:
	ReadWriteLock* mLock;
};

