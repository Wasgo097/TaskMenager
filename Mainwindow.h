#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Command.h"
#include "common.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <string>
#include <memory>
#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <map>
#include<conio.h>
#include<stdio.h>
#include<tchar.h>
#include<psapi.h>
#include <iomanip>
#include <sstream>

typedef std::string string;
typedef unsigned int size;
typedef double real;
typedef unsigned int amount;
#define win_nl(T, y) std::unique_ptr<T> y{ new T() }; //wskaŸnik do struktury    dodane
#define win_ptr(T, y) win_nl(T, y) y->dwLength = sizeof(T);  //rozmiar           dodane
//#define DIV 1024.0;




class Mainwindow{
	
	std::shared_ptr<sf::RenderWindow> _window;
	std::shared_ptr<Command> _command;
	std::shared_ptr<Weather> weather;
	sf::Vector2f _size;
	sf::Color _backgroud_color;
	sf::RectangleShape _background;
	sf::Font _main_font;
	sf::Text _main_text;
	//sf::Text _main_text2;
	Flag _flag = Flag::null;
	string _city;
	std::thread * _cmd_thr = nullptr;
	bool _change=true;

	std::string format_double(const double src, const unsigned short precision = 2) //metoda zmieiaj¹ca precyzjê wyœwietlanych wyników
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(precision) << src;
		return stream.str();
	}

	
	ULONGLONG __substract_time(const FILETIME one, const FILETIME two)      //dodane
	{
		LARGE_INTEGER a, b;
		a.LowPart = one.dwLowDateTime;
		a.HighPart = one.dwHighDateTime;

		b.LowPart = two.dwLowDateTime;
		b.HighPart = two.dwHighDateTime;

		return a.QuadPart - b.QuadPart;
	}
	void clear_string(string * str) {
		size_t span = str->find("</span");
		while (span < str->size()) {
			(*str) = str->substr(span + 7, str->size() - span - 7);
			span = str->find("</span");
		}
		std::size_t found = str->find_first_not_of(' ');
		str->erase(0, found);
	}
public:
	Mainwindow(sf::Vector2f size);
	Mainwindow(int x = 500, int y = 500);
	~Mainwindow();
	void run();
	void draw_process();
	void draw_weather();
	void draw_memory();
	void draw_cpu();
	bool is_open();
	void set_flag(Flag f);
	void set_city(string city);

	//void display_local_time() {
	//	SYSTEMTIME stSystem;
	//	
	//	GetLocalTime(&stSystem);
	//	
	//	stSystem.wYear, stSystem.wMonth, stSystem.wDay, //</span /> Date
	//	stSystem.wHour, stSystem.wMinute, stSystem.wSecond, stSystem.wMilliseconds);
	//	
	//}
	//SYSTEMTIME stLocal;

	//void process_name_(DWORD processID)
	//{
	//	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	//	// Get a handle to the process.

	//	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
	//		PROCESS_VM_READ,
	//		FALSE, processID);

	//	// Get the process name.

	//	if (NULL != hProcess)
	//	{
	//		HMODULE hMod;
	//		DWORD cbNeeded;

	//		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
	//			&cbNeeded))
	//		{
	//			GetModuleBaseName(hProcess, hMod, szProcessName,
	//				sizeof(szProcessName) / sizeof(TCHAR));
	//		}
	//	}

	//	// Print the process name and identifier.

	//	_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

	//	// Release the handle to the process.

	//	CloseHandle(hProcess);
	//}
	size get_physical_memory()                //dodane
	{
		win_ptr(MEMORYSTATUSEX, ret);
		GlobalMemoryStatusEx(ret.get());
		return static_cast<size>(ret->ullTotalPhys/1024);   //zapytaæ siê
	}
	size get_virtual_memory()                //dodane
	{
		win_ptr(MEMORYSTATUSEX, ret);
		GlobalMemoryStatusEx(ret.get());
		return static_cast<size>(ret->ullTotalVirtual/1024);   //zapytaæ siê
	}
	size get_avail_virtual_memory()                //dodane
	{
		win_ptr(MEMORYSTATUSEX, ret);
		GlobalMemoryStatusEx(ret.get());
		return static_cast<size>(ret->ullAvailVirtual/1024);   //zapytaæ siê
	}
	real get_physical_memory_usage()          // dodane
	{
		win_ptr(MEMORYSTATUSEX, ret);
		GlobalMemoryStatusEx(ret.get());
		return static_cast<size>(ret->dwMemoryLoad/1024) / 100.0;   //zapytaæ siê, dzielenie bo zwroci jako l ca³k. wyswietlanie jako zmienny przecinek
	}
	real get_core_number() {  //dodane
		win_nl(SYSTEM_INFO, ret);
		GetSystemInfo(ret.get());
		//ret.dwNumberOfProcessors;
		return static_cast<size>(ret->dwNumberOfProcessors);
	}
	//size get_hz_per_core()    //dodane
	//{
	//	win_nl(LARGE_INTEGER, ret);
	//	QueryPerformanceFrequency(ret.get());
	//	return static_cast<size>(ret->QuadPart); //suma first i second part  
	//}
	real cpu_usage()                       //dodane
	{
		real ret{ 0.0 }; //bierze aktualne dane, po czasie 
		FILETIME prevSysIdle, prevSysKernel, prevSysUser;
		if (GetSystemTimes(&prevSysIdle, &prevSysKernel, &prevSysUser) == 0)
			return 0;
		Sleep(15
		);
		FILETIME sysIdle, sysKernel, sysUser;
		if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) == 0)
			return 0;

		if (prevSysIdle.dwLowDateTime != 0 && prevSysIdle.dwHighDateTime != 0)
		{
			LONG sysIdleDiff, sysKernelDiff, sysUserDiff;
			sysIdleDiff = __substract_time(sysIdle, prevSysIdle);
			sysKernelDiff = __substract_time(sysKernel, prevSysKernel);
			sysUserDiff = __substract_time(sysUser, prevSysUser);

			LONG sysTotal = sysKernelDiff + sysUserDiff;
			LONG kernelTotal = sysKernelDiff - sysIdleDiff;

			ret = (int)(((kernelTotal + sysUserDiff) * 100.0) / sysTotal);
		}
		return ret;
	}
};
