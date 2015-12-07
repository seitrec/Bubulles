#pragma once
#include<vector>
#include "Food.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>


class Cell :
	public Entity
	
{
public:
	Cell(float size=10);
	~Cell();
	void drawName(sf::RenderWindow &window, sf::Font &font, std::string name);
	void drawScore(sf::RenderWindow &window, sf::Font &font);
	virtual void getEaten(Entity &predator);
	Cell split(sf::Vector2f target);
	bool checkStrictCollision(Cell &cell);
	bool checkCollisionCovering(Entity &entity);
	void setMoved(bool b);
	bool getWasMoved();

private:
	bool wasMoved;
};

