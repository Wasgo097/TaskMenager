#include "Command.h"
#include "Mainwindow.h"
#include <tlhelp32.h>
#include <Windows.h>
#include <iostream>
HANDLE GetProcessByName(PCSTR name) {
	DWORD pid = 0;
	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);
	// Walkthrough all processes.
	if (Process32First(snapshot, &process)) {
		do {
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			if (string(process.szExeFile) == string(name)) {
				//std::cout << process.szExeFile << std::endl;
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}
	CloseHandle(snapshot);
	if (pid != 0)
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	// Not found
	return nullptr;
}
void Command::help(string temp) {
	/*
	if (temp == "show")
		std::cout << "Mozesz wyswietlic:" << std::endl <<
		"show processes - lista procesow" << std::endl <<
		"show weather ..... - aktualna pogoda z miasta ...." << std::endl <<
		"show memory - aktualne zuzycie pamieci ram" << std::endl <<
		"show cpu - aktualne zuzycie procesora" << std::endl;
	else if (temp == "kill")
		std::cout << "Zabija process o podanym id" << std::endl;
	else if (temp == "clear")
		std::cout << "Czysci ekran konsoli" << std::endl;
	else
		std::cout << "Nieznana komenda, uzyj help show, help kill, help clear" << std::endl;
	*/
	if (temp == "show") _window->set_help(Help::show);
	else if (temp == "kill")_window->set_help(Help::kill);
	else if (temp == "clear")_window->set_help(Help::clear);
	else _window->set_help(Help::unknow);
	_window->set_flag(Flag::help);
}
void Command::show(string temp, string city) {
	if (city == "") {
		if (temp == "processes") _window->set_flag(Flag::proc);
		else if (temp == "cpu") _window->set_flag(Flag::cpu);
		else if (temp == "memory") _window->set_flag(Flag::mem);
		else {
			_window->set_flag(Flag::help);
			_window->set_help(Help::unknow);
		}
	}
	else if (temp == "weather" && (!city.empty())) {
		_window->set_flag(Flag::wea);
		_window->set_city(city);
	}
	else {
		_window->set_flag(Flag::help);
		_window->set_help(Help::unknow);
	}
}
void Command::kill(string name) {
	_window->set_flag(Flag::term);
	HANDLE handle = nullptr;
	handle = GetProcessByName(name.c_str());
	if (handle == nullptr) _window->set_process(name, false);
	else {
		TerminateProcess(handle, 0);
		_window->set_process(name, true);
	}
		
}
void Command::clear() { system("cls"); }
void Command::exec() {
	if (!_command.empty()) {
		std::vector<string> full_cmd;
		string temp = "";
		for (const auto&x : _command) {
			if (x != ' ')
				temp += x;
			else {
				full_cmd.push_back(temp);
				temp.clear();
			}
		}
		full_cmd.push_back(temp);
		if (full_cmd.size() >1) {
			if (full_cmd[0] == "show") {
				if (full_cmd.size() == 2) show(full_cmd[1]);
				else if (full_cmd[1] == "processes") {
					Processes_info temp;
					if (full_cmd.size() == 3) {
						if (full_cmd[2] == "desc")temp.sort = Sort::desc;
						else temp.sort = Sort::asc;
					}
					if (full_cmd.size() == 4) {
						if (full_cmd[2] == "desc")temp.sort = Sort::desc;
						else temp.sort = Sort::asc;
						temp.index = std::stoi(full_cmd[3]);
					}
					_window->set_process_info(temp);
					show(full_cmd[1]);
				}
				else {
					string city = "";
					for (int i = 2; i < full_cmd.size(); i++) city += full_cmd[i] + "+";
					city.erase(city.end() - 1);
					show(full_cmd[1], city);
				}
			}
			else if (full_cmd[0] == "help")help(full_cmd[1]);
			else if (full_cmd[0] == "kill")kill(full_cmd[1]);
			else {
				_window->set_flag(Flag::help);
				_window->set_help(Help::unknow);
			}
		}
		else if (full_cmd.size() == 1 && full_cmd[0] == "clear") clear();
		else if (full_cmd.size() == 1 && full_cmd[0] == "help") {
			_window->set_flag(Flag::help);
			_window->set_help(Help::helpp);
		}
		else { 
			_window->set_flag(Flag::help);
			_window->set_help(Help::unknow);
		}
	}
}
void Command::get() {
	std::getline(std::cin,_command);
}
void Command::run() {
	while (_window->is_open()){
		get();
		exec();
	}
}
Command::~Command(){}
