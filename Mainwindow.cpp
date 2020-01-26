#include "Mainwindow.h"
#include <SFML/Network.hpp>
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
void Mainwindow::clear_string(string * str) {
	size_t span = str->find("</span");
	while (span < str->size()) {
		(*str) = str->substr(span + 7, str->size() - span - 7);
		span = str->find("</span");
	}
	std::size_t found = str->find_first_not_of(' ');
	str->erase(0, found);
}
string Mainwindow::format_double(const double src, const unsigned short precision) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << src;
	return stream.str();
}
void Mainwindow::remove_space(string * str) {
	size_t last_space = str->find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789");
	*str = str->substr(last_space + 1);
}
Mainwindow::Mainwindow(int x, int y) {
	_size.x = x;
	_size.y = y;
	_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(_size.x, _size.y), "Task Menager");
	if (!_main_font.loadFromFile("Font.ttf")) {
		std::cout << "Error with font";
		std::cin.ignore(1);
		exit(-1);
	}
	else {
		_backgroud_color = sf::Color::Black;
		_command = std::make_shared<Command>(this);
		_background.setFillColor(_backgroud_color);
		_background.setPosition(0, 0);
		_background.setSize(_size);
		_main_text.setFillColor(sf::Color::White);
		_main_text.setFont(_main_font);
		_main_text.setCharacterSize(15);
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
			_change = false;
			if (_flag == Flag::term)kill_process();
			else if (_flag == Flag::cpu) draw_cpu();
			else if (_flag == Flag::mem) draw_memory();
			else if (_flag == Flag::proc) draw_processes();
			else if(_flag==Flag::wea)draw_weather();
			else if(_flag==Flag::help)draw_help();
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
		_main_text.setString("Error, request failed");
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
		set_flag(Flag::null);
	}
}
void Mainwindow::draw_processes() {
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
	DWORD pid = 0;
	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);
	std::set<string> all_processes;
	// Walkthrough all processes.
	if (Process32First(snapshot, &process)) {
		do {
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			//if (string(process.szExeFile) == string(name)) {
			//	//std::cout << process.szExeFile << std::endl;
			//	pid = process.th32ProcessID;
			//	break;
			//}
			PROCESS_MEMORY_COUNTERS_EX pmc;
			GetProcessMemoryInfo(GetProcessByName(process.szExeFile), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
			SIZE_T physMemUsedByMe = pmc.PrivateUsage;
			if (physMemUsedByMe / 1024 / 1024 != 3276)
				all_processes.insert(process.szExeFile + string(" ")+string("PID:")+std::to_string(process.th32ProcessID)+"  " + std::to_string(physMemUsedByMe / 1024 / 1024) + "MB");
			else
				all_processes.insert(process.szExeFile + string(" System process!"));
			//std::cout << process.szExeFile << std::endl;
		} while (Process32Next(snapshot, &process));
	}
	all_processes.erase("[System Process] System process!");
	string all;
	int i = 0;
	if (_process_sort.sort == Sort::asc) {
		all = "Processes:\n";
		for (const auto&x : all_processes) {
			if (i >= _process_sort.index) all += x + "\n";
			i++;
		}
	}
	else {
		all = "Processes:\n";
		std::vector<string> to_sort(all_processes.size());
		for (const auto&x : all_processes) {
				to_sort[i] = x;
				i++;
		}
		std::sort(to_sort.begin(), to_sort.end(), std::greater<string>());
		i = 0;
		for (const auto&x : to_sort) {
			if (i >= _process_sort.index) all += x + "\n";
			i++;
		}
	}
	_main_text.setString(all);
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::draw_weather() {
	if (weather == nullptr) {
		weather = std::make_shared<Weather>();
		std::map<string, string> wind_dir;
		wind_dir["↑"] = "N";
		wind_dir["↗"] = "NE";
		wind_dir["→"] = "E";
		wind_dir["↘"] = "SE";
		wind_dir["↓"] = "S";
		wind_dir["↙"] = "SW";
		wind_dir["←"] = "W";
		wind_dir["↖"] = "NW";
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
			size_t haze = data.find("Haze");
			size_t mist = data.find("Mist");
			size_t sunny= data.find("Sunny");
			size_t clear = data.find("Clear");
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
			else if (haze < 60) {
				weather->weather_type = "Haze\n";
				size_t temp_on, temp_off;
				temp_off = data.find("</span>");
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span>",temp_on);
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
				temp_off = data.find("</span>", temp_on);
				string direction;
				direction = data.substr(temp_on + 19, temp_off - temp_on - 19);
				weather->wind_dir = wind_dir[direction];
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string strong = data.substr(temp_on + 20, temp_off - temp_on - 20);
				weather->wind_str = strong;
			}
			else if (mist < 60) {
				weather->weather_type = "Mist\n";
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
				temp_off = data.find("</span>", temp_on);
				string direction;
				direction = data.substr(temp_on + 19, temp_off - temp_on - 19);
				weather->wind_dir = wind_dir[direction];
				temp_on = data.find("<span", temp_off);
				temp_off = data.find("</span", temp_on);
				string strong = data.substr(temp_on + 20, temp_off - temp_on - 20);
				weather->wind_str = strong;
			}
			else if (sunny < 120||clear<120) {
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
				//dir
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
		else {
			_main_text.setString("Request failed");
			_window->clear();
			_window->draw(_background);
			_window->draw(_main_text);
			_window->display();
		}
	}
	else 
		Create_weather_string();
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::kill_process() {
	/*if (_process_terminate)_main_text.setString("Udalo sie zamknac proces " + _process);
	else _main_text.setString("Nie udalo sie zamknac procesu " + _process);*/
	_main_text.setString("Function killing process");
	_window->clear();
	_window->draw(_background);
	_window->draw(_main_text);
	_window->display();
}
void Mainwindow::draw_memory() {
	const int maxsize = 300;
	int xtemp = 0;
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	sf::RectangleShape max;
	max.setFillColor(sf::Color::Green);
	max.setSize(sf::Vector2f(maxsize, 20));
	max.setPosition(sf::Vector2f(10, 150));
	sf::RectangleShape used;
	used.setFillColor(sf::Color::Blue);
	used.setPosition(sf::Vector2f(10, 180));
	sf::RectangleShape free;
	free.setFillColor(sf::Color::Yellow);
	while (!_change) {
		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
		DWORDLONG freePhysMem = memInfo.ullAvailPhys;
		DWORDLONG physMemUsed = totalPhysMem - freePhysMem;
		xtemp = (float(physMemUsed) / float(totalPhysMem)) * maxsize;
		used.setSize(sf::Vector2f(xtemp, 20));
		free.setPosition(10 + xtemp, 180);
		free.setSize(sf::Vector2f(maxsize - xtemp, 20));
		string temp = "RAM resources:\n";
		temp += "Total: " + std::to_string(totalPhysMem / 1024 / 1024) + " MB (green)\n";
		temp += "Free: " + std::to_string(freePhysMem / 1024 / 1024) + " MB (yellow)\n";
		temp += "Used: " + std::to_string(physMemUsed / 1024 / 1024) + " MB (blue)\n";
		_main_text.setString(temp);
		_window->clear();
		_window->draw(_background);
		_window->draw(max);
		_window->draw(free);
		_window->draw(used);
		_window->draw(_main_text);
		_window->display();
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}
void Mainwindow::draw_cpu() {
	init();
	const int maxsize = 300;
	//int i = 0;
	sf::RectangleShape max;
	max.setFillColor(sf::Color::White);
	max.setSize(sf::Vector2f(maxsize, 20));
	max.setPosition(sf::Vector2f(10, 150));
	sf::RectangleShape used;
	used.setPosition(sf::Vector2f(10, 150));
	while(!_change){
		double cputemp = getCurrentValue();
		used.setSize(sf::Vector2f(cputemp/100*maxsize, 20));
		if(cputemp<20.0)used.setFillColor(sf::Color::Blue);
		else if(cputemp>=20.0&&cputemp<50)used.setFillColor(sf::Color::Yellow);
		else if(cputemp>=20.0&&cputemp<50)used.setFillColor(sf::Color::Magenta);
		else used.setFillColor(sf::Color::Red);
		string temp = "CPU resources:\n";
		temp += "Number of cores: " + std::to_string(get_core_number()) + "\n";
		temp += "CPU speed: " + /*std::to_string(get_hz_per_core()/1000.0/1000.0)*/format_double(get_hz_per_core() / 1000.0 / 1000.0) + " GHz \n";
		temp += "CPU usage: " + /*std::to_string(cputemp)*/format_double(cputemp)+ " %\n";
		//std::cout << "";
		_main_text.setString(temp);
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->draw(max);
		_window->draw(used);
		_window->display();
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		//i++;
	}
}
void Mainwindow::draw_help() {
	if (_help == Help::unknow) {
		_main_text.setString("Nieznana komenda, uzyj help");
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
	}
	else if (_help == Help::show) {
		string temp;
		temp = "Mozesz wyswietlic:\n";
		temp += "show processes asc/desc x - lista procesow posortowanych\nwedlug nazwy rosnaco/malejaco zaczynajac od indexu x\n";
		temp += "show weather ..... - aktualna pogoda z miasta ....\n";
		temp += "show memory - aktualne zuzycie pamieci ram\n";
		temp += "show cpu - aktualne zuzycie procesora\n";
		_main_text.setString(temp);
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
	}
	else if (_help == Help::kill) {
		_main_text.setString("Zabija process o podanej nazwie");
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
	}
	else if (_help == Help::clear) {
		_main_text.setString("Czysci ekran konsoli");
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
	}
	else if (_help == Help::helpp) {
		_main_text.setString("Uzyj help show, help kill, help clear");
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
	}
	//init
	else {
		string temp;
		temp = "Mozesz wyswietlic:\n";
		temp += "	show processes asc/desc x - lista procesow posortowanych\n	wedlug nazwy rosnaco/malejaco	zaczynajac od indexu x\n";
		temp += "	show weather ..... - aktualna pogoda z miasta ....\n";
		temp += "	show memory - aktualne zuzycie pamieci ram\n";
		temp += "	show cpu - aktualne zuzycie procesora\n\n";
		temp += "Uzyj kill 'proces' aby zamknac proces\n";
		temp += "Uzyj clear aby wyczyscic ekran konsoli\n";
		_main_text.setString(temp);
		_window->clear();
		_window->draw(_background);
		_window->draw(_main_text);
		_window->display();
	}
}
bool Mainwindow::is_open() {return _window->isOpen();}
void Mainwindow::set_flag(Flag f) {
	_change = true;
	_flag = f;
	weather.reset();
}
void Mainwindow::set_help(Help h) {_help = h;}
void Mainwindow::set_city(string city) {this->_city = city;}
void Mainwindow::set_process(string proc, bool procter) {
	_process = proc;
	_process_terminate = procter;
}
void Mainwindow::set_process_info(Processes_info x) {_process_sort = x;}
void Mainwindow::init() {
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	// You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
	PdhAddEnglishCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);
}
double Mainwindow::getCurrentValue() {
	PDH_FMT_COUNTERVALUE counterVal;
	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	return counterVal.doubleValue;
}
int Mainwindow::get_core_number() {
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
}
double Mainwindow::get_hz_per_core() {
	LARGE_INTEGER ret;
	QueryPerformanceFrequency(&ret);
	return ret.QuadPart;
}
//size Mainwindow::get_physical_memory() {
//	win_ptr(MEMORYSTATUSEX, ret);
//	GlobalMemoryStatusEx(ret.get());
//	return static_cast<size>(ret->ullTotalPhys / 1024);   //zapyta� si�
//}
//size Mainwindow::get_virtual_memory() {
//	win_ptr(MEMORYSTATUSEX, ret);
//	GlobalMemoryStatusEx(ret.get());
//	return static_cast<size>(ret->ullTotalVirtual / 1024);   //zapyta� si�
//}
//size Mainwindow::get_avail_virtual_memory() {
//	win_ptr(MEMORYSTATUSEX, ret);
//	GlobalMemoryStatusEx(ret.get());
//	return static_cast<size>(ret->ullAvailVirtual / 1024);   //zapyta� si�
//}
//real Mainwindow::get_physical_memory_usage() {
//	win_ptr(MEMORYSTATUSEX, ret);
//	GlobalMemoryStatusEx(ret.get());
//	return static_cast<size>(ret->dwMemoryLoad / 1024) / 100.0;   //zapyta� si�, dzielenie bo zwroci jako l ca�k. wyswietlanie jako zmienny przecinek
//}
//real Mainwindow::cpu_usage() {
//	real ret{ 0.0 }; //bierze aktualne dane, po czasie 
//	FILETIME prevSysIdle, prevSysKernel, prevSysUser;
//	if (GetSystemTimes(&prevSysIdle, &prevSysKernel, &prevSysUser) == 0)
//		return 0;
//	Sleep(15
//	);
//	FILETIME sysIdle, sysKernel, sysUser;
//	if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) == 0)
//		return 0;
//	if (prevSysIdle.dwLowDateTime != 0 && prevSysIdle.dwHighDateTime != 0) {
//		LONG sysIdleDiff, sysKernelDiff, sysUserDiff;
//		sysIdleDiff = substract_time(sysIdle, prevSysIdle);
//		sysKernelDiff = substract_time(sysKernel, prevSysKernel);
//		sysUserDiff = substract_time(sysUser, prevSysUser);
//		LONG sysTotal = sysKernelDiff + sysUserDiff;
//		LONG kernelTotal = sysKernelDiff - sysIdleDiff;
//		ret = (int)(((kernelTotal + sysUserDiff) * 100.0) / sysTotal);
//	}
//	return ret;
//}