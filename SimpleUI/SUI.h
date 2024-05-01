#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/Window/Mouse.hpp>
#ifndef SUI_H
#define SUI_H

namespace SUI {
	class Button
	{
	private:
		sf::RectangleShape btn;
		sf::Text text;
	public:
		
		Button(sf::Vector2f size, sf::Vector2f pos);
		std::string getTextString();
		void setFont(sf::Font& font);
		void setTextString(std::string str);
		void setPosition(sf::Vector2f pos);
		void setColor(sf::Color col);
		void setTextColor(sf::Color col);
		void centerText();
		bool isHovered(sf::RenderWindow& window);

		void drawTo(sf::RenderWindow& window);
	};

}
#endif // !SUI.H



