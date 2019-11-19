#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <chrono>
#include <string>
#include <memory>
typedef std::string string;
class Command;
class Mainwindow{
	std::shared_ptr<sf::RenderWindow> _window;
	sf::Vector2u _size;
	sf::Color _backgroud_color;
	sf::RectangleShape _background;
	sf::Font _main_font;
	sf::Text _main_text;
public:
	Mainwindow(sf::Vector2u size) :_size{ size }, _backgroud_color{ sf::Color::Black } {
		if (!_main_font.loadFromFile("Font.ttf")) {
			std::cout << "Error with font";
			std::cin.ignore(1);
			exit(-1);
		}
		else {
			_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_size.x, _size.y), "Task Menager");
			_background.setFillColor(_backgroud_color);
			_background.setPosition(0, 0);
			_background.setSize(sf::Vector2f(_size));
			_main_text.setFillColor(sf::Color::White);
			_main_text.setFont(_main_font);
		}
	}
	Mainwindow(int x=500, int y=500): _backgroud_color{ sf::Color::Black } {
		if (!_main_font.loadFromFile("Font.ttf")) {
			std::cout << "Error with font";
			std::cin.ignore(1);
			exit(-1);
		}
		else {
			_size.x = x;
			_size.y = y;
			_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_size.x, _size.y), "Task Menager");
			_background.setFillColor(_backgroud_color);
			_background.setPosition(0, 0);
			_background.setSize(sf::Vector2f(_size));
			_main_text.setFillColor(sf::Color::White);
			_main_text.setFont(_main_font);
		}
	}
	void run() {
		int x = 0;
		while (_window->isOpen()){
			x++;
			sf::Event event;
			while (_window->pollEvent(event)) {
				if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					_window->close();
			}
			if (x == 1000)draw_cpu();
			_window->clear();
			_window->draw(_background);
			_window->display();
		}
	}
	void draw_process() {
		_main_text.setString("Procesy");
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	void draw_weather(string city) {
		_main_text.setString("Pogoda w miescie "+city);
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	void draw_memory() {
		_main_text.setString("Pamiec");
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	void draw_cpu() {
		_main_text.setString("Procesor");
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
};