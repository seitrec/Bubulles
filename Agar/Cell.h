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
	void setSpeed(float size);
	void drawName(sf::RenderWindow &window, sf::Font &font, std::string name);
	float getSpeed();
	Cell split(sf::Vector2f target);
	void eat(Entity &entity);
	void eject();
	bool checkCollision(Cell &cell);
	bool checkCollisionMyCells(Cell &cell);
	bool checkCollision(Food &food);
	void setMoved(bool b);
	bool getWasMoved();
//	void actionCollision(Entity& entity,Player& player, std::vector<Food>& lFood, std::vector<Player>& lPlayer);
private:
	bool wasMoved;
};

