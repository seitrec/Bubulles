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
	void eject();
	bool checkCollision(Entity &entity);
//	void actionCollision(Entity& entity,Player& player, std::vector<Food>& lFood, std::vector<Player>& lPlayer);
};

