#pragma once
#include "StateMachine.h"
#include "Draw.h"
class MainMenu : public State
{
public:
	MainMenu(){}
	~MainMenu(){}


	void Init()
	{
		PlayButton.setPosition(sf::Vector2f(250, 150));
		PlayButton.setSize(sf::Vector2f(100, 50));
		PlayButton.setFillColor(sf::Color::Blue);
		QuitButton.setPosition(sf::Vector2f(250, 350));
		QuitButton.setSize(sf::Vector2f(100, 50));
		QuitButton.setFillColor(sf::Color::Red);

	}
	void Cleanup()
	{

	}
	void Pause()
	{

	}
	void Resume()
	{

	}
	void Update(DeltaTime time,sf::RenderWindow &window)
	{
		
		if (
			(sf::Mouse::getPosition(window).x < PlayButton.getPosition().x + PlayButton.getSize().x)
			&& (PlayButton.getPosition().x < sf::Mouse::getPosition(window).x)
			&& (sf::Mouse::getPosition(window).y < PlayButton.getPosition().y + PlayButton.getSize().y)
			&& (PlayButton.getPosition().y < sf::Mouse::getPosition(window).y)
			&& (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			)
		{
			changeState = true;
		}
		else if (
			(sf::Mouse::getPosition(window).x < QuitButton.getPosition().x + QuitButton.getSize().x)
			&& (QuitButton.getPosition().x < sf::Mouse::getPosition(window).x)
			&& (sf::Mouse::getPosition(window).y < QuitButton.getPosition().y + QuitButton.getSize().y)
			&& (QuitButton.getPosition().y < sf::Mouse::getPosition(window).y)
			&& (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			)
		{
			window.close();
		}
	}
	void Draw(sf::RenderWindow &window)
	{
		window.draw(PlayButton);
		window.draw(QuitButton);
	}
	bool ChangeState()
	{
		if (changeState == true)
			return true;
		return false;
	}
private:
	sf::RectangleShape PlayButton;
	sf::RectangleShape QuitButton;
	bool changeState = false;
};	
