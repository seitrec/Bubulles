#include "Entity.h"
#include <vector>
#include <iostream>
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
};

void Entity::move(sf::Vector2f target)
{	//On pourrait globalement utiliser des vecteurs partout ici

	 sf::Vector2f relativeCentertoTarget((target.x) - (this->getCenter().x),
										(target.y) - (this->getCenter().y));

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
		std::cout << target.x << std::endl;
		std::cout << target.y << std::endl;
		std::cout << m_position.x << std::endl;
		std::cout << m_position.y << std::endl;
		std::cout << moveX << std::endl;
		std::cout << moveY << std::endl;

		setCenter(sf::Vector2f(m_position.x+moveX, m_position.y+moveY));
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


void Entity::setCenter(sf::Vector2f position)
{
	m_position = position;
	setPosition(m_position - sf::Vector2f(m_size, m_size));
};