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
#include <Windows.h>
#include <iphlpapi.h>
#include <map>
typedef std::string string;
typedef unsigned long size;
typedef double real;
typedef unsigned int amount;
#define win_nl(T, y) std::unique_ptr<T> y{ new T() }; //wskaŸnik do struktury    dodane
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
	static ULONGLONG __substract_time(const FILETIME one, const FILETIME two)      //dodane
	{
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
	//void set_process(string proc, bool procter);
	//static size get_physical_memory()                //dodane
	//{
	//	win_ptr(MEMORYSTATUSEX, ret);
	//	GlobalMemoryStatusEx(ret.get());
	//	return static_cast<size>(ret->ullTotalPhys);   //zapytaæ siê
	//}
	//static real get_physical_memory_usage()          // dodane
	//{
	//	win_ptr(MEMORYSTATUSEX, ret);
	//	GlobalMemoryStatusEx(ret.get());
	//	return static_cast<size>(ret->dwMemoryLoad) / 100.0;   //zapytaæ siê, dzielenie bo zwroci jako l ca³k. wyswietlanie jako zmienny przecinek
	//}
	//static size get_hz_per_core()    //dodane
	//{
	//	win_nl(LARGE_INTEGER, ret);
	//	QueryPerformanceFrequency(ret.get());
	//	return static_cast<size>(ret->QuadPart); //suma first i second part  
	//}
	//static real cpu_usage()                       //dodane
	//{
	//	real ret{ 0.0 }; //bierze aktualne dane, po czasie 
	//	FILETIME prevSysIdle, prevSysKernel, prevSysUser;
	//	if (GetSystemTimes(&prevSysIdle, &prevSysKernel, &prevSysUser) == 0)
	//		return 0;
	//	Sleep(15
	//	);
	//	FILETIME sysIdle, sysKernel, sysUser;
	//	if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) == 0)
	//		return 0;
	//	if (prevSysIdle.dwLowDateTime != 0 && prevSysIdle.dwHighDateTime != 0)
	//	{
	//		ULONGLONG sysIdleDiff, sysKernelDiff, sysUserDiff;
	//		sysIdleDiff = __substract_time(sysIdle, prevSysIdle);
	//		sysKernelDiff = __substract_time(sysKernel, prevSysKernel);
	//		sysUserDiff = __substract_time(sysUser, prevSysUser);
	//		ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
	//		ULONGLONG kernelTotal = sysKernelDiff - sysIdleDiff;
	//		ret = (double)(((kernelTotal + sysUserDiff) * 100.0) / sysTotal);
	//	}
	//	return ret;
	//}
};