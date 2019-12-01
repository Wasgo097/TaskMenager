#include <iostream>
#include <SFML/Graphics.hpp>
//#include <thread>
//#include <mutex>
//#include <chrono>
#include "Mainwindow.h"
#include "Command.h"
int main(){
	/*int x=500, y=500;
	std::cout << "x: "; std::cin >> x;
	std::cout << "y: "; std::cin >> y;
	Mainwindow window(&mtx,sf::Vector2f(x,y));*/
	Mainwindow window;
	window.run();
	return 0;
}