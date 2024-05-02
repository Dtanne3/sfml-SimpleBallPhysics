#include <vector>
#include <SFML/Graphics.hpp>
#include "QuadTree.h"

QuadTree::QuadTree(int currentLevel, sf::RectangleShape& boundsInput)
{
	level = currentLevel;
	bounds = boundsInput;
	bounds.setOutlineColor(sf::Color::Red);
	bounds.setOutlineThickness(2.f);
}
void QuadTree::clear()
{
	eList.clear();
	for (QuadTree& node : nodeList)
	{
		node.clear();
	}
	nodeList.clear();
}
void QuadTree::split()
{
	float subWidth = bounds.getLocalBounds().width / 2;
	float subHeight = bounds.getLocalBounds().height / 2;
	float x = bounds.getPosition().x;
	float y = bounds.getPosition().y;

	sf::Vector2f loc[4] = { sf::Vector2f(x + subWidth, y), sf::Vector2f(x,y),
		sf::Vector2f(x, y + subHeight), sf::Vector2f(x + subWidth, y + subHeight) };
	for (int i = 3; i >= 0; i--)
	{
		sf::RectangleShape r(sf::Vector2f(subWidth, subHeight));
		r.setPosition(loc[i]);
		QuadTree q(level + 1, r);
		nodeList.push_back(q);
	}
}
int QuadTree::getIndex(Ball& b)
{
	int index = -1; //return this if object can't fit in any quadrant
	float midpointV = bounds.getPosition().x + (bounds.getLocalBounds().width / 2);
	float midpointH = bounds.getPosition().y + (bounds.getLocalBounds().height / 2);

	
	bool bottomQuad = (b.getPosition().y < midpointH && b.getPosition().y + b.getRadius() < midpointH);
	bool topQuad = (b.getPosition().y > midpointH && b.getPosition().y + b.getRadius() > midpointH);
	
	bool leftQuad = (b.getPosition().x < midpointV && b.getPosition().x + b.getRadius() < midpointV);
	bool rightQuad = (b.getPosition().x > midpointV);

	if (rightQuad)
	{
		if (topQuad)
		{
			index = 0;
		}
		else if (bottomQuad)
		{
			index = 3;
		}
	}
	else if (leftQuad)
	{
		if (topQuad)
		{
			index = 1;
		}
		else if (bottomQuad)
		{
			index = 2;
		}
	}
	return index;
}
void QuadTree::insert(Ball& b)
{
	if (nodeList.size() > 0)
	{
		int index = getIndex(b);
		if (index != -1)
		{
			nodeList[index].insert(b);
			return;
		}
	}
	
	eList.push_back(&b);
	if (eList.size() > maxEntities && level < maxLevels)
	{
		if (nodeList.size() == 0)
		{
			split();
		}
		int i = 0;
		while (i < eList.size())
		{
			int index = getIndex(*eList[i]);
			if (index != -1)
			{
				nodeList[index].insert(*eList[i]);
				eList.erase(eList.begin() + i);
			}
			else
			{
				i++;
			}
		}
	}
}
std::vector<Ball*> QuadTree::getPotentialCollisions(Ball& b, std::vector<Ball*>& returnList)
{
	//find where the object is on the tree, then return list of objects in the same node
	int index = getIndex(b);
	if (index != -1 && nodeList.size() > 0)
	{
		nodeList[index].getPotentialCollisions(b, returnList);
	}
	
	for (int i = 0; i < eList.size(); i++)
	{
		//eList[i]->getPosition().x != b.getPosition().x && eList[i]->getPosition().y != b.getPosition().y
		if (&b != eList[i])
		{
			returnList.push_back(eList[i]);
		}
	}
	return returnList;
}
void QuadTree::drawBounds(sf::RenderWindow& panel)
{
	if(nodeList.size() > 0)
	{
		for (QuadTree node : nodeList)
		{
			node.drawBounds(panel);
		}
	}
	else
	{
		panel.draw(bounds);
	}
}
