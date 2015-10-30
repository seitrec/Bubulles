#pragma once
#include <SFML/Graphics.hpp>

class Entity :
	public sf::CircleShape
{
public:
	Entity();
	~Entity();
	sf::Vector2f getCenter();
	void setCenter(sf::Vector2f position);
	float getSize();
	void setSize(float size);
	sf::Vector2f move(sf::Vector2f target);
	sf::Color getColor();
	void setColor(sf::Color);
	void draw(sf::RenderWindow * ptrWindow);
	virtual void getEaten(Entity& predator);

protected:
	sf::Vector2f m_position;
	float m_size;
	sf::Color m_color;
	float m_speed;
	float m_speedBonus;
	float m_timeBonus;
};

