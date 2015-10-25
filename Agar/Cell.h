#pragma once
#include<vector>
#include "Food.h"
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
    void getEaten(Entity entity);
	void eject();
	void checkCollision(std::vector<Food> *ptrlFood);
};

