#pragma once
#include "gameObject.h"
#include <vector>
class Bullet : public GameObject
{
public:

	Bullet() {}
	~Bullet() {}

	void init(float radius, sf::Color fillColor, sf::Vector2f position)
	{
		type = 'B';
		bullet.setRadius(radius);
		bullet.setFillColor(fillColor);
		bullet.setPosition(position);
	}
	void deleteBullet()
	{
		delete this;
	}
	void draw(sf::RenderWindow &window) { window.draw(bullet); }
	void move()
	{
		if (dir == "W")
		{
			bullet.setPosition(bullet.getPosition().x, bullet.getPosition().y - 0.1);
		}
		else if (dir == "S")
		{
			bullet.setPosition(bullet.getPosition().x, bullet.getPosition().y + 0.1);
		}
		else if (dir == "A")
		{
			bullet.setPosition(bullet.getPosition().x - 0.1, bullet.getPosition().y);
		}
		else if (dir == "D")
		{
			bullet.setPosition(bullet.getPosition().x + 0.1, bullet.getPosition().y);
		}
	}	
	std::string dir;
private:
	sf::CircleShape bullet;

};

class Player : public GameObject
{
public:
	Player() { health = 100; damage = 10; }
	~Player()
	{
		bulletList.clear();
	}
	void draw(sf::RenderWindow &window)
	{
		window.draw(circle);
		for (int i = 0; i < bulletList.size(); i++)
		{
			bulletList.at(i)->move();
			bulletList.at(i)->draw(window);

		}
	}
	void init(float radius, sf::Color fillColor, sf::Vector2f position)
	{
		playerColor = fillColor;
		setPosition(position);
		circle.setOrigin(radius, radius);
		circle.setRadius(radius);
		circle.setFillColor(fillColor);
		circle.setPosition(getPosition());
	}
	void setData(std::string name, sf::Vector2f pos, float rot = 0, sf::Vector2f scale = sf::Vector2f(1, 1), std::string shoot = "", int bullet = -1)
	{
		if (name != "")data.name = name;
		data.position = pos;
		data.rotation = rot;
		if (scale != sf::Vector2f(0, 0))data.scale = scale;
		data.shoot = shoot;
		if( bullet != -1) deleteBullet(bulletList.at(bullet));
	}
	packetData getData()
	{
		return data;
	}
	void move(float x, float y)
	{
		movingDirection = sf::Vector2f(x, y);
		setPosition(circle.getPosition().x + x, circle.getPosition().y + y);
		circle.setPosition(getPosition());
	}
	void setPlayerPosition(sf::Vector2f pos) {
		setPosition(pos);
		circle.setPosition(getPosition());
	}
	void shoot(std::string dir)
	{
		Bullet *bullet = new Bullet;
		bullet->dir = dir;
		bullet->init(10.f, playerColor, circle.getPosition());
		bulletList.push_back(bullet);
		data.shoot = "";
	}
	void deleteBullet(Bullet* bullet)
	{
		bullet->deleteBullet();
	}
	std::vector<Bullet*> getBulletList()
	{
		return bulletList;
	}
private:
	int health;
	int damage;
	int score;
	sf::Vector2f movingDirection;
	bool controlledPlayer = false;
	sf::Color playerColor;
	sf::CircleShape circle; // for testing
	std::vector<Bullet*> bulletList;
	float shoottimer;
	bool dead;
};

