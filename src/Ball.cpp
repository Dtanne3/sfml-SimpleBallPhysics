#include "Ball.h"

Ball::Ball(float radius, sf::Vector2f position, sf::Color color)
{
	circ.setRadius(radius);
	circ.setPosition(position);
	circ.setFillColor(color);
}
void Ball::setPosition(sf::Vector2f NPos)
{
	circ.setPosition(NPos);
}

sf::Vector2f Ball::getPosition() 
{
	return circ.getPosition();
}

float Ball::getMass()
{
	return mass;
}
float Ball::getRadius()
{
	return circ.getRadius();
}
void Ball::drawTo(sf::RenderWindow& window)
{
	window.draw(circ);
}
