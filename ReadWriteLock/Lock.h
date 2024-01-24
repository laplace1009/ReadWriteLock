#pragma once

#include "ReadWriteLock.h"
#include "WriteLockGuard.h"
#include "ReadLockGuard.h"

namespace lock
{
	class ReadWriteLock;

	class ReadLockGuardPtr;

	class ReadLockGuardRef;

	class WriteLockGuardPtr;

	class WriteLockGuardRef;
}