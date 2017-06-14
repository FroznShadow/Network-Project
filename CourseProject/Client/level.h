#pragma once

#include "GameObject.h"
#include <vector>
class Wall : public GameObject

{
public:
	Wall() { }
	~Wall() {}
	virtual void draw(sf::RenderWindow &window)
	{
		window.draw(rect);
	}
	sf::RectangleShape rect; // for testing
	int id;

private:
};

class Level
{
public:
	Level() {}
	~Level() {}
	void init()
	{
		addWall(sf::Vector2f(0, 0), float(0), sf::Vector2f(10, 600));
		addWall(sf::Vector2f(0, 0), float(0), sf::Vector2f(600, 10));
		addWall(sf::Vector2f(590, 0), float(0), sf::Vector2f(10, 600));
		addWall(sf::Vector2f(0, 590), float(0), sf::Vector2f(600, 10));
		addWall(sf::Vector2f(300, 0), float(0), sf::Vector2f(10, 200));
		addWall(sf::Vector2f(300, 400), float(0), sf::Vector2f(10, 200));
		addWall(sf::Vector2f(0, 150), float(0), sf::Vector2f(150, 10));
		addWall(sf::Vector2f(0, 450), float(0), sf::Vector2f(150, 10));
		addWall(sf::Vector2f(450, 150), float(0), sf::Vector2f(150, 10));
		addWall(sf::Vector2f(450, 450), float(0), sf::Vector2f(150, 10));
		addWall(sf::Vector2f(250, 300), float(0), sf::Vector2f(100, 10));
		addWall(sf::Vector2f(100, 250), float(0), sf::Vector2f(10, 100));
		addWall(sf::Vector2f(500, 250), float(0), sf::Vector2f(10, 100));

	}
	void draw(sf::RenderWindow &window)
	{
		for (int i = 0; i < wallList.size(); i++)
		{
			wallList.at(i)->draw(window);
		}
	}
	void addWall(sf::Vector2f P, float R, sf::Vector2f S)
	{
		Wall* newWall = new Wall;
		newWall->rect.setSize(sf::Vector2f(1, 1));
		newWall->rect.setFillColor(sf::Color::Cyan);
		newWall->rect.setPosition(P);
		newWall->rect.setRotation(R);
		newWall->rect.setScale(S);
		newWall->id = ++counter;
		wallList.push_back(newWall);
		newWall->type = 'W';

	}
	std::vector<Wall*> getWallList()
	{
		return wallList;
	}
private:
	int counter;
	std::vector<Wall*> wallList;
};

