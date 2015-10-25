#include "Cell.h"
#include "Global.h"
#include "Food.h"
#include <SFML/Graphics.hpp>

Cell::Cell()
{
	setPosition2(sf::Vector2f((rand() % worldSize + 1), (rand() % worldSize + 1)));
	setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	setSize(40);
	}

Cell::~Cell()
{
}

void Cell::split(sf::Vector2f target)
{
}

void Cell::eat(Entity entity)
{
   // entity.getEaten(*this);
}

void Cell::getEaten(Entity entity)
{}

void Cell::eject()
{
}

void Cell::checkCollision(std::vector<Food>* ptrlFood)
{
	for (unsigned i = 0; i < ptrlFood->size(); ++i)
	{
		if (sqrt(pow((this->getPosition().x - ptrlFood->at(i).getPosition().x), 2) +
			pow((this->getPosition().y - ptrlFood->at(i).getPosition().y), 2))
			< fabs(this->getSize() - ptrlFood->at(i).getSize() / 2))
		{
			//TODO rajouter un argument à get eaten
			ptrlFood->at(i).getEaten();
			ptrlFood->erase(ptrlFood->begin()+i);
		}
	}

}