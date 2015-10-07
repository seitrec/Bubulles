#include "Bubble.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
#include <math.h>
#include <algorithm>  

using namespace std;


Bubble::Bubble(): Entity()
{
	m_currentY = 100;
	m_currentX=100;
	setPosition(sf::Vector2f(m_currentX, m_currentY));
	setSize(5);
}

Bubble::~Bubble()
{
}

void Bubble::Move(float mouseX, float mouseY)
{

	float relativeCenterMouseX = mouseX - (this->getPosition().x+m_size);
	float relativeCenterMouseY = mouseY - (this->getPosition().y+m_size);
	float moveX=0;
	float moveY=0;
	if ((abs(relativeCenterMouseX)>3 || abs(relativeCenterMouseY)>3))
	{
		float c = sqrt((m_speed*m_speed) / (relativeCenterMouseX*relativeCenterMouseX + relativeCenterMouseY*relativeCenterMouseY));
		moveX = c*relativeCenterMouseX;
		moveY = c*relativeCenterMouseY;
		if (m_currentX <0)
		{
			moveX = std::fmax(moveX, 0.0);
		}
		else if (m_currentX >worldSize)
		{
			moveX = std::fmin(moveX, 0);
		}
		if (m_currentY <0)
		{
			moveY = std::fmax(moveY, 0.0);
		}
		else if (m_currentY >worldSize)
		{
			moveY = std::fmin(moveY, 0);
		}
		move(sf::Vector2f(moveX,moveY));
	} 
	m_currentX = (this->getPosition().x + m_size);
	m_currentY = (this->getPosition().y + m_size);
}

void Bubble::Eat(float size)
{
	m_size = sqrt(getRadius()*getRadius() + size*size);
	setSize(m_size);
}

void Bubble::setSize(float radius)
{
	setRadius(radius);
	m_size = radius;
	m_speed = 40 / radius;
}

Bubble Bubble::Split(float mouseX, float mouseY)
{
	float size = getSize() / sqrt(2);
	Bubble bubble;
	bubble.setSize(size);
	this->setSize(size);
	float relativeCenterMouseX = mouseX - (this->getPosition().x + m_size);
	float relativeCenterMouseY = mouseY - (this->getPosition().y + m_size);
	if (abs(relativeCenterMouseX) > 3 || abs(relativeCenterMouseY) > 3)
	{
		float c = sqrt(((m_speed+3*m_size)*(m_speed+3*m_size)) / (relativeCenterMouseX*relativeCenterMouseX + relativeCenterMouseY*relativeCenterMouseY));
		bubble.setPosition(getPosition().x + c*relativeCenterMouseX, getPosition().y + c*relativeCenterMouseY);
	}
	return bubble;
}
