#pragma once
#include <SFML/Graphics.hpp>

class Entity :
	public sf::CircleShape
{
public:
    Entity (float size);
	~Entity();
	void draw(sf::RenderWindow &window);
	sf::Vector2f getCenter();
	void setCenter(sf::Vector2f position);
	sf::Color getColor();
	void setColor(sf::Color);
	float getSize();
	void setSize(float size);
    void setSpeed(float size);
	sf::Vector2f move(sf::Vector2f target);
	virtual void getEaten(Entity& predator);

protected:
	sf::Vector2f m_position;
	float m_size;
	sf::Color m_color;
	float m_speed;
	float m_speedBonus;
	float m_timeBonus;
};

