#include <assert.h>
#include <windows.h>
#include <ImageHlp.h>

#include <array>
#include <algorithm>

#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#include <atomic>

#include <chrono>
#include <complex>
#include <condition_variable>
#include <cstdio>
#include <exception>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <sstream>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

#include "InterlockedSingleList.h"
#include "Templates.h"

using namespace std;

void ListCPUCacheInfo()
{
	DWORD size = 0;
	BOOL res = GetLogicalProcessorInformation(nullptr, &size);

	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		auto infoSize = sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		
		std::unique_ptr<BYTE[]> buffer(new BYTE[size]);
		auto pInfo = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)buffer.get();
		
		if (!GetLogicalProcessorInformation(pInfo, &size))
			return;
		

		while((PBYTE)pInfo < buffer.get() + size)
		{
			if (pInfo->Relationship == RelationCache)
			{
				std::cout << "CPU number: ";

				size_t i = 0;
				while (auto mask = pInfo->ProcessorMask >> i++)
					if (mask & 0x1)
						std::cout << i << " ";

				std::cout << ", level: L" << (size_t)pInfo->Cache.Level
					<< ", type: ";

				switch(pInfo->Cache.Type)
				{
				case CacheUnified:
				std::cout << "unified"; break;
				case CacheInstruction:
				std::cout << "instruction"; break;
				case CacheData:
				std::cout << "data"; break;
				case CacheTrace:
				std::cout << "trace"; break;
				}

				std::cout << ", size: " << pInfo->Cache.Size
					<< ", line size: " << pInfo->Cache.LineSize << "\n";
			}

			++pInfo;
		}
	}
}

void UpdateImage()
{
	WIN32_FIND_DATAA data = {0,};
	HANDLE hSearch = FindFirstFileA("e:\\Dev\\WinAPI\\Debug\\NewsBase.exe", &data);
	
	if (hSearch == INVALID_HANDLE_VALUE)
		return;

	PIMAGE_SECTION_HEADER pScnHeader;
	ULONG dirSize = 0;

	do
	{
		LOADED_IMAGE image;
		if (MapAndLoad(data.cFileName, NULL, &image, FALSE, FALSE) == FALSE)
			continue;

		IMAGE_LOAD_CONFIG_DIRECTORY config;
		GetImageConfigInformation(&image, &config);

		PVOID pDir = ImageDirectoryEntryToDataEx(image.MappedAddress, FALSE, 
				IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, &dirSize, &pScnHeader);
		MEMORY_BASIC_INFORMATION buffer;
		VirtualQueryEx(GetCurrentProcess(), pDir, &buffer, sizeof(IMAGE_LOAD_CONFIG_DIRECTORY));

		config.MajorVersion = 1;
		config.MinorVersion = 1;
		config.ProcessAffinityMask = 0x3;
		SetImageConfigInformation(&image, &config);

		UnMapAndLoad(&image);

		PLOADED_IMAGE pImage = ImageLoad(data.cFileName, NULL);  
		if (pImage)
		{
			MEMORY_BASIC_INFORMATION buffer;
			VirtualQueryEx(GetCurrentProcess(), pImage, &buffer, sizeof(LOADED_IMAGE));
			
			IMAGE_LOAD_CONFIG_DIRECTORY config;
			GetImageConfigInformation(pImage, &config);
						
			PVOID pDir = ImageDirectoryEntryToDataEx(pImage->MappedAddress, FALSE, 
				IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, &dirSize, &pScnHeader);
			VirtualQueryEx(GetCurrentProcess(), pDir, &buffer, sizeof(IMAGE_LOAD_CONFIG_DIRECTORY));
			
			// memory pointed with pDir is read-only
			//PBYTE pAffinity = (PBYTE)pDir + sizeof(DWORD) * 12;
			//*pAffinity = 0;													// access violation

			//config.ProcessAffinityMask = 0x2;
			//SetImageConfigInformation(pImage, &config);						//access violation
			ImageUnload(pImage);
		}

	} while (FindNextFileA(hSearch, &data) == TRUE);
	
	FindClose(hSearch);
}

void WaitableTimer()
{
	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL); 
	LARGE_INTEGER dueTime;
	dueTime.QuadPart = -2 * 10000000;   // 5 sec 

	LONG period = 1000; // msec
	
	auto f = [](LPVOID arg, DWORD low, DWORD high)
	{
		FILETIME ft = {low, high};
		FILETIME local;
		SYSTEMTIME time;
		FileTimeToLocalFileTime(&ft, &local);
		FileTimeToSystemTime(&local, &time);
		
		std::printf("%02d - %02d:%02d:%02d:%03d\n", *(size_t*)arg, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	
	};

	auto g = [](ULONG_PTR arg) { std::cout << arg <<". QueueUserAPC\n"; };
	QueueUserAPC(g, GetCurrentThread(), 1);
	QueueUserAPC(g, GetCurrentThread(), 2);

	size_t count = 30;
	if (hTimer && SetWaitableTimer(hTimer, &dueTime, period, f, &count, FALSE))
	{
		while (count-- > 15)
			WaitForSingleObjectEx(hTimer, INFINITE, FALSE);
		
		SleepEx(INFINITE, TRUE);
		
		while (count--)
			//WaitForSingleObjectEx(hTimer, INFINITE, TRUE);
			SleepEx(INFINITE, TRUE);
	}
}

class TestString
{
public:
	std::string str;
public:
	TestString()
	{
		str = "";
	}
	
	TestString(const char* pStr)
	{
		str = pStr;
		str.replace(0, 4, "that");
	}

	TestString(const TestString& rhs) = default;
	TestString& operator=(const TestString& rhs) = default;

	TestString(TestString&& rhs)
	{
		*this = std::move(rhs);
	}
	
	TestString& operator=(TestString&& rhs)
	{
		str.swap(rhs.str);
		rhs.str.clear();
		return *this;
	}
	
};

constexpr uint32_t fib(uint32_t n) 
{
	return n < 2 ? n : fib(n-1) + fib(n-2);
}

using frame = std::chrono::duration<std::uintmax_t, std::ratio<1, 25>>;

frame operator "" _frame(uintmax_t val)
{
	return frame(val);
}

template<typename F, typename A>
std::future<typename std::result_of<F(A&&)>::type> spawn_task(F&& f, A&& a)
{
	typedef std::result_of<F(A&&)>::type result_type;
	std::packaged_task<result_type(A&&)> task(std::forward<F>(f));
		
	std::future<result_type> res(task.get_future());
	std::thread t(std::move(task), std::forward<A>(a));
	t.detach();
	return res;
}

std::atomic<bool> x = false, y = false;
std::atomic<int> z = 0;

void write_x_then_y()
{
	x.store(true, std::memory_order_relaxed);
	y.store(true, std::memory_order_relaxed);
}
void read_y_then_x()
{
	while (!y.load(std::memory_order_relaxed));
	if (x.load(std::memory_order_relaxed))
		++z;
}


using namespace chrono_literals;

struct pcout : public stringstream
{
	static inline mutex cout_mutex;

	~pcout()
	{
		lock_guard<mutex> l{ cout_mutex };
		cout << rdbuf();
		cout.flush();
	}
};

static string create(const char* s)
{
	pcout{} << "3s CREATE " << quoted(s) << '\n';
	this_thread::sleep_for(3s);
	return { s };
}

static string concat(const string& a, const string& b)
{
	pcout{} << "5s CONCAT "
		<< quoted(a) << " "
		<< quoted(b) << '\n';
	this_thread::sleep_for(5s);
	return a + b;
}

static string twice(const string& s)
{
	pcout{} << "3s TWICE  " << quoted(s) << '\n';
	this_thread::sleep_for(3s);
	return s + s;
}

template <typename F, typename ...T>
static auto asynchronize(F f, T...xs)
{
	return [=]()
	{
		return async(launch::async, f, xs...);
	};
}

template <typename F, typename ...T>
static auto async_adapter(F f, T...xs)
{
	//c
	return [=]()
	{
		return async(launch::async, [f](auto ... xs) { return f(xs.get()...); } , xs()...);
	};
}


int main()
{
	thread::hardware_concurrency();
	{
		auto result = async_adapter(concat,
			async_adapter(twice, async_adapter(concat, asynchronize(create, "foo "), asynchronize(create, "bar "))),
			async_adapter(concat, asynchronize(create, "this "), asynchronize(create, "that ")));

		cout << "Setup done. Nothing executed yet.\n";

		cout << result().get() << '\n';
	//}
	
	//{	
		thread a(write_x_then_y);
		thread b(read_y_then_x);
		a.join();
		b.join();
				
		thread c;
		c = thread(result);
		c.join();
		c = thread(result);
		c.join();
		

		assert(z.load() != 0);

		// atomics
		std::atomic_flag afl = ATOMIC_FLAG_INIT;
		_ASSERT(!afl.test_and_set());
		_ASSERT(afl.test_and_set());
		afl.clear();

		std::atomic<int> ai;
		std::atomic_init(&ai, 1);
		ai.store(10);
		ai.load();
		ai.exchange(2);

		int expected = 1, exchanged = 10;
		while (!ai.compare_exchange_strong(expected, exchanged))
			std::cout << "ai = " << ai << " expected = " << expected << " exchanged = " << exchanged << '\n';
		std::cout << "ai = " << ai << " expected = " << expected << " exchanged = " << exchanged << '\n';
		
		struct Atom
		{
			unsigned char : 3;
			unsigned char : 0;
			unsigned char : 6;
			unsigned char : 2;
		};
		std::cout << std::atomic<Atom>{}.is_lock_free();

		Atom atom;
		std::atomic<Atom*> pAtom;
		pAtom.store(&atom);
		//pAtom.fetch_add(1);
	}

	{
		std::vector<int> a{1, 2, 3, 4};
		auto f = [](auto data) { return std::accumulate(std::begin(data), std::end(data), 100); };
		auto res = spawn_task(f, a);
		a.push_back(res.get());
	}
	
	{   
		using namespace std::chrono;

		time_point<steady_clock, hours> tp(hours(1));
		tp.time_since_epoch();

		auto in = steady_clock::now();
		std::this_thread::sleep_for(frame(50));
		auto out = steady_clock::now();
		
		frame est = duration_cast<frame>(out - in);
		
		using sec = duration<double, std::ratio<1, 1>>;
		sec est2 = est;
		std::cout << est.count() << " frames = " << est2.count() << " seconds" << std::endl; ;
		est += 10_frame;
		std::cout << est.count() << " frames = " << (est2 = est).count() << " seconds" << std::endl; ;
	}
		
	{
		using namespace std::chrono_literals;
		std::promise<TestString> promise;
		auto res = promise.get_future();

		std::thread t([](auto p)
						{ 
							std::this_thread::sleep_for(3s);
							p.set_value(TestString("async_test"));
							// p.set_value(TestString("async_test")); //throw std::future_error

						}, std::move(promise));

		/*
		std::async([](auto p)
						{ 
							std::this_thread::sleep_for(5s);
							p.set_value(TestString("async_test"));
							
						}, std::move(promise));
		*/
		_ASSERT(res.valid());
		
		try
		{
			res.wait();
			auto temp = res.get();
		}
		catch (const std::future_error& ex)
		{
			std::cout << ex.what();
		}

		t.join();
		return 1;
	}

	decltype(std::declval<TestString>()) aaa = TestString("asdas");

	std::array<char, fib(5)> achar = {0};
	std::array<std::pair<int, double>, 3> myArray = {{{1, 1.0}, {2, 2.0}, {3, 3.0}}};
	
/*
	std::vector<int> v = {13, 42};
	std::multimap<std::string, double> mmap = {{"first", 1.1}, {"second", 2.2}, {"first", 1.9}};
	std::unordered_set<std::pair<int,int>> uset = {{1, 10}};

	auto il = {1,2,3};
	print_container(il);
	print_container(v);
	print_container(mmap);
	print_container(uset);
*/

/*
	{
		try
		{
			auto pred = [x = 42.0](double augment) mutable { x += augment;  return x; };
			std::packaged_task<double(double)> task(pred);

			auto taskFuture = task.get_future();
			task.get_future();
			std::thread th(std::move(task), 1.0);
			th.join();
			std::cout << taskFuture.get();
		}
		catch (const std::future_error& ex) { std::cout << ex.what(); }
	}
*/

	{
		std::once_flag flag;

		std::thread threads[10];
		for (auto& t : threads)
			t = std::thread([&flag](){ 
										std::call_once(flag, [](){ 
																	std::cout << "call once?\n"; 
																 }); 
									 });

		for (auto& t : threads)
			t.join();
		
		return 0;
	}
	
/*
	{
		auto f = [](std::thread t) { return t.join(); };

		//tf(f, std::thread([](){return 42;}));
		f(std::thread([](){return 42;}));

		std::thread t(f, std::thread([](){return 42;}));
		t.join();
	}
*/
	
	{	// template trash	

		auto sum = fold(std::make_index_sequence<10>{});
		sum += fold(100, 1000);

		auto il = {1, 2, 3};
		bar(std::initializer_list<int>{1, 2, 3});
		std::vector<std::pair<int, int>>().emplace_back(2, 3);

		auto ptr = &"Hello";
		int a = 1;

		// decltype(a)&& a2 = a;  //type of a2 is int&&
		decltype((a)) && a2 = a;   //type of a2 is int&
		decltype(auto) a3 = std::move(a);   //type of a3 is int&&

		tf(1, a);
		tf(1, 1);

		const int ci = 0;
		auto ai = bar<const int>(ci);
		decltype(auto) dai = bar<const int>(ci);

		//std::size_t n = sizeof(std::cout << 42);

		int& lva = a;
		const int& clva = a;

		int&& rva = 2;
		rva = 3;
		const int&& crva = 5;

		auto&& b1 = a;
		auto&& b2 = (a);
		auto&& b3 = lva;
		auto&& b4 = clva;
		auto&& b5 = rva;
		auto&& b6 = crva;
		auto&& b = int(1);

		//foo(std::move(a));
		//foo(a);
		//foo(lva);
		//foo(clva);
		foo<int&&>(std::move(rva));
		foo<const int&&>(int());

		// std::string& sref = std::string("qweqwe");   //non-standard extension. use /Za (Disable Language Extensions)
		
		/*
		std::cout << Numbers<1,2,3,9,8>::size << '\n';
		std::array<int, 4> array = {1,2,3,4};
		auto tuple = a2t(array);
		static_assert(std::is_same<decltype(tuple), std::tuple<int, int, int, int>>::value, "");
		*/
	}
	
	

	//WaitableTimer();
	
	//TestInterlockedSingleList();
	
	/*
	volatile LONG val = 0x66;
	LONG mask = 0xFF;
	val = InterlockedAnd(&val, mask);
	val = InterlockedOr(&val, mask);
	InterlockedXor(&val, mask);
	*/
	
	

	return 0;
}

