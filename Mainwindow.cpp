#include "Mainwindow.h"
#include <SFML/Network.hpp>
void Mainwindow::clear_string(string * str) {
	size_t span = str->find("</span");
	while (span < str->size()) {
		(*str) = str->substr(span + 7, str->size() - span - 7);
		span = str->find("</span");
	}
	std::size_t found = str->find_first_not_of(' ');
	str->erase(0, found);
}
void Mainwindow::remove_space(string * str) {
	size_t last_space = str->find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789");
	*str = str->substr(last_space + 1);
}
Mainwindow::Mainwindow(sf::Vector2f size) {
	if (!_main_font.loadFromFile("Font.ttf")) {
		std::cout << "Error with font";
		std::cin.ignore(1);
		exit(-1);
	}
	else {
		_size = size;
		_backgroud_color = sf::Color::Black;
		_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_size.x, _size.y), "Task Menager");
		_command = std::make_shared<Command>(this);
		_background.setFillColor(_backgroud_color);
		_background.setPosition(0, 0);
		_background.setSize(_size);
		_main_text.setFillColor(sf::Color::White);
		_main_text.setFont(_main_font);
		_main_text.setCharacterSize(15);
	}
}
Mainwindow::Mainwindow(int x, int y) {
	if (!_main_font.loadFromFile("Font.ttf")) {
		std::cout << "Error with font";
		std::cin.ignore(1);
		exit(-1);
	}
	else {
		_size.x = x;
		_size.y = y;
		_backgroud_color = sf::Color::Black;
		_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_size.x, _size.y), "Task Menager");
		_command = std::make_shared<Command>(this/*,&_mtx*/);
		_background.setFillColor(_backgroud_color);
		_background.setPosition(0, 0);
		_background.setSize(sf::Vector2f(_size));
		_main_text.setFillColor(sf::Color::White);
		_main_text.setFont(_main_font);
	}
}
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
		sf::Event event;
		while (_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				_window->close();
			if (event.type == sf::Event::Resized || event.type == sf::Event::GainedFocus)
				_change = true;
		}
		if (_change) {
			if (_flag == Flag::term) {
				kill_process();
			}
			else if (_flag == Flag::cpu) {
				draw_cpu();
			}
			else if (_flag == Flag::mem) {
				draw_memory();
			}
			else if (_flag == Flag::proc) {
				draw_process();
			}
			else if(_flag==Flag::wea){
				draw_weather();
			}
			else {
				_window->clear();
				_window->draw(_background);
				_window->display();
			}
			_change = false;
		}
	}
}
void Mainwindow::Create_weather_string(){
	if (weather != nullptr) {
		string text = "Weather in " + _city + "\n" + weather->weather_type;
		if (weather->min_temp == weather->max_temp)text += weather->min_temp + " C\n";
		else text += weather->min_temp + "..." + weather->max_temp + " C\n";
		text += "Wind: " + weather->wind_dir + " " + weather->wind_str + "km/h\n";
		text += "Rain: " + weather->water + " mm";
		_main_text.setString(text);
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
	}
	else {
		std::cout << "Error, request failed" << std::endl;
		set_flag(Flag::null);
	}
}
void Mainwindow::draw_process() {
	//GetLocalTime(&stLocal);
	//DWORD aProcesses[1024], cbNeeded, cProcesses;
	//unsigned int i;
	//if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	//{
	//	//Sleep(500);
	//}
	//// Calculate how many process identifiers were returned.
	//cProcesses = cbNeeded / sizeof(DWORD);
	//// Print the name and process identifier for each process.
	//for (i = 0; i < 10; i++)
	//{
	//
	//	if (aProcesses[i] != 0)
	//	{
	//		//PrintProcessNameAndID(aProcesses[i]);
	//		std::string tmpStr = "";
	//		tmpStr += "Procesy: \n";
	//		//tmpStr += "ZPM: ";
	//		double temp = aProcesses[i];//
	//		//double temp2 = get_physical_memory();//
	//		tmpStr += std::to_string(temp);
	//		//tmpStr += "\nPM: ";
	//		//tmpStr += std::to_string(temp2);
	//		_main_text.setString(tmpStr);//
	//		Sleep(500);
	//	}
	//}
	_main_text.setString("Proces: ");
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	//_window->draw(_main_text2);
	_window->display();
}
void Mainwindow::draw_weather() {
	if (weather == nullptr) {
		weather = std::make_shared<Weather>();
		std::map<string, string> wind_dir;
		wind_dir["↑"] = "polnoc";
		wind_dir["↗"] = "polnocny-zachod";
		wind_dir["→"] = "wschod";
		wind_dir["↘"] = "poludniowy-wschod";
		wind_dir["↓"] = "poludnie";
		wind_dir["↙"] = "poludniowy-zachod";
		wind_dir["←"] = "zachod";
		wind_dir["↖"] = "polnocny-zachod";
		_main_text.setString("Pogoda w miescie " + _city);
		sf::Http http("http://wttr.in/");
		sf::Http::Request request("/"+_city, sf::Http::Request::Get);
		sf::Http::Response response = http.sendRequest(request);
		// check the status
		if (response.getStatus() == sf::Http::Response::Ok)	{
			///pre
			size_t open = response.getBody().find("<pre>");
			size_t close = response.getBody().find("</pre>");
			string data = response.getBody().substr(open,close-open);
			size_t overcast = data.find("Overcast");
			size_t cloudy = data.find("Cloudy");
			size_t swallon_fog= data.find("Shallow Fog");
			if (overcast <60) {
				weather->weather_type = "Overcast\n";
				size_t temp_on, temp_off;
				temp_off = data.find("</span>");
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span>", temp_on);
				weather->min_temp = data.substr(temp_on + 20, temp_off - temp_on - 20);
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span>", temp_on);
				string temperature = data.substr(temp_on + 20, temp_off - temp_on - 20);
				if (temperature.length() < 5) {
					weather->max_temp = temperature;
					temp_off = data.find("</span>", temp_off+1);
				}
				else
					weather->max_temp = weather->min_temp;
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string direction;
				direction = data.substr(temp_on + 19, temp_off - temp_on - 19);
				weather->wind_dir = wind_dir[direction];
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string strong = data.substr(temp_on + 20, temp_off - temp_on - 20);
				weather->wind_str = strong;
				/*size_t mm = data.find("mm", temp_off);
				string water = data.substr(mm - 5, 5);
				remove_space(&water);
				weather->water = water;*/
			}
			else if (cloudy < 60) {
				weather->weather_type = "Cloudy\n";
				size_t temp_on, temp_off;
				temp_off = data.find("</span>");
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span>", temp_on);
				weather->min_temp = data.substr(temp_on + 20, temp_off - temp_on - 20);
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span>", temp_on);
				string temperature= data.substr(temp_on + 20, temp_off - temp_on - 20);
				if (temperature.length() < 5) weather->max_temp = temperature;
				else weather->max_temp = weather->min_temp;
				temp_on = data.find("<span", temp_off);
				temp_on = data.find("<span", temp_on+1);
				temp_off = data.find("</span", temp_on);
				string direction;
				direction = data.substr(temp_on + 19, temp_off - temp_on - 19);
				weather->wind_dir = wind_dir[direction];
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string strong = data.substr(temp_on + 20, temp_off - temp_on - 20);
				weather->wind_str = strong;
				/*size_t mm = data.find("mm", temp_off);
				string water = data.substr(mm - 5, 5);
				remove_space(&water);
				weather->water = water;*/
			}
			else if (swallon_fog < 60) {
				weather->weather_type = "Shallow Fog\n";
				size_t temp_on, temp_off;
				temp_off = data.find("</span>");
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span>", temp_on);
				weather->min_temp = data.substr(temp_on + 20, temp_off - temp_on - 20);
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span>", temp_on);
				string temperature = data.substr(temp_on + 20, temp_off - temp_on - 20);
				if (temperature.length() < 5) {
					weather->max_temp = temperature;
					temp_off = data.find("</span>", temp_off + 1);
				}
				else
					weather->max_temp = weather->min_temp;
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string direction;
				direction = data.substr(temp_on + 19, temp_off - temp_on - 19);
				weather->wind_dir = wind_dir[direction];
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string strong = data.substr(temp_on + 20, temp_off - temp_on - 20);
				weather->wind_str = strong;
			}
			else {
				size_t temp_on, temp_off;
				close = data.find("</span>");
				open = data.find("<span", close);
				string weather_type = data.substr(close + 7, open - close - 7);
				clear_string(&weather_type);
				weather->weather_type = weather_type;
				temp_off = data.find("</span>", open + 1);
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span>", temp_on);
				string temperature;
				temperature = data.substr(temp_on + 20, temp_off - temp_on - 20);
				weather->min_temp = temperature;
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				temperature = data.substr(temp_on + 20, temp_off - temp_on - 20);
				if (temperature.length() < 5) weather->max_temp = temperature;
				else weather->max_temp = weather->min_temp;
				temp_off = data.find("</span>", temp_off + 1);
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string direction;
				direction = data.substr(temp_on + 19, temp_off - temp_on - 19);
				weather->wind_dir = wind_dir[direction];
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string strong = data.substr(temp_on + 20, temp_off - temp_on - 20);
				weather->wind_str = strong;
			}
			size_t mm = data.find("mm"/*, temp_off*/);
			string water = data.substr(mm - 5, 5);
			remove_space(&water);
			weather->water = water;
			Create_weather_string();
		}
		else
			std::cout << "request failed" << std::endl;
	}
	else 
		Create_weather_string();
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::kill_process() {
	if (_process_terminate)
		_main_text.setString("Udalo sie zamknac proces " + _process);
	else
		_main_text.setString("Nie udalo sie zamknac procesu " + _process);
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::draw_memory() {
	std::string tmpStr = "";
	tmpStr += "Pamiec: \n";
	tmpStr += "ZPM: ";
	double temp = get_physical_memory_usage();//
	double temp2 = get_physical_memory();//
	tmpStr += std::to_string(temp);
	tmpStr += "\nPM: ";
	tmpStr += std::to_string(temp2);
	_main_text.setString(tmpStr);//
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::draw_cpu() {
	std::string tmpStr = "";
	tmpStr += "Number of Cores: ";
	double temp = get_core_number();
	double temp2 = cpu_usage();
	tmpStr += std::to_string(temp);
	tmpStr += "\nCpu Usage: ";
	tmpStr += std::to_string(temp2);
	_main_text.setString(tmpStr);//
	//_main_text.setString("Procesor");
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
bool Mainwindow::is_open() {
	return _window->isOpen();
}
void Mainwindow::set_flag(Flag f) {
	_change = true;
	_flag = f;
	weather.reset();
}
void Mainwindow::set_city(string city) {
	this->_city = city;
}
void Mainwindow::set_process(string proc, bool procter) {
	_process = proc;
	_process_terminate = procter;
}
