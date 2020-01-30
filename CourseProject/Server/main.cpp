#include <SFML/Graphics.hpp>
#include <iostream>
#include "Time.h"
#include "Game.h"
#include "StateMachine.h"


StateMachine *sm = sm->getInstance();

int main()
{
	DeltaTime time;
	sf::String playerInput;
	sf::Text playerText;

	sf::RenderWindow window(sf::VideoMode(600, 600), "Da Geimu");
	Game* game = new Game;
	sm->PushState(game);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				game->SaveScore();
				window.close();
			}
			if (event.type == sf::Event::TextEntered)
			{
				playerInput += event.text.unicode;
				playerText.setString(playerInput);
			}
		}
		window.clear();
		sm->Update(time);
		sm->Draw(window);
		window.display();
	}

	return 0;
}