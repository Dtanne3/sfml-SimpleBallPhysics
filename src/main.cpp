#include <stdio.h>
#include <vector>
#include <valarray>
#include <thread>
#include <numeric>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "SUI.h"
#include "Ball.h"
#include "QuadTree.h"


bool checkBoundsCollisionX(float posX, sf::RectangleShape& panel, float r)
{
	if (posX < panel.getPosition().x || posX > panel.getPosition().x + panel.getLocalBounds().width - r)
	{
		return true;
	}
	return false;
}

bool checkBoundsCollisionY(float posY, sf::RectangleShape& panel, float r)
{
	if (posY < panel.getPosition().y || posY > panel.getPosition().y + panel.getLocalBounds().height - r)
	{
		return true;
	}
	return false;
}

float calcDist(Ball& b1, Ball& b2)
{
	return sqrt(pow((b2.getPosition().x - b1.getPosition().x), 2) + pow((b2.getPosition().y - b1.getPosition().y), 2));
}

void calcPos(Ball& b, sf::RectangleShape& panel, float dT)
{
	float yForce = (b.getMass() * 9.8);
	float xVel = b.velocity.x;
	float yVel = b.velocity.y + (yForce * dT);
	b.velocity = sf::Vector2f(xVel, yVel);
	printf("Ball velocity: (%f , %f)\n", xVel, yVel);
	
	
	sf::Vector2f nPos = sf::Vector2f((b.getPosition().x + b.velocity.x), (b.getPosition().y + b.velocity.y));
	if (checkBoundsCollisionX(nPos.x, panel, b.getRadius()))
	{
		float minBound = panel.getPosition().x;
		float maxBound = (panel.getPosition().x + panel.getLocalBounds().width);
		float newX = (nPos.x < minBound) ? minBound + b.getRadius() : maxBound - b.getRadius();
		nPos.x = newX;
		b.velocity.x *= -1;
	}
	if (checkBoundsCollisionY(nPos.y, panel, b.getRadius()))
	{
		
		float minBound = panel.getPosition().y;
		float maxBound = (panel.getPosition().y + panel.getLocalBounds().height);
		float newY = (nPos.y < minBound) ? minBound + b.getRadius() : maxBound - b.getRadius();
		nPos.y = newY; 
		b.velocity.y *= -1;
	}

	printf("new Pos: (%f,%f) \n", nPos.x, nPos.y);
	b.setPosition(nPos);
}

void calcCol(Ball& b1, Ball& b2)
{
	float b1Mass = b1.getMass();
	float b2Mass = b2.getMass();
	std::valarray<float> b1Vel = { b1.velocity.x, b1.velocity.y };
	std::valarray<float> b2Vel = { b2.velocity.x, b2.velocity.y };
	std::valarray<float>b1Pos = { b1.getPosition().x, b1.getPosition().y };
	std::valarray<float>b2Pos = { b2.getPosition().x, b2.getPosition().y };


	auto elasticColVel = [](std::valarray<float> v1, std::valarray<float> v2, float m1, float m2, std::valarray<float> c1,
		std::valarray<float> c2) -> std::valarray<float>
	{
		auto dot = [](std::valarray<float> a, std::valarray<float> b) -> float
		{
			printf("%f\n", (a[0] * b[0]) + (a[1] * b[1]));
			return (a[0] * b[0]) + (a[1] * b[1]);
		};
		
		auto vectorNorm = [](std::valarray<float> a) -> float
		{
			printf("%f\n", pow(sqrt(pow(a[0], 2) + pow(a[1], 2)), 2));
			return pow(sqrt(pow(a[0], 2) + pow(a[1], 2)), 2);
		};
		
		return v1 - (2 * m2 / m1 + m2) *
			(dot(v1 - v2, c1 - c2) / vectorNorm(c1 - c2));
	};

	
	std::valarray<float> r1 = elasticColVel(b1Vel, b2Vel, b1Mass, b2Mass, b1Pos, b2Pos);
	std::valarray<float> r2 = elasticColVel(b2Vel, b1Vel, b2Mass, b1Mass, b2Pos, b1Pos);


	b1.velocity.x = (r1[0] > 100) ? 100 : r1[0];
	b1.velocity.y = (r1[1] > 100) ? 100 : r1[1];
	b2.velocity.x = (r1[0] > 100) ? 100 : r2[0];
	b2.velocity.y = (r1[1] > 100) ? 100 : r2[1];

	printf("newX1: %f\n", r1[0]);
	printf("newY1: %f\n", r1[1]);
	printf("newX2: %f\n", r2[0]);
	printf("newY2: %f\n", r2[1]);
}

bool checkCollision(Ball& b1, Ball& b2)
{
	float dist = calcDist(b1, b2);
	printf("dist: %f\n", dist);
	return (dist < (b1.getRadius() + b2.getRadius()));
}

void applyBallCollision(std::vector<Ball>& entityList, std::vector<Ball*>& colVector, QuadTree& q)
{
	//insert objects into quadtree entity list, get potential collisions for each entity, apply collision response towards relevant objects
	q.clear();
	for (int i = 0; i < entityList.size(); i++)
	{
		q.insert(entityList.at(i));
	}
 
	for (int i = 0; i < entityList.size(); i++)
	{
		colVector.clear();
		colVector = q.getPotentialCollisions(entityList[i], colVector);
		printf("colVector size: %i\n", colVector.size());
		if (colVector.size() > 0)
		{
			int j = 0;
			while (j < colVector.size())
			{
				if (checkCollision(entityList[i], *colVector[j]))
				{
					
					printf("collision detected\n");
					calcCol(entityList[i], *colVector[j]);
				}
				j++;
			}
		}
	}
}

void applyBallPhysics(sf::RectangleShape& panel, std::vector<Ball>& eList, std::vector<Ball*> cVect, QuadTree &cFinder, float dT)
{
	//handle ball physics
	for (Ball& b : eList)
	{
		applyBallCollision(eList, cVect, cFinder);
		calcPos(b, panel, dT);
	}

}

void highlightButton(SUI::Button& b, sf::RenderWindow& win)
{
	//highlight button if mouse is on button
	if (b.isHovered(win))
	{
		b.setColor(sf::Color::White);
	}
	else
	{
		b.setColor(sf::Color::Blue);
	}
	
}

bool mouseInPanel(sf::RectangleShape& panel, sf::RenderWindow& window)
{
	//Check if mouse is inside panel
	float mPosX = sf::Mouse::getPosition(window).x;
	float mPosY = sf::Mouse::getPosition(window).y;
	float panX = panel.getPosition().x;
	float panY = panel.getPosition().y;

	if (mPosX > panX && mPosX <= panX + panel.getLocalBounds().width
		&& mPosY > panY && mPosY <= panY + panel.getLocalBounds().height)
	{
		return true;
	}
	return false;
}

int main()
{
	//Var
	const unsigned int FPS = 30;
	enum State {IDLE, ADD};
	State currentState = IDLE;
	const int entityLimit = 50;
	std::vector<Ball> entityList;
	sf::Color colors[] = { sf::Color::Blue, sf::Color::Red, sf::Color::Green };
	float deltaTime = 1/(float)FPS;
	std::vector<Ball*> collisionVector = {};

	//Font
	sf::Font sans;
	sans.loadFromFile("OpenSans.ttf");
	
	//Window
	sf::RenderWindow window(sf::VideoMode(1200, 600), "Ball Physics", sf::Style::Close);
	window.setFramerateLimit(FPS);

	//Window Elements
	sf::RectangleShape MainPanel(sf::Vector2f(1200.f, 600.f));
	MainPanel.setFillColor(sf::Color::Black);
	sf::RectangleShape SimPanel(sf::Vector2f(800.f, 600.f));
	
	SUI::Button addBall(sf::Vector2f(200.f, 100.f),sf::Vector2f(900.f, 50.f));
	addBall.setColor(sf::Color::Blue);
	addBall.setFont(sans);
	addBall.setTextString("Add");
	addBall.setTextColor(sf::Color::Black);

	SUI::Button clearPanel(sf::Vector2f(200.f, 100.f), sf::Vector2f(900.f, 300.f));
	clearPanel.setColor(sf::Color::Blue);
	clearPanel.setFont(sans);
	clearPanel.setTextString("Clear");
	clearPanel.setTextColor(sf::Color::Black);

	sf::RectangleShape r(sf::Vector2f(SimPanel.getSize().x, SimPanel.getSize().y));
	QuadTree collisionFinder(1, r);
	
	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				//window activity
				case sf::Event::Closed:
					window.close();
					break;
				
				case sf::Event::MouseMoved:
					highlightButton(addBall, window);
					highlightButton(clearPanel, window);
					break;
				case sf::Event::MouseButtonPressed:
					if (addBall.isHovered(window))
					{
						currentState = (currentState == IDLE) ? ADD : IDLE;
						std::string text = (addBall.getTextString() == "Add") ? "Cancel" : "Add";
						addBall.setTextString(text);
					}
					else if (clearPanel.isHovered(window))
					{
						entityList.clear();
					}
					else if(mouseInPanel(SimPanel, window))
					{
						//add ball object to mouse position
						if (currentState == ADD)
						{
							if (entityList.size() < entityLimit)
							{
								entityList.push_back(Ball(10.f, sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y), colors[std::rand() % 3]));
							}
						}
					}
					break;
			}
		}
		//draw entities
		window.clear();
		window.draw(MainPanel);
		window.draw(SimPanel);
		addBall.drawTo(window);
		clearPanel.drawTo(window);
		applyBallPhysics(SimPanel, entityList, collisionVector, collisionFinder, deltaTime);
		collisionFinder.drawBounds(window);
		for (Ball& b : entityList)
		{
			b.drawTo(window);
		}
		window.display();
		printf("done\n");
	}
}