#pragma once
#include "ReadWriteLock.h"

class ReadLockGuardPtr
{
public:
	ReadLockGuardPtr(ReadWriteLock* lock)
		: mLock(lock)
	{
		mLock->ReadLock();
	}

	~ReadLockGuardPtr()
	{
		mLock->ReadUnlock();
	}

private:
	ReadWriteLock* mLock;
};


class ReadLockGuardRef
{
public:
	ReadLockGuardRef(ReadWriteLock& lock)
		: mLock(lock)
	{
		mLock.ReadLock();
	}

	~ReadLockGuardRef()
	{
		mLock.ReadUnlock();
	}

private:
	ReadWriteLock& mLock;
};

