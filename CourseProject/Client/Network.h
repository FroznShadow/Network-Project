#pragma once
#include "SFML\Network.hpp"
#include <iostream>
#include <sstream>
class Network
{
public:
	Network() {}
	~Network() {}

	void init()
	{

		std::cout << "Establishing a connection to a server on local ip address " << ipAddress << " on port 2310" << std::endl;
		status = socket.connect(ipAddress, 2310);
		if (status != sf::Socket::Done)
		{
			std::cout << "Could not connect to server" << std::endl;
		}
		else std::cout << "Connected to server" << std::endl;
	}
	std::string login(std::string username, std::string password)
	{
		return username;
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
				std::cout << "Disconnected Sending Data " << std::endl;
			}
			
		}
		//else std::cout << "error";

	}
	void disconnect()
	{
		if(socket.getRemoteAddress() != sf::IpAddress::None)
		socket.disconnect();
	}
	void getScore(std::string user_id)
	{
		sf::Http::Request request("/getScore", sf::Http::Request::Post);

		std::ostringstream stream;
		stream << "&name=" << user_id;
		request.setBody(stream.str());

		sf::Http http("http://192.168.10.50");
		sf::Http::Response response = http.sendRequest(request);

		if (response.getStatus() == sf::Http::Response::Ok)
		{
			std::cout << response.getBody() << std::endl;
		}
		else
		{
			std::cout << "Request failed" << std::endl;
		}
	}
	void setIpAddress(std::string string)
	{
		ipAddress = string;
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
		//else std::cout << "nothing to get";
	}	
	sf::Packet receive;
private:
	sf::Socket::Status status;
	sf::TcpSocket socket;
	sf::Packet packet;
	sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();

	//char data[100] = {};
};