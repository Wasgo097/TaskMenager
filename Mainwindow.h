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
#include <tlhelp32.h>
#include <map>
#include <set>
#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <iomanip>
#include <sstream>
typedef std::string string;
typedef unsigned int size;
typedef double real;
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
	Flag _flag = Flag::help;
	Help _help = Help::init;
	string _city;
	string _process;
	bool _process_terminate;
	std::thread * _cmd_thr = nullptr;
	bool _change=true;
	Processes_info _process_sort;
	static ULONGLONG substract_time(const FILETIME one, const FILETIME two){
		LARGE_INTEGER a, b;
		a.LowPart = one.dwLowDateTime;
		a.HighPart = one.dwHighDateTime;
		b.LowPart = two.dwLowDateTime;
		b.HighPart = two.dwHighDateTime;
		return a.QuadPart - b.QuadPart;
	}
	void clear_string(string * str);
	void remove_space(string * str);
	string format_double(const double src, const unsigned short precision = 2){
		std::stringstream stream;
		stream << std::fixed << std::setprecision(precision) << src;
		return stream.str();
	}
public:
	Mainwindow(sf::Vector2f size);
	Mainwindow(int x = 500, int y = 500);
	~Mainwindow();
	void run();
	void draw_processes();
	void Create_weather_string();
	void draw_weather();
	void draw_memory();
	void draw_cpu();
	void draw_help();
	void kill_process();
	bool is_open();
	void set_flag(Flag f);
	void set_help(Help h);
	void set_city(string city);
	void set_process(string proc, bool procter);
	void set_process_info(Processes_info x);
	//void process_name_(DWORD processID){
	//	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	//	// Get a handle to the process.
	//	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
	//		PROCESS_VM_READ,
	//		FALSE, processID);
	//	// Get the process name.
	//	if (NULL != hProcess)		{
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
	size get_physical_memory();
	size get_virtual_memory();
	size get_avail_virtual_memory();
	real get_physical_memory_usage();
	real get_core_number();
	size get_hz_per_core();
	real cpu_usage();
};
