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
typedef std::string string;
typedef unsigned int size;
typedef double real;
typedef unsigned int amount;
#define win_nl(T, y) std::unique_ptr<T> y{ new T() }; //wska�nik do struktury    dodane
#define win_ptr(T, y) win_nl(T, y) y->dwLength = sizeof(T);  //rozmiar           dodane
class Mainwindow{
	std::shared_ptr<sf::RenderWindow> _window;
	std::shared_ptr<Command> _command;
	std::shared_ptr<Weather> weather;
	sf::Vector2f _size;
	sf::Color _backgroud_color;
	sf::RectangleShape _background;
	sf::Font _main_font;
	sf::Text _main_text;
	Flag _flag = Flag::null;
	string _city;
	string _process;
	bool _process_terminate;
	std::thread * _cmd_thr = nullptr;
	bool _change=true;
	static ULONGLONG __substract_time(const FILETIME one, const FILETIME two){      //dodane
		LARGE_INTEGER a, b;
		a.LowPart = one.dwLowDateTime;
		a.HighPart = one.dwHighDateTime;
		b.LowPart = two.dwLowDateTime;
		b.HighPart = two.dwHighDateTime;
		return a.QuadPart - b.QuadPart;
	}
	///clear from </span and space from begining
	void clear_string(string * str);
	void remove_space(string * str);
public:
	Mainwindow(sf::Vector2f size);
	Mainwindow(int x = 500, int y = 500);
	~Mainwindow();
	void run();
	void draw_process();
	void Create_weather_string();
	void draw_weather();
	void draw_memory();
	void draw_cpu();
	void kill_process();
	bool is_open();
	void set_flag(Flag f);
	void set_city(string city);
	void set_process(string proc, bool procter);
	void process_name_(DWORD processID){
		TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
		// Get a handle to the process.
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ,
			FALSE, processID);
		// Get the process name.
		if (NULL != hProcess)		{
			HMODULE hMod;
			DWORD cbNeeded;
			if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
				&cbNeeded))
			{
				GetModuleBaseName(hProcess, hMod, szProcessName,
					sizeof(szProcessName) / sizeof(TCHAR));
			}
		}
		// Print the process name and identifier.
		_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
		// Release the handle to the process.
		CloseHandle(hProcess);
	}
	size get_physical_memory() {               //dodane
		win_ptr(MEMORYSTATUSEX, ret);
		GlobalMemoryStatusEx(ret.get());
		return static_cast<size>(ret->ullTotalPhys);   //zapyta� si�
	}
	real get_physical_memory_usage(){          // dodane
		win_ptr(MEMORYSTATUSEX, ret);
		GlobalMemoryStatusEx(ret.get());
		return static_cast<size>(ret->dwMemoryLoad) / 100.0;   //zapyta� si�, dzielenie bo zwroci jako l ca�k. wyswietlanie jako zmienny przecinek
	}
	real get_core_number() {  //dodane
		win_nl(SYSTEM_INFO, ret);
		GetSystemInfo(ret.get());
		//ret.dwNumberOfProcessors;
		return static_cast<size>(ret->dwNumberOfProcessors);
	}
	size get_hz_per_core(){    //dodane
		win_nl(LARGE_INTEGER, ret);
		QueryPerformanceFrequency(ret.get());
		return static_cast<size>(ret->QuadPart); //suma first i second part  
	}
	real cpu_usage(){                       //dodane
		real ret{ 0.0 }; //bierze aktualne dane, po czasie 
		FILETIME prevSysIdle, prevSysKernel, prevSysUser;
		if (GetSystemTimes(&prevSysIdle, &prevSysKernel, &prevSysUser) == 0)
			return 0;
		Sleep(15
		);
		FILETIME sysIdle, sysKernel, sysUser;
		if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) == 0)
			return 0;
		if (prevSysIdle.dwLowDateTime != 0 && prevSysIdle.dwHighDateTime != 0){
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
