#include "Mainwindow.h"
Mainwindow::~Mainwindow() {
	_cmd_thr->join();
	delete _cmd_thr;
}
void Mainwindow::run() {
	_cmd_thr = new std::thread(&Command::run,_command);
	_window->clear();
	_window->draw(_background);
	_window->display();
	while (_window->isOpen()) {
		//mtx->lock();
		sf::Event event;
		while (_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				_window->close();
		}
		if (_flag == Flag::null) {
			_window->clear();
			_window->draw(_background);
			_window->display();
		}
		else if (_flag==Flag::cpu) {
			draw_cpu();
		}
		else if (_flag==Flag::mem) {
			draw_memory();
		}
		else if (_flag==Flag::proc) {
			draw_process();
		}
		//wea
		else{
			draw_weather();
		}
		//mtx->unlock();
	}
}
void Mainwindow::draw_process() {
	_main_text.setString("Procesy");
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::draw_weather() {
	_main_text.setString("Pogoda w miescie " + _city);
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::draw_memory() {
	_main_text.setString("Pamiec");
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::draw_cpu() {
	_main_text.setString("Procesor");
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
bool Mainwindow::is_open() {
	return _window->isOpen();
}
void Mainwindow::set_flag(Flag f) {
	_flag = f;
}
void Mainwindow::set_city(string city) {
	this->_city = city;
}
