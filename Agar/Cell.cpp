#include "Cell.h"
#include "Global.h"
#include "Food.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <math.h>

Cell::Cell()
{
	//SetSize avant SetCenter sinon impossible de d�finir le centre....
	setSize(40);
	m_speed = 5;
	setCenter(sf::Vector2f(800,800));
	setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
}

Cell::~Cell()
{
}

Cell Cell::split(sf::Vector2f target)
{
	this->setSize(this->getSize()/sqrt(2));
	Cell child;
	child.setSize(this->getSize());
	child.setColor(this->getColor());
	float relativeCenterMouseX;
	relativeCenterMouseX = target.x - (this->getCenter().x + m_size);
	float relativeCenterMouseY;
	relativeCenterMouseY = target.y - (this->getCenter().y + m_size);
	if (fabs(relativeCenterMouseX) > 3 || fabs(relativeCenterMouseY) > 3)
	{
		float c = sqrt(((m_speed + 3 * m_size) * (m_speed + 3 * m_size)) / (relativeCenterMouseX * relativeCenterMouseX + relativeCenterMouseY * relativeCenterMouseY));
		child.setCenter(sf::Vector2f (this->getCenter().x + c * relativeCenterMouseX, this->getCenter().y + c * relativeCenterMouseY));
	}
	return child;
}

void Cell::eat(Entity entity) 
{
}


void Cell::eject()
{
}


bool Cell::checkCollision(Entity &entity) //ne faudrait-il mieux pas utiliser const& ptrlFood ? on passe l'adresse du vecteur, et on le laisse constant vu qu'on ne va pas le modifier, juste le parcourir
{
	return (sqrt(pow((this->getCenter().x - entity.getCenter().x), 2) +
		pow((this->getCenter().y - entity.getCenter().y), 2))
		< fabs(this->getSize() - entity.getSize() / 2));
}
/*
void Cell::actionCollision(Entity & entity, Player & player, std::vector<Food>& lFood, std::vector<Player>& lPlayer)
{
	float difEatMin = 1.1;
	if (this->getSize() >= entity.getSize()*difEatMin)
	{
		entity.getEaten(*this, player, lPlayer);

	}
	else
	{
		//Faire qqch de graphique
	}
}
*/