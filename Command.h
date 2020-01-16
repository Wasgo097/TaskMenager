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
	void help(string temp);
	void show(string temp, string city = "");
<<<<<<< HEAD
	void kill(string name);
=======
	void kill(string id);
>>>>>>> df995e29a225298c097c0d5dc921483d00163261
	void clear();
public:
	Command(Mainwindow * window,string cmd="") :_command(cmd),_window(window){}
	string command() {return _command;}
	void command(string x) {_command = x;}
	void exec();
	void get();
	void run();
	~Command();
};

