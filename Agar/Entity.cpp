#include "Entity.h"
#include <vector>
#include <math.h>
#include "Global.h"
#include <SFML/Graphics.hpp>


using namespace std;



Entity::Entity()
{
}

Entity::~Entity()
{
}

sf::Vector2f Entity::getPosition()
{
	return m_position;
}

void Entity::draw(sf::RenderWindow * ptrWindow)
{
	ptrWindow->draw(*this);
}


float Entity::getSize()
{
	return m_size;
};

void Entity::setSize(float size)
{
	m_size = size;
	setRadius(m_size);
	m_speed = 5;
};

void Entity::move(sf::Vector2f target)
{

	 sf::Vector2f relativeCentertoTarget((target.x) - (this->getPosition().x + m_size),
										(target.y) - (this->getPosition().y + m_size));

	float moveX = 0;
	float moveY = 0;
	if ((fabs(relativeCentertoTarget.x)>3 || fabs(relativeCentertoTarget.y)>3))
	{
		float c = sqrt((m_speed*m_speed) / (relativeCentertoTarget.x*relativeCentertoTarget.x + relativeCentertoTarget.y*relativeCentertoTarget.y));
		moveX = c*relativeCentertoTarget.x;
		moveY = c*relativeCentertoTarget.y;
		
		if (m_position.x <0)
		{
			moveX = fmax(moveX, 0.0);
		}
		else if (m_position.x >worldSize)
		{
			moveX = fmin(moveX, 0);
		}
		if (m_position.y <0)
		{
			moveY = fmax(moveY, 0.0);
		}
		else if (m_position.y >worldSize)
		{
			moveY = fmin(moveY, 0);
		}
		setPosition2(sf::Vector2f(m_position.x+moveX, m_position.y+moveY));
	}

	
};

sf::Color Entity::getColor()
{
	return m_color;
};

void Entity::setColor(sf::Color color)
{
	m_color = color;
	setFillColor(color);
};

//TO DO voir comment faire, setPosition existe d�j� dans CircleShape donc on ne peut pas le red�finir ici...
void Entity::setPosition2(sf::Vector2f position)
{
	m_position = position;
	setPosition(m_position);
};

void Entity::getEaten()
{

}
