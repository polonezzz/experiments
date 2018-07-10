
#include "InterlockedSingleList.h"

#include <iostream>
#include <chrono>
#include <thread>

InterlockedSingleList::InterlockedSingleList()
	: pHead((PSLIST_HEADER)_aligned_malloc(sizeof(SLIST_HEADER), MEMORY_ALLOCATION_ALIGNMENT))
{
	if (pHead)
		InitializeSListHead(pHead);
}

InterlockedSingleList::~InterlockedSingleList()
{
	if (!pHead)
		return;
	
	while (auto pItem = InterlockedPopEntrySList(pHead))
		 _aligned_free(pItem);
	
	_aligned_free(pHead);
}

bool InterlockedSingleList::Push(DWORD data)
{
	if (!pHead)
		return false;

	auto pItem = (SListItem*)_aligned_malloc(sizeof(SListItem), MEMORY_ALLOCATION_ALIGNMENT);
	if (pItem)
	{
		pItem->data = data;
		InterlockedPushEntrySList(pHead,&pItem->header);
	}

	return pItem != nullptr;
}

bool InterlockedSingleList::Pop(DWORD& data)
{
	if (!pHead)
		return false;
	
	auto pItem = (SListItem*)InterlockedPopEntrySList(pHead);
	if (pItem)
	{
		data = std::move(pItem->data);
		_aligned_free(pItem);
	}	

	return pItem != nullptr;
}


void TestInterlockedSingleList()
{
	InterlockedSingleList slist;

	if (!slist)
		return;
	
	bool exit = false;

	auto pushThread = [&]()
	{
		DWORD data = 1;
		while (!exit && slist.Push(data++)) 
			//std::this_thread::sleep_for(std::chrono::milliseconds(10));
			std::this_thread::yield();
	};

	auto popThread = [&]()
	{
		DWORD data;
		while (!exit)
		{
			slist.Pop(data);
			//std::this_thread::sleep_for(std::chrono::milliseconds(20));
			std::this_thread::yield();
		}
	};

	std::thread threads[] = 
	{
		std::thread{pushThread},
		std::thread{pushThread},
		std::thread{pushThread},
		std::thread{pushThread},
		//std::thread{pushThread},
		//std::thread{pushThread},
		//std::thread{pushThread},
		//std::thread{pushThread},
		std::thread{popThread},
		std::thread{popThread},
		std::thread{popThread},
		std::thread{popThread},
		std::thread{popThread},
		std::thread{popThread},
		std::thread{popThread},
		std::thread{popThread}
	};

	size_t count = 100;
	while (count--)
	{
		std::cout << "slist size: " << slist.Size() << "\n"; 
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	
	exit = true;
	
	for (auto& t : threads)
		t.join();
}