#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
	private:
		sf::CircleShape circ;
		sf::Color color;
		float mass = 5.f;
	public:
		sf::Vector2f velocity;
		sf::Vector2f acceleration = sf::Vector2f(0.f, 0.f);
		
		Ball() {};
		Ball(float radius, sf::Vector2f position, sf::Color color);
		void setPosition(sf::Vector2f NPos);
		sf::Vector2f getPosition();
		float getMass();
		float getRadius();
		void drawTo(sf::RenderWindow& window);
};

