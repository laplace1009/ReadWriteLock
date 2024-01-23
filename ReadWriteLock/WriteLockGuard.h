#pragma once
#include "ReadWriteLock.h"

class WriteLockGuardPtr
{
public:
	WriteLockGuardPtr(ReadWriteLock* lock)
		: mLock(lock)
	{
		mLock->WriteLock();
	}

	~WriteLockGuardPtr()
	{
		mLock->WriteUnlock();
	}

private:
	ReadWriteLock* mLock;
};

class WriteLockGuardRef
{
public:
	WriteLockGuardRef(ReadWriteLock& lock)
		: mLock(lock)
	{
		mLock.WriteLock();
	}

	~WriteLockGuardRef()
	{
		mLock.ReadUnlock();
	}

private:
	ReadWriteLock& mLock;
};

