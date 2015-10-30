#pragma once
#include "Cell.h"
#include "Food.h"


class Player
{
public:
	Player();
	~Player();
	void move(int);
	void split(int);
	void eject();
	std::vector<Cell> &getCells();
	void addCell(Cell cell);
	void delCell(int k);
	void setTarget(sf::Vector2f target);
	sf::Vector2f getViewCenter();
	void splitIA(std::vector<Food> &lFood, std::vector<Player> &lPlayer, int splitTime);
	void setIATarget(sf::Vector2f mouseCoordonates, std::vector<Food> &lFood, std::vector<Player> &lPlayer);
	std::tuple<Cell, int> getClosestCell(std::vector<Player> &lPlayer);
	sf::Vector2f getClosestLocation(std::vector<Food> &lFood);
	void setCellZone();
	std::vector<float> &getCellZone();
	void drawCells(sf::RenderWindow *ptrWindow);
	void drawName(sf::RenderWindow &window, sf::Font &font);
	void drawCellsScore(sf::RenderWindow &window, sf::Font &font);
	void checkCollision(std::vector<Food> *ptrlFood);
	std::string getName();
	void setName(std::string name);
	void setMoved(bool b);
	void setScore();
	void setStrategy(bool isIA);
	int getScore();
	bool canMerge(int);


private:
	std::string m_name;
	std::vector<Cell> m_cells;
	sf::Vector2f m_target;
	std::vector<float> m_cells_zone;
	int m_score;
	std::map<std::string, std::map<std::string, bool>> strategy;
	int merge_available;
};

