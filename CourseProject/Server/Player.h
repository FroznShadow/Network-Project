#pragma once
#include "GameObject.h"
#include <vector>
#include <SFML\Network.hpp>
class Bullet : public GameObject
{
public:
	
	Bullet() {}
	~Bullet(){}

	void init(float radius, sf::Color fillColor, sf::Vector2f position)
	{
		type = 'B';
		bullet.setRadius(radius);
		bullet.setFillColor(fillColor);
		bullet.setPosition(position);
		BoundingBox = bullet.getGlobalBounds();
	}
	void deleteBullet()
	{
		delete this;
	}
	void update(DeltaTime time)
	{
		move(direction);
		lifeTimer-=0.1;
	}
	void draw(sf::RenderWindow &window) { window.draw(bullet); }
	void move(std::string dir)
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
	sf::FloatRect getBoundingBox()
	{
		BoundingBox = bullet.getGlobalBounds();
		return BoundingBox;
	}
	void setDir(std::string dir)
	{
		direction = dir;
	}
	int getlifeTimer()
	{
		return lifeTimer;
	}
	void destroy() 
	{
		lifeTimer = 0;
	}
private:
	sf::CircleShape bullet;
	std::string direction;
	double lifeTimer = 12000;
};

class Player : public GameObject
{
public:
	Player() { health = 100; damage = 10; }
	~Player() 
	{
		std::cout << "dead" << std::endl;
		bulletList.clear();
	}

	void update(DeltaTime time) 
	{
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList.at(i)->getlifeTimer() <= 0)
			{
				deleteBullet(bulletList.at(i));
				deletingBullet = i;
				bulletList.erase(bulletList.begin() + i);
			}
		}

		for (int i = 0; i < bulletList.size(); i++)
		{
			bulletList.at(i)->update(time);
		}
		shoottimer -= 0.1f;
		if (health <= 0)
		{
			dead = true;
		}
	}
	void draw(sf::RenderWindow &window) 
	{
		window.draw(circle);
		for (int i = 0; i < bulletList.size(); i++)
		{
			bulletList.at(i)->draw(window);
		}
	}

	void init(float radius,sf::Color fillColor,sf::Vector2f position) 
	{
		playerColor = fillColor;
		setPosition(position);
		circle.setOrigin(radius, radius);
		circle.setRadius(radius);
		circle.setFillColor(fillColor);
		circle.setPosition(getPosition());
		BoundingBox = circle.getGlobalBounds();
		type = 'P';
		
		
	}
	void setData(std::string name, sf::Vector2f pos, float rot = 0, sf::Vector2f scale = sf::Vector2f(1,1), std::string shooting = "", int deleteBullet = -1)
	{
		if( name != "")data.name = name;
		data.position = pos;
		data.rotation = rot;
		if(scale != sf::Vector2f(0,0))data.scale = scale;
		data.shooting = shooting;
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList.at(i)->getlifeTimer() <= 0)
			data.deletingBullet = i;
		}
		
	}
	packetData getData()
	{
		return data;
	}
	std::string getShoot()
	{
		return shooting;
	}
	void hit()
	{
		std::cout << "getHit" << std::endl;
		health -= damage;
	}
	void setFullHealth() { health = 100; }
	void moving(sf::Packet packet)
	{
		std::string a;
		packet >> a;
		if (a.compare("W") == 0)
		{
			move(0, -0.05f);
		}
		if (a.compare("S") == 0)
		{
			move(0, 0.05f);
		}
		if (a.compare("A") == 0)
		{
			move(-0.05f, 0);
		}
		if (a.compare("D") == 0)
		{
			move(0.05f, 0);
		}

		if (a.compare("Up") == 0)
		{
			if (shoottimer <= 0)
			{
				shoot("W");
				shooting = "W";
				shoottimer = 100;
			}
		}
		if (a.compare("Down") == 0)
		{
			if (shoottimer <= 0)
			{
				shoot("S");
				shooting = "S";
				shoottimer = 100;
			}
		}
		if (a.compare("Left") == 0)
		{
			if (shoottimer <= 0)
			{
				shoot("A");
				shooting = "A";
				shoottimer = 100;
			}
		}
		if (a.compare("Right") == 0)
		{
			if (shoottimer <= 0)
			{
				shoot("D");
				shooting = "D";
				shoottimer = 100;
			}
		}

	}
	void move(float x, float y)
	{
		movingDirection = sf::Vector2f(x, -y);
		setPosition(circle.getPosition().x +x, circle.getPosition().y + y);
		circle.setPosition(getPosition());
	}
	void setPlayerPosition(sf::Vector2f pos) 
	{
		setPosition(pos);
		circle.setPosition(getPosition());
	}
	void setnotDead() { dead = false; }
	bool getDead() { return dead; }
	void shoot(std::string dir)
	{
		Bullet *bullet = new Bullet;
		bullet->setDir(dir);
		bullet->init(10.f, playerColor, circle.getPosition());
		bulletList.push_back(bullet);
	}
	void deleteBullet(Bullet* bullet)
	{
		bullet->deleteBullet();
	}
	int getDeleteBullet()
	{
		return deletingBullet;
	}
	std::vector<Bullet*> getBulletList()
	{
		return bulletList;
	}
	void checkCollision(GameObject* otherObject)
	{
		BoundingBox = circle.getGlobalBounds();
		if (BoundingBox.intersects(otherObject->getBoundingBox()))
		{
			if (otherObject->type == 'W')
			{
				if (BoundingBox.top  < otherObject->BoundingBox.top && BoundingBox.top  + BoundingBox.height > otherObject->BoundingBox.top )
				{
					move(0, -0.1f);
				}
				if (BoundingBox.top < otherObject->BoundingBox.top + otherObject->BoundingBox.height && BoundingBox.top + BoundingBox.height > otherObject->BoundingBox.top + otherObject->BoundingBox.height)
				{
					move(0, 0.1f);
				}
				if (BoundingBox.left < otherObject->BoundingBox.left + otherObject->BoundingBox.width && BoundingBox.left + BoundingBox.width > otherObject->BoundingBox.left + otherObject->BoundingBox.width)
				{
					move(0.1f, 0);
				}
				if (BoundingBox.left < otherObject->BoundingBox.left && BoundingBox.left + BoundingBox.width > otherObject->BoundingBox.left )
				{
					move(-0.1f, 0);
				}
			}
			if (otherObject->type == 'B')
			{
				hit();
				otherObject->destroy();
			}
		}
	}
	sf::FloatRect getBoundingBox()
	{
		BoundingBox = circle.getGlobalBounds();
		return BoundingBox;
	}
	void increaseScore()
	{
		std::cout << "score up" << std::endl;
		score++;
	}
	
private:
	int health;
	int damage;
	int score;
	sf::Vector2f movingDirection;
	sf::Color playerColor;
	sf::CircleShape circle; // for testing
	std::vector<Bullet*> bulletList;
	float shoottimer;
	bool dead;


};
