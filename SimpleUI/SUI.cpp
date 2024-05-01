#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <stdio.h>
#include <string>
#include "SUI.h"

//Button Class
SUI::Button::Button(sf::Vector2f size, sf::Vector2f pos)
{
	btn.setSize(size);
	btn.setPosition(pos);
}
std::string SUI::Button::getTextString()
{
	return text.getString();
}
void SUI::Button::setFont(sf::Font& font)
{
	text.setFont(font);
}
void SUI::Button::setTextString(std::string str)
{
	text.setString(str);
}
void SUI::Button::setPosition(sf::Vector2f pos)
{
	btn.setPosition(pos);
}
void SUI::Button::setColor(sf::Color col)
{
	btn.setFillColor(col);
}
void SUI::Button::setTextColor(sf::Color col)
{
	text.setFillColor(col);
}
void SUI::Button::centerText()
{
	float xPos = btn.getPosition().x + (btn.getLocalBounds().width / 2) - (text.getGlobalBounds().width/2);
	float yPos = btn.getPosition().y + (btn.getLocalBounds().height / 2) - (text.getGlobalBounds().height / 2);
	text.setPosition(xPos, yPos);
}
bool SUI::Button::isHovered(sf::RenderWindow& window)
{
	float mPosX = sf::Mouse::getPosition(window).x;
	float mPosY = sf::Mouse::getPosition(window).y;
	//if mouse is within button coordinates
	if (mPosX > btn.getPosition().x && mPosX <= btn.getPosition().x + btn.getLocalBounds().width && 
	mPosY > btn.getPosition().y && mPosY <= btn.getPosition().y + btn.getLocalBounds().height)
	{
		return true;
	}
	return false;
}
void SUI::Button::drawTo(sf::RenderWindow& window)
{
	window.draw(btn);
	centerText();
	window.draw(text);
}
