#include "Player.h"
#include "Global.h"
#include <iostream>
#include <math.h>
#include "Cell.h"


Player::Player(bool isIA):m_score(0), merge_available(0)
// This class implements a Player, controlled either by a human or a bot, and capable of controlling cells
// param isIA (bool): Determines if a player will be controlled by a human (0) or a bot (1)
// return null
{
    setStrategy(isIA);
    Cell cell;
    if (isIA)
    {
        cell= Cell(10);
    }
    else
    {
        cell=Cell(20);
    }
    addCell(&cell);
}


Player::~Player()
// destructor
{
}


void Player::move(int splitTime)
// Changes the postion of each cell for this Player
// param splitTime (int): time of last split, determining if this player's cells can merge or not 
// (if they can't, they can't move through each other)
// return null
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		
        sf::Vector2f move = m_cells[i]->move(m_target);
		Cell *ptrCell = (m_cells[i]);
		ptrCell->setSize(ptrCell->getSize());
		ptrCell->setCenter(sf::Vector2f(ptrCell->getCenter().x + move.x, ptrCell->getCenter().y + move.y));
		for (int j = i+1; j < m_cells.size(); ++j)
		{
				if ((ptrCell->checkStrictCollision(*m_cells[j])) && !(canMerge(splitTime)))
				{
					sf::Vector2f new_target = sf::Vector2f(m_cells[i]->getCenter().x - m_cells[j]->getCenter().x, m_cells[i]->getCenter().y - m_cells[j]->getCenter().y);
					move = ptrCell->move(ptrCell->getCenter() + new_target);
			}
		}
			
		m_cells[i]->setCenter(sf::Vector2f(ptrCell->getCenter().x + move.x, ptrCell->getCenter().y + move.y));
	}
}


bool Player::canMerge(int splitTime)
// Determine if this player's cells can merge, or not
// param splitTime (int): Time of last split
// return unnamed (bool): 1 if cells can merge, 0 else
{
	return (splitTime > merge_available);
}


void Player::split(int splitTime)
// Splits all this player's cells that can split
// param spliTime (int): Time of last split
// return null
{
	int nb_cells = m_cells.size();
	//if (min_size > 10)
	for (int j = 0; j < nb_cells; ++j)
	{
		if (m_cells[j]->getSize() > 40)
		{
			merge_available = splitTime + 10;
			m_cells[j]->split(m_target);
        }
	}
}


std::vector<Cell*> &Player::getCells()
// Getter for a player's list of cells
// return m_cells (vector<Cell>): vector of a player's cells
{
	return m_cells;
}


void Player::addCell(Cell *cell)
// Add a new cell to this player's list
// param cell (Cell): the cell to add
// return null
{
	m_cells.push_back(cell);
    cell->setPlayer(this);
}


void Player::delCell(int k)
// Delete a cell from this player's list
// param k (int): Index of the cell to delete
// return null
{
	m_cells.erase(m_cells.begin() + k);
}


void Player::setTarget(sf::Vector2f target)
// Setter of the target for this player's move during this frame
// param target (Vector2f): coordinates of the target location
// return null
{
	m_target = target;
}


sf::Vector2f Player::getViewCenter()
// Determine the position of the camera view for this player
// return centreView (Vector2f): Coordinates of the camera center
{
	//Si on arrive près des bords il ne faut plus prendre le centre des cellules
	sf::Vector2f centreCells = sf::Vector2f(((m_cells_zone[0] + m_cells_zone[1]) / 2), ((m_cells_zone[2] + m_cells_zone[3]) / 2));
	sf::Vector2f centreView = centreCells;

	if (centreCells.x < WINDOW_SIZE / 4)
	{
		centreView.x = WINDOW_SIZE / 4;
	}
	else if (centreCells.x > WORLD_SIZE - WINDOW_SIZE / 4)
	{
		centreView.x = WORLD_SIZE - WINDOW_SIZE / 4;
	}

	if (centreCells.y < WINDOW_SIZE / 4)
	{
		centreView.y = WINDOW_SIZE / 4;
	}
	else if (centreCells.y > WORLD_SIZE - WINDOW_SIZE / 4)
	{
		centreView.y = WORLD_SIZE - WINDOW_SIZE / 4;
	}

	return centreView;
}


void Player::splitIA(std::vector<Player> &lPlayer, int Player_id, int splitTime)
// All the actions related to splitting for the IAs: Depending on their designed behaviour, IAs will choose, or not, to split
// param &lPlayer (vector<Player>): List of the players, usefull to IAs to determine if they are 'safe'
// param Player_id (int): The id of the player trying to split
// param splitTime (int): Time of last split, determining if the player can split
// return null
{
	if (!strategy.at("split").at("human")) {
		Cell* closestCell;
		int distance;
		std::tie(closestCell, distance) = getClosestCell(lPlayer, Player_id);

		if (strategy.at("split").at("aggro") && (m_cells.size() == 1) ) {
			for (int i = 0; i < lPlayer.size(); ++i) {
				if (distance <= 1.8 * m_cells[0]->getSize() && m_cells[0]->getSize() / sqrt(2) > 1.05* closestCell->getSize()) {
					setTarget(closestCell->getCenter());
					split(splitTime);
				}
			}
		}
		if (strategy.at("split").at("spread")) {
			if (distance > 800) {
				split(splitTime);
			}
		}
	}
}


void Player::setIATarget(sf::Vector2f mouseCoordonates, std::vector<Food> &lFood, std::vector<Player> &lPlayer, int Player_id)
// All the actions related to targetting a move for the players and IAs: Depending on their designed behaviour, IAs will choose where they want to go
// param mouseCoordonates (Vector2f): Couple of coordinates of the mouse, for the player's move
// param &lFood (vector<Food>): List of the foods, for the most basic move: 'I need food'
// param &lPlayer (vector<Player>): List of the players, usefull to IAs to determine how they are 'safe'
// param Player_id (int): The id of the player trying to move
// return null
{	
	if (strategy.at("target").at("human") == true)
	{
		setTarget(mouseCoordonates);
	}
	else
	{
		if (strategy.at("target").at("closest") == true) // Go to the closest food
		{
			setCellZone();
			setTarget(getClosestLocation(lFood));
		}
		if (strategy.at("target").at("stalk") == true) // Follow an eatable cell, if it's close enough to seem reachable
		{
			Cell* closestCell;
			int distance;
			std::tie(closestCell, distance) = getClosestCell(lPlayer, Player_id);
			if ((distance < 4*closestCell->getSize() && m_cells.size() == 1) && EATING_RATIO*closestCell->getSize() < getSmallestCellSize()) {
				setTarget(closestCell->getCenter());
			}
		}
		if (strategy.at("target").at("flee") == true) // Think about survival and get away from a close and big annemy
		{
			Cell* closestCell;
			int distance;
			std::tie(closestCell, distance) = getClosestCell(lPlayer, Player_id);
			if (distance < 4*closestCell->getSize() && closestCell->getSize() > EATING_RATIO*getSmallestCellSize()) {
				setTarget(getViewCenter() + getViewCenter() - closestCell->getCenter());
			}
		}
	}
}


std::tuple<Cell*, int> Player::getClosestCell(std::vector<Player> &lPlayer, int Player_id)
// Process which ennemy cell is the closest to a player
// param &lPlayer (vector<Player>): List of the players
// param Player_id (int): The id of the player
// return (closestCell, minDistance) (tuple<Cell, int>): closestCell is the closest cell found, and minDistance the distance to it
{
	float minDistance = WORLD_SIZE;
	float distance;
	Cell *closestCell;
	for (int i = 0; i < lPlayer.size(); ++i) {
		if (i != Player_id) {
			for (int j = 0; j < lPlayer[i].getCells().size(); ++j)
			{
				distance = sqrt(pow((m_cells[0]->getCenter().x - lPlayer[i].m_cells[j]->getCenter().x), 2) +
					pow((m_cells[0]->getCenter().y - lPlayer[i].m_cells[j]->getCenter().y), 2));

				if (distance < minDistance)
				{
					minDistance = distance;
					closestCell = lPlayer[i].getCells()[j];
				}
			}
		}
	}
	return std::make_tuple(closestCell, minDistance);
}


sf::Vector2f Player::getClosestLocation(std::vector<Food> &lFood)
// Process the closest food for a player
// param &lFood (vector<Food>): List of all available foods
// return location (Vector2f): Couple of coordonates of the closest food
{
	float minDistance = WORLD_SIZE;
	float distance;
	sf::Vector2f location = sf::Vector2f(0, 0);
	for (int i = 0; i < lFood.size(); ++i)
	{
		distance = sqrt(pow((this->getViewCenter().x - lFood[i].getCenter().x), 2) +
						pow((this->getViewCenter().y - lFood[i].getCenter().y), 2));

		if (distance < minDistance)
		{
			minDistance = distance;
			location = lFood[i].getCenter();
		}
	}
	return location;
}


void Player::setCellZone()
// Updates m_cell_zone, 4 coordonates representing the smallest rectangle containing a player's cells
// return null
{
	int marge = 10;
	//xmax,xmin,ymax,ymin
	m_cells_zone.clear();
	m_cells_zone.push_back(0);
	m_cells_zone.push_back(WORLD_SIZE);
	m_cells_zone.push_back(0);
	m_cells_zone.push_back(WORLD_SIZE);

	for (int i = 0; i < m_cells.size(); ++i)
	{
		if (m_cells[i]->getCenter().x + m_cells[i]->getSize() > m_cells_zone[0])
		{
			m_cells_zone[0] = marge + m_cells[i]->getCenter().x + m_cells[i]->getSize();
		}
		if (m_cells[i]->getCenter().x - m_cells[i]->getSize() < m_cells_zone[1])
		{
			m_cells_zone[1] = marge + m_cells[i]->getCenter().x - m_cells[i]->getSize();
		}
		if (m_cells[i]->getCenter().y + m_cells[i]->getSize() > m_cells_zone[2])
		{
			m_cells_zone[2] = marge +m_cells[i]->getCenter().y + m_cells[i]->getSize();
		}
		if (m_cells[i]->getCenter().y - m_cells[i]->getSize() < m_cells_zone[3])
		{
			m_cells_zone[3] = marge + m_cells[i]->getCenter().y - m_cells[i]->getSize();
		}
	}
	if (m_cells.size() == 0)
	{
		m_cells_zone[0] = 0;
		m_cells_zone[1] = WORLD_SIZE;
		m_cells_zone[2] = 0;
		m_cells_zone[3] = WORLD_SIZE;
	}
}


std::vector<float> &Player::getCellZone()
// Getter for m_cells_zone
// return m_cells_zone (vector): 4 coordonates representing the smallest rectangle containing a player's cells
{
	return m_cells_zone;
}


void Player::drawCells(sf::RenderWindow * ptrWindow)
// Draw a player's cells on the main window
// param ptrWindow (RenderWindow): The main game window
// return null
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		ptrWindow->draw(*m_cells[i]);
	}
}


void Player::drawName(sf::RenderWindow &window, sf::Font &font)
// Draw a player's name on his cells
// param ptrWindow (RenderWindow): The main game window
// param font (Font): The font used to write the name
// return null
{

	for (int i = 0; i < m_cells.size(); ++i)
	{
		m_cells[i]->drawName(window, font, this->getName());
	}
}


void Player::drawCellsScore(sf::RenderWindow & window, sf::Font & font)
// Draw a player's Score on his cells
// param ptrWindow (RenderWindow): The main game window
// param font (Font): The font used to write the score
// return null
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		m_cells[i]->drawScore(window, font);
	}
}


std::string Player::getName()
// Getter of a player's name
// return m_name (string): this player's name
{
	return m_name;
}


void Player::setName(std::string name)
// Setter of a player's name
// param name (string): this player's name
{
	m_name = name;
}


void Player::setScore()
// Updater of a player's score
// return null
{
	float score = 0;
	if (m_cells.size() == 1)
	{
		m_score = roundf(m_cells[0]->getSize());
	}
	else
	{
		for (int i = 0; i < m_cells.size(); ++i)
		{
			score+=pow(m_cells[i]->getSize(),2);
		}
		m_score = roundf(sqrt(score));
	}

}

void Player::setStrategy(bool isIA)
{
	if (isIA) {
		strategy = { {"split", { { "spread", true }, { "aggro", true }, { "human", false } } }, { "target", { { "closest", true }, { "flee", true },{ "stalk", true }, { "human", false } } } };
	}
	else { 
		strategy = { { "split",{ { "spread", false },{ "aggro", false }, { "human", true } } },{ "target",{ { "closest", false },{ "flee", false },{ "stalk", false }, { "human", true } } } };
	}
}

float Player::getSmallestCellSize()
{
	float smallest = m_cells[0]->getSize();
	for (int i = 1; i < m_cells.size(); ++i)
	{
		if (m_cells[i]->getSize() < smallest) {
			smallest = m_cells[i]->getSize();
		}
	}
	return smallest;
}


int Player::getScore()
{
	return m_score;
}
