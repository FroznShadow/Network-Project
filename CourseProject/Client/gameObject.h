#pragma once

#include "SFML/System.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Network.hpp"
struct packetData
{
	std::string name;
	sf::Vector2f position;
	float rotation;
	sf::Vector2f scale;
	std::string shoot;
	int deleteBullet;
};

class GameObject : public sf::Transformable
{
public:
	GameObject() {}
	~GameObject() {}
	void draw(sf::RenderWindow &window) {}
	sf::Vector2f position;
	sf::Vector2f rotation;
	sf::Vector2f scale;
	sf::Sprite sprite;
	sf::Texture texture;
	char type;
	virtual void destroy() {}
	packetData data;
private:
	sf::Packet packet;

};

sf::Packet& operator <<(sf::Packet& packet, const packetData& data)
{
	return packet << data.name << data.position.x << data.position.y << data.rotation << data.scale.x << data.scale.y << data.shoot << data.deleteBullet;
}
sf::Packet& operator >> (sf::Packet& packet, packetData& data)
{
	return packet >> data.name >> data.position.x >> data.position.y >> data.rotation >> data.scale.x >> data.scale.y >> data.shoot >> data.deleteBullet;
}
