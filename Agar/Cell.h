#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

class Cell :
	public Entity
	
{
public:
	Cell();
	~Cell();
	void split(sf::Vector2f target);
	void eat(Entity Entity);
	void getEaten(Entity cell);
	void eject();

};

