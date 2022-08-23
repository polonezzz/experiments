// Memory.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <Psapi.h>

#include <exception>
#include <iostream>
#include <memory>

#pragma comment(lib, "psapi.lib")

void PrintCacheInfo()
{
	DWORD size = 0;
	GetLogicalProcessorInformationEx(RelationCache, nullptr, &size);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		PBYTE pProcInfo = (PBYTE)malloc(size);
		if (GetLogicalProcessorInformationEx(RelationCache, (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)pProcInfo, &size))
		{
			for(auto p = pProcInfo; p < pProcInfo + size;)
			{
				auto ptr = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)p;
				p += ptr->Size;

				if(ptr->Relationship != RelationCache)
					continue;

				auto& proc = ptr->Cache;

				std::cout << "L" << (uint32_t)proc.Level
					<< " " << (proc.Type == CacheInstruction ? "Instruction" :
							   proc.Type == CacheData ? "Data" :
							   proc.Type == CacheTrace ? "Trace" : "Unified")
					<< ", CacheSize: " << proc.CacheSize << " bytes"
					<< ", Associativity: " << (uint16_t)proc.Associativity
					<< ", LineSize: " << proc.LineSize << " bytes\n";
			}

			free(pProcInfo);
		}
	}
}

void FileMapping(const wchar_t* name)
{
	HANDLE hFile = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	
	const size_t fileSize = 100;

	HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READWRITE, 0, fileSize, L"WinExperimentsMap");
	HANDLE hMap2 = OpenFileMapping(PAGE_READWRITE, FALSE, L"WinExperimentsMap");
	CloseHandle(hFile);
	
	if (!(hMap || hMap2))
		return;

	uint8_t* pView = static_cast<uint8_t*>(MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0));
	uint8_t* pView2 = static_cast<uint8_t*>(MapViewOfFile(hMap2, FILE_MAP_COPY, 0, 0, 0));

	CloseHandle(hMap2);
	CloseHandle(hMap);

	MEMORY_BASIC_INFORMATION mbi = {0,};
	VirtualQuery(pView2, &mbi, sizeof(mbi)); //page had PAGE_WRITECOPY protection attribute

	uint32_t sum = 0;
	for (uint8_t i = 0; i < fileSize; ++i)
	{
		pView[i] = i;
		pView2[i] = fileSize - i;
	}
	
	MEMORY_BASIC_INFORMATION mbi2 = {0,};
	VirtualQuery(pView2, &mbi2, sizeof(mbi2));  //page has PAGE_READWRITE protection attribute
	
	memcpy(pView, pView2, fileSize);

	
	UnmapViewOfFile(pView2);
//	CloseHandle(hMap2);
	
	UnmapViewOfFile(pView2);
//	CloseHandle(hMap);
}

/*
bool SetPrivilege(LPCWSTR privilege, bool enable)
{
	HANDLE hToken;
	//if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES, FALSE, &hToken))
	//	return false;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		return false;

	DWORD bufsize = 0;
	TOKEN_PRIVILEGES tp;
	TOKEN_USER tu;
	GetTokenInformation(hToken, TokenUser, &tu, sizeof(TOKEN_USER), &bufsize);

	LUID luid;
	bool ret = TRUE == LookupPrivilegeValue(NULL, privilege, &luid);

	TOKEN_PRIVILEGES newState, prevState;
	DWORD prevLen = 0;

	newState.PrivilegeCount = 1;
	newState.Privileges[0].Luid = luid;
	newState.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

	ret = ret && TRUE == AdjustTokenPrivileges(hToken, FALSE, &newState, sizeof(TOKEN_ADJUST_PRIVILEGES), &prevState, &prevLen);

	CloseHandle(hToken);
	return ret;
}
*/

int wmain(int argc, wchar_t* argv[])
{
	PROCESS_MEMORY_COUNTERS_EX counters = {0,};
	GetProcessMemoryInfo(GetCurrentProcess(), (PPROCESS_MEMORY_COUNTERS)&counters, sizeof(PROCESS_MEMORY_COUNTERS_EX));
	
	MODULEINFO moduleInfo = {0,};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(moduleInfo));

	//void* pMain = main;
	
	//try to allocate on stack, on heap (new, malloc), static (global), main
	MEMORY_BASIC_INFORMATION memInfo = {0,};
	VirtualQuery(&memInfo, &memInfo, sizeof(memInfo));

	SYSTEM_INFO sysInfo = {0,};
	GetSystemInfo(&sysInfo);
	
	SYSTEM_INFO nativeInfo = {0,};
	GetNativeSystemInfo(&nativeInfo);
	
	if (argc > 1)
		FileMapping(argv[1]);
	
	/*
	if (SetPrivilege(SE_LOCK_MEMORY_NAME, true))
	{
		auto largePageSize = GetLargePageMinimum();
		void* pLargePages = VirtualAlloc(NULL, largePageSize * 2, MEM_RESERVE | MEM_COMMIT | MEM_LARGE_PAGES | MEM_TOP_DOWN, PAGE_READWRITE);
		if (pLargePages)
		{
		
			VirtualFree(pLargePages, 0, MEM_RELEASE);
		}
	}
	*/
	
	auto pageSize = sysInfo.dwPageSize;
	void* pReserved = VirtualAlloc(NULL, pageSize * 2, MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!pReserved)
		return -1;
	
	void* pCommited = VirtualAlloc((BYTE*)pReserved + pageSize - 1, 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pCommited)
	{
	
		//FlushInstructionCache(GetCurrentProcess(), pCommited, pageSize);
		
		for (char c = 'A', *ptr = (char*)pCommited; c <= 'Z'; ++c, ++ptr)
			*ptr = c;
		
		__try
		{
			
			BYTE* ptr = (BYTE*)pReserved;
			ptr[pageSize] = 0xFF;
			ptr[pageSize + 2] = 0xFF;
		}
		__except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_EXECUTE_HANDLER)
		{
			;
		}
	}
	
	VirtualFree(pReserved, 0, MEM_RELEASE);
	
	return 0;
}

