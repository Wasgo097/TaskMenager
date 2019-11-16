#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "Command.h"
int main(){
	//sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Yellow);
	//shape.setPosition(sf::Vector2f(150, 150));
	//while (window.isOpen()){
	//	sf::Event event;
	//	while (window.pollEvent(event)){
	//		if (event.type == sf::Event::Closed||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	//			window.close();
	//		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) {
	//			/*std::string tekst;
	//			getline(std::cin,tekst);
	//			sf::Font font;
	//			font.loadFromFile("Font.ttf");
	//			sf::Text text;
	//			text.setFont(font);
	//			text.setString(tekst);*/
	//			char c;
	//			std::cin >> c;
	//			switch (c)
	//			{
	//			case 'r':
	//				shape.setFillColor(sf::Color::Red);
	//				break;
	//			case 'g':
	//				shape.setFillColor(sf::Color::Green);
	//				break;
	//			case 'b':
	//				shape.setFillColor(sf::Color::Blue);
	//				break;
	//			default:
	//				shape.setFillColor(sf::Color::Yellow);
	//			}
	//			window.clear();
	//			window.draw(shape);
	//			//window.draw(text);
	//			window.display();
	//			//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	//		}
	//	}
	//	window.clear();
	//	window.draw(shape);
	//	window.display();
	//}
	string x = "help show";
	Command cmd(x);
	cmd.exec();
	std::cin.ignore(1);
	return 0;
}