#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include "ReadWriteLock.h"
#include "ReadLockGuard.h"
#include "WriteLockGuard.h"
#include "Types.h"
#include "TLS.h"

/*-----------------
	example ÄÚµå
------------------*/

using namespace std;

Atomic<uint32> sANum = 1;

int32 num = 0;

ReadWriteLock* GLock = new ReadWriteLock();

void Add()
{
	for (int i = 0; i < 10000; ++i)
	{
		num++;
	}
}

void Sub()
{
	for (int i = 0; i < 10000; ++i)
	{
		num--;
	}
}

int main(int argc, char** argv)
{

	LThreadId = sANum.fetch_add(1);
	sANum.fetch_add(1);
	thread t1{ []()
		{
			LThreadId = sANum.fetch_add(1);
			WriteLockGuardPtr guard{ GLock };
			Add();
		} };

	
	thread t2{ []()
		{
			LThreadId = sANum.fetch_add(1);
			WriteLockGuardPtr guard{ GLock };
			Sub();
		} };

	t1.join();
	t2.join();

	cout << num << endl;

	delete GLock;
}