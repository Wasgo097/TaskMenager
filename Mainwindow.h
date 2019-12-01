#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Command.h"
#include "common.h"
#include <thread>
#include <chrono>
#include <string>
#include <memory>
#include <mutex>
#include <iostream>
typedef std::string string;
class Mainwindow{
	std::shared_ptr<sf::RenderWindow> _window;
	std::shared_ptr<Command> _command;
	sf::Vector2f _size;
	sf::Color _backgroud_color;
	sf::RectangleShape _background;
	sf::Font _main_font;
	sf::Text _main_text;
	Flag _flag = Flag::null;
	string _city;
	std::thread * _cmd_thr = nullptr;
	std::mutex _mtx;
public:
	Mainwindow(/*std::mutex * mt,*/ sf::Vector2f size) :_size{ size }, _backgroud_color{ sf::Color::Black }/*, mtx{ mt }*/{
		if (!_main_font.loadFromFile("Font.ttf")) {
			std::cout << "Error with font";
			std::cin.ignore(1);
			exit(-1);
		}
		else {
			_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_size.x, _size.y), "Task Menager");
			_command = std::make_shared<Command>(this,&_mtx);
			_background.setFillColor(_backgroud_color);
			_background.setPosition(0, 0);
			_background.setSize(_size);
			_main_text.setFillColor(sf::Color::White);
			_main_text.setFont(_main_font);
		}
	}
	Mainwindow(/*std::mutex * mt,*/ int x = 500, int y = 500) : _backgroud_color{ sf::Color::Black }/*, mtx{ mt }*/ {
		if (!_main_font.loadFromFile("Font.ttf")) {
			std::cout << "Error with font";
			std::cin.ignore(1);
			exit(-1);
		}
		else {
			_size.x = x;
			_size.y = y;
			_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_size.x, _size.y), "Task Menager");
			_command = std::make_shared<Command>(this,&_mtx);
			_background.setFillColor(_backgroud_color);
			_background.setPosition(0, 0);
			_background.setSize(sf::Vector2f(_size));
			_main_text.setFillColor(sf::Color::White);
			_main_text.setFont(_main_font);
		}
	}
	~Mainwindow();
	void run();
	void draw_process();
	void draw_weather();
	void draw_memory();
	void draw_cpu();
	bool is_open();
	void set_flag(Flag f);
	void set_city(string city);
};