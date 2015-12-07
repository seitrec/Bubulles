#include "Entity.h"
#include <vector>
#include <iostream>
#include <math.h>
#include "Global.h"
#include <SFML/Graphics.hpp>
using namespace std;

Entity::Entity(float size) :   m_size(size), m_speed(0), m_speedBonus(0), m_timeBonus(0)
// This class implements an Entity: A disc SFML shape with a few attributes
// param size (float): the radius of the disc
// return null
{
    setSize(size);
}

Entity::~Entity()
// Destructor
{
}

void Entity::draw(sf::RenderWindow &window)
// Draws an entity in a window
// param window (RenderWindow): the SFML window in which to draw
// return null
{
	window.draw(*this);
}


sf::Vector2f Entity::getCenter()
// Getter (Vector2f) of the position of the center of this entity
{
	return m_position;
}

void Entity::setCenter(sf::Vector2f position)
// Setter of this entity's center position
// param position (Vector2f): the new center position to give to this entity
// return null
{
	m_position = position;
	setPosition(m_position - sf::Vector2f(m_size, m_size));
}

sf::Color Entity::getColor()
// Getter (Color) of this entity's color
{
	return m_color;
};

void Entity::setColor(sf::Color color)
// Setter of this entity's color
// param color (Color): the new color to give to this entity
// return null
{
	m_color = color;
	setFillColor(color);
};

float Entity::getSize()
// Getter (float) of the size of this entity
{
	return m_size;
};

void Entity::setSize(float size)
// Setter of this entity's size
// param size (float): the new size to give to this entity
// return null
{
	m_size = size;
	setRadius(m_size);
	setOutlineThickness(fmin(m_size / 10,5));
};

void Entity::setSpeed(float size)
// Change the size of this entity
// param size (float): the new size to which the speed must be correlated
// return null
{
	m_speed = 200 / size;
};

sf::Vector2f Entity::move(sf::Vector2f target)
// Process the new coordinates of this entity for the next frame, given a target and using this entity's speed
// param target (Vector2f): Coordinates of this entity's target point
// return move (Vector2f): Coordinates of this entity for the next frame
{

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

void Entity::getEaten(Entity& predator)
// Method to call when the this entity is eaten by another entity predator
// param predator (Entity): the Entity (likely a Cell) that eats this entity
// return null
{

}