#include <vector>
#include <math.h>
#include "Entity.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
using namespace std;


oldEntity::Entity() : sf::CircleShape(5), m_currentX(rand() % worldSize + 1), m_currentY(rand() % worldSize + 1)
{
	setPosition(sf::Vector2f(m_currentX, m_currentY));
	setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	setSize(5);
}

oldEntity::~Entity()
{
}

sf::Vector2f Entity::getCenter()
{
	sf::Vector2f r(this->getPosition().x + m_size, this->getPosition().y + m_size);
	return r;
}

void Entity::setSize(float radius)
{
	m_size = radius;
	setRadius(radius);
}

void Entity::move(sf::Vector2f target)
{

}

float Entity::getSize()
{
	return m_size;
}
void Entity::setSize(float size)
{
	m_size = size;
}
void Entity::setColor(sf::Color color)
{
	m_color = color;
	setFillColor(color);
}
sf::Color Entity::getColor()
{
	return m_color;
}

Entity Entity::getClosest(vector<Entity> lEntity)
{
	float dist_min=worldSize;
	float dist;
	Entity closest;
	for (int i=0; i<lEntity.size();++i)
	{
		dist = sqrt(pow((lEntity[i].m_currentX - this->m_currentX),2) + pow((lEntity[i].m_currentY - this->m_currentY),2));
		if (dist < dist_min)
		{
			dist_min = dist;
			closest=lEntity[i];
		}
	}
	return closest;
}
