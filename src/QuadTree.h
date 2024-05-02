#pragma once
#include <vector>
#include "Ball.h"
#include <SFML/Graphics.hpp>
class QuadTree
{
private:
	//vars
	const int maxEntities = 3;
	const int maxLevels = 4;
	int level;
	std::vector<Ball*> eList;
	std::vector<QuadTree> nodeList;
	sf::RectangleShape bounds;
	
	//functions
	int getIndex(Ball& b);
	void split();
	
public:
	//functions
	QuadTree(int currentLevel, sf::RectangleShape& boundsInput);
	void insert(Ball& b);
	void clear();
	std::vector<Ball*> getPotentialCollisions(Ball& b, std::vector<Ball*> &returnlist);
	
	//for drawing. May Delete Later
	sf::RectangleShape getBounds();
	void drawBounds(sf::RenderWindow& panel);
};

