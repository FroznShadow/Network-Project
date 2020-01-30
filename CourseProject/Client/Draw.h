#pragma once
#include "Level.h"
#include "Player.h"
#include "Network.h"
#include "StateMachine.h"
class Game : public State
{
public:
	Game() {}
	~Game() {}
	void Init()
	{
		networkInstance->init();
		level.init();
		player1 = new Player;
		player2 = new Player;
		player1->init(20.f, sf::Color::Blue, sf::Vector2f(50, 50));
		player2->init(20.0f, sf::Color::Red, sf::Vector2f(500, 500));
		wallList = level.getWallList();
		player1_bulletList = player1->getBulletList();
		player2_bulletList = player2->getBulletList();
	}

	void Draw(sf::RenderWindow &window)
	{
		level.draw(window);
		player1->draw(window);
		player2->draw(window);
	}
	void Cleanup() {}
	void Pause() {}
	void Resume() {}
	void Update(DeltaTime time, sf::RenderWindow &window)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			std::string input = "Up";
			networkInstance->sendData(input);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			std::string input = { "Down" };
			networkInstance->sendData(input);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			std::string input = { "Left" };
			networkInstance->sendData(input);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			std::string input = { "Right" };
			networkInstance->sendData(input);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			std::string input = { "W" };
			networkInstance->sendData(input);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			std::string input = { "S" };
			networkInstance->sendData(input);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			std::string input = { "A" };
			networkInstance->sendData(input);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			std::string input = { "D" };
			networkInstance->sendData(input);
		}
		packetData data,data2;
		networkInstance->receiveData();
		networkInstance->receive >> data >> data2;
		if (data.name == "Player1")
		{
			player1->setData(data.name, data.position, data.rotation, data.scale, data.shoot);
			if (data2.name == "Player2")
			{
				player2->setData(data2.name, data2.position, data2.rotation, data2.scale, data2.shoot);
			}
		}
		else if (data.name == "Player2")
		{
			player2->setData(data.name, data.position, data.rotation, data.scale, data.shoot);
			if (data2.name == "Player1")
			{
				player1->setData(data2.name, data2.position, data2.rotation, data2.scale, data2.shoot);
			}
		}
		//std::cout << "Player position X " << data.position.x << " Player position Y " << data.position.y << std::endl;
		float x, y;
		x = player1->getData().position.x - player1->getPosition().x;
		y = player1->getData().position.y - player1->getPosition().y;
		player1->move(x, y);
		if(player1->getData().shoot != "") player1->shoot(player1->getData().shoot);
		if (player2->getData().shoot != "") player2->shoot(player2->getData().shoot);
		x = player2->getData().position.x - player2->getPosition().x;
		y = player2->getData().position.y - player2->getPosition().y;
		player2->move(x, y);

	}
	void setNetwork(Network* network)
	{
		networkInstance = network;
	}

private:

	Level level;
	Player* player1;
	Player* player2;
	std::vector<Wall*> wallList;
	std::vector<Bullet*> player1_bulletList;
	std::vector<Bullet*> player2_bulletList;
	Network *networkInstance;

};

