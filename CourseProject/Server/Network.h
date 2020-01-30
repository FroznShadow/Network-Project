#pragma once
#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include <list>
#include "SFML\Network.hpp"
#include <ctime>



class Network
{
public:
	Network() {}
	~Network() {}

	void init()
	{
		
		listener.setBlocking(false);
		status = listener.listen(35350);
		if (status != sf::Socket::Done)
		{
			std::cout << status << std::endl;
		}
		else std::cout <<  "listening to a port " << 35350 << " on ip address " << sf::IpAddress::getLocalAddress() << std::endl;

		selector.add(listener);
		migrate();

	}
	void migrate()
	{
		try {
			pqxx::connection C("dbname=postgres user=postgres password=asd123 hostaddr=127.0.0.1 port=5432");
			if (C.is_open()) {
				std::cout << "Opened database successfully: " << C.dbname() << std::endl;
				
				char* sql = "SELECT EXISTS (SELECT 1 FROM   information_schema.tables WHERE  table_schema = 'public' AND    table_name = 'migrations');";
				pqxx::nontransaction N(C);
				pqxx::row r = N.exec1(sql);
				N.commit();
				std::string table_exists = r[0].as<std::string>();
				if (table_exists == "f")
				{
					sql = "CREATE TABLE migrations(ID SERIAL, name TEXT NOT NULL)";

					pqxx::work W1(C);
					W1.exec(sql);
					W1.commit();
					std::cout << "Migrations table created successfully" << std::endl;
					sql = "INSERT INTO migrations(NAME) VALUES('add_migrations_table')";
					pqxx::work W2(C);
					W2.exec(sql);
					W2.commit();
					std::cout << "Migrations table updated successfully" << std::endl;
				}
				sql = "SELECT EXISTS (SELECT 1 FROM   information_schema.tables WHERE  table_schema = 'public' AND    table_name = 'users');";
				pqxx::nontransaction N2(C);
				r = N2.exec1(sql);
				N2.commit();
				table_exists = r[0].as<std::string>();
				if (table_exists == "f")
				{

					pqxx::work W3(C);
					sql = "CREATE TABLE USERS(ID SERIAL, username TEXT NOT NULL, password TEXT NOT NULL)";
					W3.exec(sql);
					W3.commit();
					std::cout << "Users table created successfully" << std::endl;
					sql = "INSERT INTO migrations(NAME) VALUES('add_users_table')";
					pqxx::work W4(C);
					W4.exec(sql);
					W4.commit();
					std::cout << "Migrations table updated successfully" << std::endl;
				}
				C.disconnect();
				//register_user("asd", "123");
				std::string username = login("asd", "1223");
				std::cout << "Username: " << username << std::endl;
			}
			else {
				std::cout << "Can't open database" << std::endl;
			}
		}
		catch (const std::exception & e) {
			std::cerr << e.what() << std::endl;
		}
	}
	std::string login(std::string username, std::string password)
	{

		std::string user = "";
		pqxx::connection C("dbname=postgres user=postgres password=asd123 hostaddr=127.0.0.1 port=5432");

		pqxx::work W(C);
		std::string sql = "SELECT username FROM USERS WHERE username = $1 AND password = $2";
		try {
			pqxx::result res = W.exec_params(sql, username, password);

			W.commit();
			C.disconnect();
			if (res.empty())
			{
				std::cout << "No user found" << std::endl;
				user = "Player " + std::to_string(++players);
				return user;
			}
			user = res[0]["username"].as<std::string>();
			return user;
		}
		catch( const std::exception & e) {
			std::cerr << e.what() << std::endl;
			return "";
		}
		

	}
	void register_user(std::string username, std::string password)
	{

		std::string user;
		pqxx::connection C("dbname=postgres user=postgres password=asd123 hostaddr=127.0.0.1 port=5432");

		pqxx::work W(C);
		std::string sql = "INSERT INTO users(username, password) VALUES("+W.quote(username)+","+W.quote(password)+ ")";
		try {
			pqxx::result res = W.exec(sql);

			W.commit();
			C.disconnect();
			std::cout << "User inserted into users table" << std::endl;
			if (res.empty())
			{
				std::cout << "No user found" << std::endl;
				return;
			}
			user = res[0]["username"].as<std::string>();
			std::cout << "username: " << user << std::endl;
			return;
		}
		catch (const std::exception & e) {
			std::cerr << e.what() << std::endl;
			return;
		}
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
	void sendScore(int score, int user_id)
	{

		sf::Http::Request request("/score", sf::Http::Request::Post);

		std::ostringstream stream;
		stream << "user_id=" << user_id << "&score=" << score;
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
				std::cout << "Socket " << it->first << it->second << " Disconnected " << std::endl;
				sockets.erase(it);
				
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
	sf::Socket::Status status;
};
	