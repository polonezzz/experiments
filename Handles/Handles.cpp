
#include "windows.h"
#include <array>
#include <iostream>


int waterFill(const std::array<int, 9>& land) 
{

	int leftMax = 0;
	int rightMax = 0;
	int left = 0;
	int right = land.size() - 1;
	int volume = 0;

	while(left < right) 
	{
		if(land[left] > leftMax) 
		{
			leftMax = land[left];
		}
		
		if(land[right] > rightMax) 
		{
			rightMax = land[right];
		}
		
		if(leftMax >= rightMax) 
		{
			volume += rightMax - land[right];
			right--;
		}
		else 
		{
			volume += leftMax - land[left];
			left++;
		}
	}
	return volume;
}


int wmain(int argc, wchar_t* argv[], wchar_t* env[])
{
	int vol =  waterFill({2, 5, 1, 4, 6, 4, 7, 7, 6});
	
	std::wcout << L"------enviroment variables------------\n";
	wchar_t** pEnvVar = env;
	while (pEnvVar && *pEnvVar)
	{
		std::wcout << *pEnvVar << L"\n";
		++pEnvVar;
	}
	
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	
	HANDLE hEvent = CreateEventW(&sa, TRUE, TRUE, L"{BC294BFE-A5F4-448D-84DD-C0C4B20E15DD}");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hEvent);
		
		int a;
		std::cin >> a;
		return 0;
	}

	wchar_t buf[MAX_PATH + 1] = {0,};
	DWORD size = GetModuleFileNameW(NULL, buf, sizeof(buf)/sizeof(wchar_t));
	if (size)
	{
		STARTUPINFO si = {0,};
		PROCESS_INFORMATION info = {0,};
		BOOL res = CreateProcessW(NULL, buf, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, NULL, &si, &info); 

		ResumeThread(info.hThread);
		WaitForSingleObject(info.hProcess, INFINITE);
		
		int a;
		std::cin >> a;
		
		CloseHandle(info.hThread);
		CloseHandle(info.hProcess);
	}
	CloseHandle(hEvent);

    return 0;
}

