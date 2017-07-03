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
	std::string shooting;
	int deletingBullet;
};
class GameObject : public sf::Transformable
{
public:
	GameObject() {}
	~GameObject() {}
	void update(DeltaTime time) {}
	void draw(sf::RenderWindow &window) {}
	virtual sf::FloatRect getBoundingBox() = 0;
	sf::Vector2f position;
	sf::Vector2f rotation;
	sf::Vector2f scale;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::FloatRect BoundingBox;
	packetData data;
	std::string name;
	std::string shooting;
	int deletingBullet = -1;
	char type;
	virtual void destroy() {}	

private:

};

sf::Packet& operator <<(sf::Packet& packet, const packetData& data)
{
	return packet 
		<< data.name 
		<< data.position.x 
		<< data.position.y 
		<< data.rotation 
		<< data.scale.x 
		<< data.scale.y 
		<< data.shooting 
		<< data.deletingBullet;
}
sf::Packet& operator >> (sf::Packet& packet, packetData& data)
{
	return packet 
		>> data.name 
		>> data.position.x 
		>> data.position.y 
		>> data.rotation 
		>> data.scale.x 
		>> data.scale.y 
		>> data.shooting 
		>> data.deletingBullet;
}
