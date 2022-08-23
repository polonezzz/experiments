
#include "windows.h"
#include <array>
#include <iostream>

#include <memory>
#include <vector>

using namespace std;

struct Good: std::enable_shared_from_this<Good> // note: public inheritance
{
    std::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};
 
struct Bad
{
    std::shared_ptr<Bad> getptr() {
        return std::shared_ptr<Bad>(this);
    }
    ~Bad() { std::cout << "Bad::~Bad() called\n"; }
};

class A
{
protected:	
	int a = 0;
public:
	A(int _a) : a(_a) {}
	virtual int get() = 0;

	virtual ~A() {}
};

class A2 : /*virtual*/ public A
{
public:
	A2(int _a) : A(_a) {}
	virtual double get(int) {return 0;};
};

class B : /*virtual*/ public A
{
public:
	B() : A(0xBEEF) {}
	int get() {return a;}
	virtual void put (int) {;}
	void non_virt() {;}

	static void* operator new(size_t size, const Good& g)
	{
		return ::operator new(size);
	}

	static void* operator new(size_t size, void* ptr)
	{
		return ptr;
	}

	static void operator delete(void* ptr, const Good& g)
	{
		return ::operator delete(ptr);
	}

	static void operator delete (void* ptr)
	{
		return ::operator delete(ptr);
	}

	static void operator delete (void* ptr, void*)
	{
		return;
	}
	
};


class C : public A2, public B
{
public:
	C() : A2(0xDEAD)/*, A(0xFEED)*/ {}
	int get() {return A::a;}
	void put (int) {;}
};

long switch_eg(long x, long y, long z)
{
	long w = 1;

	switch(x)
	{
	case 2: 
		w = y * z; 
		break;
	case 67: 
		w = y / z;
	case 95: 
		w += z;
		break;
	case 200:
	case 202:
		w -= z;
		break;
	default:
		w = 2;
	}

	return w;
}

struct Shared : public std::enable_shared_from_this<Shared>
{
	int data = 13;

	std::shared_ptr<Shared> getptr()
	{
		return shared_from_this();
	}

	const int& getdata()
	{
		return data;
	}
};

int wmain(int argc, wchar_t* argv[], wchar_t* env[])
{
	auto aptr = make_shared<int>(Shared().getdata());
	*aptr = 17;

	auto sh = shared_ptr<Shared>(new Shared);
	auto bptr = make_shared<Shared>()->shared_from_this();
	*aptr = bptr->getdata();

	long w = 0;
	cin >> w;
	w = switch_eg(w, 3, 4);
	if (w > 2)
		return 0;
	{
		Good g;
		std::unique_ptr<C> bptr(new(g) C);
		bptr->get();

		uint8_t* buf = new uint8_t[sizeof(B)];
		B* brptr = new(buf) B;
		brptr->~B();
		delete[] buf;
	}
	
	C c;
	B& b = c;
	b.get();

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

