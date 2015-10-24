#pragma once
#include "Cell.h"


class Player
{
public:
	Player();
	~Player();
	void move();
	void split();
	void eject();
	std::vector<Entity> getCells();
	void addCell(Entity cell);
	void delCell(Entity cell);
	void setTarget(sf::Vector2f target);
	sf::Vector2f getViewCenter();
	void drawCells(sf::RenderWindow *ptrWindow);

private:
	std::vector<Entity> m_cells;
	sf::Vector2f m_target;
};

