#pragma once
#include "Cell.h"
#include "Food.h"


class Player
{
public:
	Player();
	~Player();
	void move();
	void split();
	void eject();
	std::vector<Cell> &getCells();
	void addCell(Cell cell);
	void delCell(Cell cell);
	void setTarget(sf::Vector2f target);
	sf::Vector2f getViewCenter();
	sf::Vector2f getClosestLocation(std::vector<Food> &lFood);
	void setCellZone();
	std::vector<float> &getCellZone();
	void drawCells(sf::RenderWindow *ptrWindow);
	void checkCollision(std::vector<Food> *ptrlFood);

private:
	std::vector<Cell> m_cells;
	sf::Vector2f m_target;
	std::vector<float> m_cells_zone;
};

