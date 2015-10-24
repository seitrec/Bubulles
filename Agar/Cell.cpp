#include "Cell.h"
#include "Global.h"
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

void Cell::eat(Entity Entity)
{
}

void Cell::getEaten(Entity cell)
{
}

void Cell::eject()
{
}
