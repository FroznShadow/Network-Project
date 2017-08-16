#pragma once
#include "SFML\Network.hpp"
#include <iostream>

class Network
{
public:
	Network() {}
	~Network() {}

	void init()
	{
		status = socket.connect(sf::IpAddress(172,31,16,158), 35350);
		if (status != sf::Socket::Done)
		{
			std::cout << "mistakeeeeeee";
		}
		else std::cout << "Connected to port" << std::endl;
	}

	void sendData(std::string s)
	{
		packet.clear();
		if (packet << s) 
		{
			sf::Socket::Status stat;
			status = socket.send(packet);
			if (status == sf::Socket::Done)
			{
				//std::cout << "Done Sending Data " << std::endl;
			}
			while (status == sf::Socket::Partial)
			{
				status = socket.send(packet);
			}
			if (status == sf::Socket::Error)
			{
				std::cout << "Error Sending Data " << std::endl;
			}
			if (status == sf::Socket::Disconnected)
			{
				std::cout << "Disconnecter Sending Data " << std::endl;
			}
			
		}
		else std::cout << "error";

	}
	void receiveData()
	{
		//std::cout << "Message received" << std::endl;
		receive.clear();
		if (socket.receive(receive) == sf::Socket::Done)
		{

			//std::cout << "Received packet " << "Size " << receive.getDataSize() << std::endl;
			/*packetData data;
			receive >> data;
			std::cout << "Position X: " << data.position.x << " Position Y: " << data.position.y;*/
		}
		else std::cout << "nothing to get";
	}	
	sf::Packet receive;
private:
	sf::Socket::Status status;
	sf::TcpSocket socket;
	sf::Packet packet;

	//char data[100] = {};
};