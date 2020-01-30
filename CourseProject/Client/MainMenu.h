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
		if (!font.loadFromFile("fonts/arial.ttf"))
		{
			std::cout << "Error loading font" << std::endl; // error...
		}


		PlayButton.setPosition(sf::Vector2f(250, 100));
		PlayButton.setSize(sf::Vector2f(100, 50));
		PlayButton.setFillColor(sf::Color::Blue);

		PlayText.setPosition(PlayButton.getPosition());
		PlayText.setString("Play");
		PlayText.setFont(font);

		ServerButton.setPosition(sf::Vector2f(250, 200));
		ServerButton.setSize(sf::Vector2f(100, 50));
		ServerButton.setFillColor(sf::Color::Cyan);

		ServerText.setPosition(ServerButton.getPosition());
		ServerText.setString("Set Server IP");
		ServerText.setFont(font);

		LoginButton.setPosition(sf::Vector2f(250, 300));
		LoginButton.setSize(sf::Vector2f(100, 50));
		LoginButton.setFillColor(sf::Color::Cyan);

		LoginText.setPosition(LoginButton.getPosition());
		LoginText.setString("Login");
		LoginText.setFont(font);

		ScoreButton.setPosition(sf::Vector2f(250, 400));
		ScoreButton.setSize(sf::Vector2f(100, 50));
		ScoreButton.setFillColor(sf::Color::Cyan);

		ScoreText.setPosition(ScoreButton.getPosition());
		ScoreText.setString("Score");
		ScoreText.setFont(font);
		
		QuitButton.setPosition(sf::Vector2f(250, 500));
		QuitButton.setSize(sf::Vector2f(100, 50));
		QuitButton.setFillColor(sf::Color::Red);

		QuitText.setPosition(QuitButton.getPosition());
		QuitText.setString("Quit");
		QuitText.setFont(font);


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
		else if (
			(sf::Mouse::getPosition(window).x < ScoreButton.getPosition().x + ScoreButton.getSize().x)
			&& (ScoreButton.getPosition().x < sf::Mouse::getPosition(window).x)
			&& (sf::Mouse::getPosition(window).y < ScoreButton.getPosition().y + ScoreButton.getSize().y)
			&& (ScoreButton.getPosition().y < sf::Mouse::getPosition(window).y)
			&& (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			)
		{
			Network network;
			std::string user_id;
			std::cout << "insert username" << std::endl;
			std::cin >> user_id;
			network.getScore(user_id);

		}
		else if (
			(sf::Mouse::getPosition(window).x < ServerButton.getPosition().x + ServerButton.getSize().x)
			&& (ServerButton.getPosition().x < sf::Mouse::getPosition(window).x)
			&& (sf::Mouse::getPosition(window).y < ServerButton.getPosition().y + ServerButton.getSize().y)
			&& (ServerButton.getPosition().y < sf::Mouse::getPosition(window).y)
			&& (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			)
		{
			std::string ipAddress;
			std::cout << "insert server ip address: " << std::endl;
			std::cin >> ipAddress;
			networkInstance->setIpAddress(ipAddress);
			std::cout << "server ip address set to : " << ipAddress << std::endl;

		}
		else if (
			(sf::Mouse::getPosition(window).x < LoginButton.getPosition().x + LoginButton.getSize().x)
			&& (LoginButton.getPosition().x < sf::Mouse::getPosition(window).x)
			&& (sf::Mouse::getPosition(window).y < LoginButton.getPosition().y + LoginButton.getSize().y)
			&& (LoginButton.getPosition().y < sf::Mouse::getPosition(window).y)
			&& (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			)
		{
			std::cout << "Login to server: " << std::endl;
			std::string username;
			std::string password;
			std::cout << "type username: " << std::endl;
			std::cin >> username;
			std::cout << "type password: " << std::endl;
			std::cin >> password;

			std::string playerName = networkInstance->login(username, password);
		}
	}
	void Draw(sf::RenderWindow &window)
	{
		window.draw(PlayButton);
		window.draw(PlayText);
		window.draw(QuitButton);
		window.draw(QuitText);
		window.draw(ScoreButton);
		window.draw(ScoreText);
		window.draw(ServerButton);
		window.draw(ServerText);
		window.draw(LoginButton);
		window.draw(LoginText);
	}
	void setNetwork(Network *network)
	{
		networkInstance = network;
	}
	bool ChangeState()
	{
		if (changeState == true)
			return true;
		return false;
	}
private:
	sf::Event event;
	Network* networkInstance;
	sf::RectangleShape PlayButton;
	sf::Text PlayText;
	sf::String playerInput;
	sf::Text ipAddress;
	sf::RectangleShape QuitButton;
	sf::Text QuitText;
	sf::RectangleShape ScoreButton;
	sf::Text ScoreText;
	sf::RectangleShape ServerButton;
	sf::Text ServerText;
	sf::RectangleShape LoginButton;
	sf::Text LoginText;
	sf::Font font;
	bool changeState = false;
};	
