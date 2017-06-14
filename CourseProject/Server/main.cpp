#include <SFML/Graphics.hpp>
#include <iostream>
#include "Time.h"
#include "Game.h"
#include "StateMachine.h"


StateMachine *sm = sm->getInstance();

int main()
{
	DeltaTime time;
	sf::RenderWindow window(sf::VideoMode(600, 600), "Da Geimu");
	sm->PushState(new Game);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		sm->Update(time);
		sm->Draw(window);
		window.display();
		
	}

	return 0;
}