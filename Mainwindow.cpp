#include "Mainwindow.h"
#include <SFML/Network.hpp>
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
		//_main_text2.setFillColor(sf::Color::White);//
		//_main_text2.setFont(_main_font);//
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
		//_main_text2.setFillColor(sf::Color::White);//
		//_main_text2.setFont(_main_font);//
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
		//if (_change) {
			if (_flag == Flag::null) {
				_window->clear();
				_window->draw(_background);
				_window->display();
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
			else {
				draw_weather();
			}
			_change = false;
		}
	}
//}
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
			///first span, weather type
			close = data.find("</span>");
			//close = data.find("</span>", close + 1);
			open = data.find("<span",close);
			string weather_type = data.substr(close+7, open-close-7);
			clear_string(&weather_type);
			weather->weather_type = weather_type;
			//std::cout << weather_type;
			string temperature;
			size_t temp_on, temp_off;
			temp_off = data.find("</span>", open+1);
			temp_on = data.find("<span", temp_off);
			temp_off = data.find("</span>", temp_on);
			temperature = data.substr(temp_on+20, temp_off - temp_on-20);
			//dolny zakres
			weather->min_temp = temperature;
			//std::cout << temperature<<" ";

			temp_on = data.find("<span", temp_off);
			temp_off = data.find("</span", temp_on);
			temperature = data.substr(temp_on + 20, temp_off - temp_on - 20);
			//gorny zakres
			weather->max_temp = temperature;
			//std::cout << temperature<<std::endl;
		
			temp_off = data.find("</span>", temp_off+1);
			temp_on = data.find("<span", temp_off);
			temp_off = data.find("</span", temp_on);
			string direction;
			direction = data.substr(temp_on+19, temp_off - temp_on-19);
			weather->wind_dir = wind_dir[direction];
			//std::cout << wind_dir[direction]<<std::endl;

			temp_on = data.find("<span", temp_off);
			temp_off = data.find("</span", temp_on);
			string strong= data.substr(temp_on + 20, temp_off - temp_on - 20);
			weather->wind_str = strong;
			//std::cout << strong<<std::endl;

			temp_off = data.find("</span", temp_off+1);
			temp_off = data.find("</span", temp_off+1);
			temp_off = data.find("</span", temp_off+1);
			temp_off = data.find("</span", temp_off+1);
			temp_off = data.find("</span", temp_off+1);
			temp_off = data.find("</span", temp_off+1);
			size_t f_m = data.find('m', temp_off);
			string water = data.substr(temp_off+8, f_m - temp_off-8);
			if (water.size() < 10)
				//std::cout << water << std::endl;
				weather->water = water;
			else {
				temp_off = data.find("</span", temp_on);
				temp_off = data.find("</span", temp_off + 1);
				temp_off = data.find("</span", temp_off + 1);
				f_m = data.find('m', temp_off);
				temp_off = f_m;
				f_m= data.find('m', f_m+1);
				water = data.substr(temp_off, f_m - temp_off);
				size_t last_space = water.find_first_not_of("abcdefghijklmnopqrstuvwxyz ");
				water=water.substr(last_space + 1);
				//double x = std::stod(water);
				weather->water = water;
				//std::cout << x << std::endl;
			}
		}
		else{
			std::cout << "request failed" << std::endl;
		}
	}
	else {
		std::cout <<std::endl<< "Nie musze znowu wchodzic i szukac!!"<<std::endl;
	}
	std::cout << weather->weather_type << weather->min_temp << "..." << weather->max_temp << std::endl <<
		weather->wind_dir << " " << weather->wind_str << std::endl <<
		weather->water << std::endl;
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}

	void Mainwindow::draw_memory() {
		//do{
		
			//_main_text.setString("Pamiec: \n");
		//string temptime = GetLocalTime(&stLocal);
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
			Sleep(500);
		
			
		//} while (_getch() != '27');
	}

	void Mainwindow::draw_cpu() {
		//do {
			std::string tmpStr = "";
			tmpStr += "Number of Cores: ";
			double temp = get_core_number();//
			double temp2 = cpu_usage();//
			tmpStr += std::to_string(temp);
			tmpStr += "\nCpu Usage: ";
			tmpStr += std::to_string(temp2);
			//tmpStr2 += std::to_string(temp2);
			_main_text.setString(tmpStr);//
			//_main_text.setString("Procesor");
			_window->clear();
			_window->draw(_background);
			_window->draw(_main_text);
			_window->display();
			Sleep(500);
		/*}
			if (GetKeyState('x') < 0) {
				Sleep(1000);
			}
			else {
				break;
			}*/
	/*}while (_getch() != '27');*/

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
