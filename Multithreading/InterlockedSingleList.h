#pragma once

#include <windows.h>
#include <utility>

//template<typename T>
class InterlockedSingleList
{
	struct SListItem
	{
		SLIST_ENTRY header;
		DWORD data;
	};

	PSLIST_HEADER pHead;

public:
	InterlockedSingleList();
	~InterlockedSingleList();

	operator bool() { return pHead != nullptr; }

	bool Push(DWORD data);
	bool Pop(DWORD& data);
	size_t Size() { return pHead ? QueryDepthSList(pHead) : 0; }

};

void TestInterlockedSingleList();