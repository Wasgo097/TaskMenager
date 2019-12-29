#include <iostream>
#include "Mainwindow.h"
int main(){
	/*int x=500, y=500;
	std::cout << "x: "; std::cin >> x;
	std::cout << "y: "; std::cin >> y;
	Mainwindow window(sf::Vector2f(x,y));*/
	Mainwindow window;
	window.run();
	return 0;
}
//#define _WIN32_WINNT  0x0501 // I misunderstand that
//#include <windows.h>
//#include <iostream>
//#include <cstdlib>
//
//using namespace std;
//
//int main()
//{
//	MEMORYSTATUSEX statex;
//
//	statex.dwLength = sizeof(statex); // I misunderstand that
//
//	GlobalMemoryStatusEx(&statex);
//	cout << "Physical RAM => " << (float)statex.ullTotalPhys / (1024 * 1024 * 1024) << endl;
//
//	system("PAUSE");
//	return EXIT_SUCCESS;
//}