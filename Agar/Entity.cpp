#include "Entity.h"
#include <vector>
#include <iostream>
#include <math.h>
#include "Global.h"
#include <SFML/Graphics.hpp>



using namespace std;


Entity::Entity(int size) :   m_size(size), m_speed(0), m_speedBonus(0), m_timeBonus(0)
{
    setSize(size);
}

Entity::~Entity()
{
}


sf::Vector2f Entity::getCenter()
{
	//sf::Vector2f center= this->getPosition() + sf::Vector2f(m_size, m_size);
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
	setOutlineThickness(fmin(m_size / 10,5));
};

sf::Vector2f Entity::move(sf::Vector2f target)
{	//On pourrait globalement utiliser des vecteurs partout ici

	sf::Vector2f relativeCentertoTarget(target - getCenter());
	sf::Vector2f move = sf::Vector2f(0, 0);

	if ((fabs(relativeCentertoTarget.x)>3 || fabs(relativeCentertoTarget.y)>3))
	{
		float c = sqrt((m_speed*m_speed) / (relativeCentertoTarget.x*relativeCentertoTarget.x + relativeCentertoTarget.y*relativeCentertoTarget.y));
		move.x = c*relativeCentertoTarget.x;
		move.y = c*relativeCentertoTarget.y;
		
		if (m_position.x <0)
		{
			move.x = fmax(move.x, 0.0);
		}
		else if (m_position.x >WORLD_SIZE)
		{
			move.x = fmin(move.x, 0);
		}
		if (m_position.y <0)
		{
			move.y = fmax(move.y, 0.0);
		}
		else if (m_position.y >WORLD_SIZE)
		{
			move.y = fmin(move.y, 0);
		}
	}
	return move;
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


void Entity::setCenter(sf::Vector2f position)
{
	m_position = position;
	setPosition(m_position - sf::Vector2f(m_size, m_size));
}