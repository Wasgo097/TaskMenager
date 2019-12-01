#pragma once
#include "common.h"
#include <memory>
#include <iostream>
#include <string>
#include <mutex>
#include <vector>
typedef std::string string;
class Mainwindow;
class Command{
	Mainwindow * _window;
	string _command;
	std::mutex * mtx;
	void help(string temp);
	void show(string temp, string city = "");
	void kill(string id);
	void clear();
public:
	Command(Mainwindow * window,std::mutex * mtx,string cmd="") :_command(cmd),_window(window),mtx(mtx) {}
	string command() {return _command;}
	void command(string x) {_command = x;}
	void exec();
	void get();
	void run();
	~Command();
};

