#ifndef DEF_Entity
#define DEF_Entity

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Entity :public sf::CircleShape
{
public:
	Entity();
	~Entity();
	sf::Vector2f getCenter();
	float getSize();
	void setSize(float radius);
	void setColor(sf::Color);
	sf::Color getColor();
	Entity getClosest(std::vector<Entity> lEntity);

protected:
	float m_size;
	int m_currentX;
	int m_currentY;
	sf::Color m_color;
	int m_positionX;
	int m_positionY;
};

#endif