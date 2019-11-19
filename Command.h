#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <vector>
typedef std::string string;
class Mainwindow;
class Command{
	std::shared_ptr<Mainwindow> window;
	string _command;
	void help(string temp) {
		if (temp == "show") 
			std::cout << "Mozesz wyswietlic:" << std::endl<<
				"show process - lista procesow"<<std::endl<<
				"show weather 'MIASTO' - aktualna pogoda z miasta 'MIASTO'"<<std::endl<<
				"show memory - aktualne zuzycie pamieci ram"<<std::endl<<
				"show cpu - aktualne zuzycie procesora"<<std::endl;
		else if (temp == "kill") 
			std::cout << "Zabija process o podanym id" << std::endl;
		else if (temp == "clear") 
			std::cout << "Czysci ekran konsoli" << std::endl;
		else 
			std::cout << "Nieznana komenda, uzyj help show, help kill, help clear" << std::endl;
	}
	void show(string temp,string city="") {
		if (city == "") {
			if (temp == "process") {

			}
			else if (temp == "cpu") {

			}
			else if (temp == "memory") {

			}
			else {
				std::cout << "Nieznana komenda, uzyj help show";
			}
		}
		else if (temp == "weather" && (!city.empty())) {
			std::cout << "Pogoda w " + city + " jest idealna, nie za cieplo, nie za zimno, tak w sam raz";
		}
		else {
			std::cout << "Nieznana komenda, uzyj help show";
		}
	}
	void kill(string id) {

	}
	void clear() {system("cls");}
public:
	Command(string cmd="") :_command(cmd) {}
	string command() {return _command;}
	void command(string x) {_command = x;}
	void exec() {
		std::vector<string> full_cmd;
		string temp = "";
		for (const auto&x:_command) {
			if (x != ' ')
				temp += x;
			else {
				full_cmd.push_back(temp);
				temp.clear();
			}
		}
		full_cmd.push_back(temp);
		temp.clear();
		if (full_cmd[0]=="show"){
			if(full_cmd.size()==2) show(full_cmd[1]);
			else show(full_cmd[1], full_cmd[2]);
		}
		else if (full_cmd[0] == "help") 
			help(full_cmd[1]);
		else if (full_cmd[0] == "kill") 
			std::cout << "kill";
		else std::cout << "Nieznana komenda";
	}
	~Command();
};

