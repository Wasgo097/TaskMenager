#pragma once
#include "common.h"
#include <memory>
#include <iostream>
#include <string>
#include <mutex>
#include <vector>
#include <Windows.h>
#include <tlhelp32.h>
typedef std::string string;
class Mainwindow;
class Command{
	Mainwindow * _window;
	string _command;
	void help(string temp);
	void show(string temp, string city = "");
	void kill(string name);
	void clear();
	//HANDLE GetProcessByName(PCSTR name);
public:
	Command(Mainwindow * window,string cmd="") :_command(cmd),_window(window){}
	string command() {return _command;}
	void command(string x) {_command = x;}
	void exec();
	void get();
	void run();
	~Command();
};

