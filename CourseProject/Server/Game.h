#pragma once
#include "StateMachine.h"
#include "Level.h"
#include "Player.h"
#include "Network.h"
class Game : public State
{
public:
	Game() {}
	~Game() {}
	virtual void Cleanup() override {}
	virtual void Pause() override {}
	virtual void Resume() override {}
	virtual void Init() override 
	{
		network.init();
		level.init();
		player1 = new Player;
		player2 = new Player;
		player1->init(20.f,sf::Color::Blue,sf::Vector2f(50,50));
		player2->init(20.0f,sf::Color::Red,sf::Vector2f(500,500));
		wallList = level.getWallList();
		player1_bulletList = player1->getBulletList();
		player2_bulletList = player1->getBulletList();
		player1->setData("Player1", player1->getPosition(), player1->getRotation(), player1->getScale(), "", -1);
		player2->setData("Player2", player2->getPosition(), player2->getRotation(), player2->getScale(), "", -1);

	}
	void Update(DeltaTime time)
	{
		network.listen();
		level.update(time);
		player1->moving(network.receive1);
		player2->moving(network.receive2);
		player1->update(time);
		player2->update(time);
		player1->checkCollision(player2);
		player2->checkCollision(player1);
		player1_bulletList = player1->getBulletList();
		player2_bulletList = player2->getBulletList();
		for (int i = 0; i < wallList.size(); i++)
		{
			player1->checkCollision(wallList.at(i));
			player2->checkCollision(wallList.at(i));
		}
		for (int i = 0; i < player1_bulletList.size(); i++)
		{
			player1_bulletList.at(i)->getBoundingBox();
			player2->checkCollision(player1_bulletList.at(i));
		}
		for (int i = 0; i < player2_bulletList.size(); i++)
		{
			player2_bulletList.at(i)->getBoundingBox();
			player1->checkCollision(player2_bulletList.at(i));
		}


		if (player1->getDead()==true)
		{
			player2->increaseScore();
			player1->setnotDead();
			player1->setPlayerPosition(sf::Vector2f(20, 20));
			player1->setFullHealth(); 
		}
		if (player2->getDead() == true)
		{
			player1->increaseScore();
			player2->setnotDead();
			player2->setPlayerPosition(sf::Vector2f(540, 540));
			player2->setFullHealth();
		}
		sendData();
		network.receive1.clear();
		network.receive2.clear();
		
	}
	void sendData()
	{
		player1->setData(player1->name,player1->getPosition(),player1->getRotation(),player1->getScale(),player1->getShoot());
		player2->setData(player2->name, player2->getPosition(), player2->getRotation(), player2->getScale(), player2->getShoot());
		network.sending << player1->getData() << player2->getData();
		network.sendData(network.sending);
		player1->shooting = "";
		player2->shooting = "";
		player1->deletingBullet = -1;
		player2->deletingBullet = -1;
	}
	void receiveData()
	{

	}
	void Draw(sf::RenderWindow &window)
	{
		level.draw(window);
		player1->draw(window);
		player2->draw(window);
	}

private:
	
	Level level;
	Player* player1;
	Player* player2;
	std::vector<Wall*> wallList;
	std::vector<Bullet*> player1_bulletList;
	std::vector<Bullet*> player2_bulletList;
	Network network;
	//packetData data;
	//char data[100];
};

