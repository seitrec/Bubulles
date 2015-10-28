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
	void delCell(int k);
	void setTarget(sf::Vector2f target);
	sf::Vector2f getViewCenter();
	sf::Vector2f getClosestLocation(std::vector<Food> &lFood);
	void setCellZone();
	std::vector<float> &getCellZone();
	void drawCells(sf::RenderWindow *ptrWindow);
	void drawName(sf::RenderWindow &window, sf::Font &font);
	void checkCollision(std::vector<Food> *ptrlFood);
	std::string getName();
	void setName(std::string name);


private:
	std::string m_name;
	std::vector<Cell> m_cells;
	sf::Vector2f m_target;
	std::vector<float> m_cells_zone;
};

