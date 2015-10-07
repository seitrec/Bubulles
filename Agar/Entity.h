#ifndef DEF_Entity
#define DEF_Entity

#include <string>
#include <SFML/Graphics.hpp>

class Entity :public sf::CircleShape
{
public:
	Entity();
	~Entity();
	sf::Vector2f getCenter();
	int getSize();
	void setSize(float radius);

protected:
	float m_size;
	int m_currentX;
	int m_currentY;
	std::string m_color;
	int m_positionX;
	int m_positionY;
};

#endif