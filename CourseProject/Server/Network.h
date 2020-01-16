#pragma once
#include "SFML\Network.hpp"
#include <iostream>
#include <list>
class Network
{
public:
	Network() {}
	~Network() {}
	int port = 35350;

	void init()
	{
		
		listener.setBlocking(false);
		if (listener.listen(port) != sf::Socket::Done)
		{
			std::cout << "Error 1";
		}
		else std::cout <<  "listening to a port: " << port << " on ip address: " << sf::IpAddress::getPublicAddress() << std::endl;

		selector.add(listener);
	}
	void listen()
	{

		if (selector.wait(sf::microseconds(1)))
		{
			if (selector.isReady(listener))
			{

				std::cout << "connection call received" << std::endl;
				sf::TcpSocket* socket = new sf::TcpSocket;
				
				socket->setBlocking(false);
				if (listener.accept(*socket) == sf::Socket::Done)
				{
			
					sockets[++players] = socket;
					//sockets.push_back(socket);
					selector.add(*socket);
					std::cout << "socket accepted";
				}
				else delete socket;
			}
			else {
				if(sockets.begin() != sockets.end()) for (std::map<int, sf::TcpSocket*>::iterator it = sockets.begin(); it != sockets.end(); it++)
				{
					sf::TcpSocket& socket = *it->second;
					if (selector.isReady(socket))
					{
						//std::cout << "Message received" << std::endl;
						if (it->first == 1)
						{
							receive1.clear();
							if (socket.receive(receive1) == sf::Socket::Done)
							{
								//std::cout << "Received packet " << "Size "<< receive1.getDataSize() << std::endl;
							}
							//else //std::cout << "nothing to get";
						}
						else if (it->first == 2)
						{
							receive2.clear();
							if (socket.receive(receive2) == sf::Socket::Done)
							{
								//std::cout << "Received packet " << "Size "<< receive2.getDataSize() << std::endl;
							}
							//else //std::cout << "nothing to get";
						}
					}
					//else //std::cout << "not ready";
				}
			}
		}
	}

	void sendData(sf::Packet send)
	{
		if(sockets.begin() != sockets.end())for (std::map<int, sf::TcpSocket*>::iterator it = sockets.begin(); it != sockets.end(); it++)
		{
			sf::Socket::Status status;
			sf::TcpSocket& socket = *it->second;
			socket.setBlocking(false);
			status = socket.send(send);

			if (status == sf::Socket::Done)
			{
				//std::cout << "Done Sending Data " << std::endl;
			}
			while (status == sf::Socket::Partial)
			{
				status = socket.send(send);
			}
			if (status == sf::Socket::Error)
			{
				std::cout << "Error Sending Data " << std::endl;
			}
			if (status == sf::Socket::Disconnected)
			{
				std::cout << "Disconnecter Sending Data " << std::endl;
				it = sockets.erase(it);
			}
		}
		sending.clear();
	}
sf::Packet receive1;
sf::Packet receive2;
sf::Packet sending;
std::map<int, sf::TcpSocket*> sockets;
private:
	sf::Thread* thread = 0;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::size_t received;
	int players = 0;
};
	