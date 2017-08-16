#include <SFML/Graphics.hpp>
#include "Draw.h"
#include "input.h"
#include "Network.h"
#include "StateMachine.h"
#include "Time.h"
#include "MainMenu.h"
int main()
{
	StateMachine* sm = sm->getInstance();
	DeltaTime time;
	MainMenu *mainMenu = new MainMenu;

	sm->PushState(mainMenu);
	sf::RenderWindow window(sf::VideoMode(600, 600), "Da Geimu");
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	
		window.clear();
		sm->Update(time,window);
		sm->Draw(window);	
		if (mainMenu->ChangeState() == true)
		{
				Game *game = new Game;
			sm->ChangeState(game);
		}
		window.display();
	}
	return 0;
}