#include "Player.h"
#include "Global.h"
#include <iostream>
#include <math.h>

Player::Player(Cell firstcell , bool isIA):m_score(0), merge_available(0)
{
    setStrategy(isIA);
    addCell(firstcell);
}


Player::~Player()
{
}

void Player::move(int splitTime)
{
/*	int nbCells = m_cells.size();
	int nbMoved = 0;

	while (nbMoved != nbCells)
	{
		for (int i = 0; i < m_cells.size(); ++i)
		{
			if (!m_cells[i].getWasMoved())
			{
				//Cas à traiter quand move = 0
				sf::Vector2f move = m_cells[i].move(m_target);
				Cell copieCell = m_cells[i];
				copieCell.setCenter(sf::Vector2f(m_cells[i].getCenter().x + std::copysignf(1,move.x), m_cells[i].getCenter().y));
				for (int j = 0; j < m_cells.size(); ++j)
				{
					if (j != i)
					{
						if (copieCell.checkCollisionMyCells(m_cells[j]))
						{
							//Il y a collision même en bougeant d'un pixel donc on ne peut rien faire.
						}
						else
						{
							//Il y a collision
						}
					}
				}
			}
		}
	}*/
	for (int i = 0; i < m_cells.size(); ++i)
	{
		sf::Vector2f move = m_cells[i].move(m_target);
		Cell copieCell = m_cells[i];
		copieCell.setSize(copieCell.getSize());
		copieCell.setCenter(sf::Vector2f(m_cells[i].getCenter().x + move.x, m_cells[i].getCenter().y + move.y));
		for (int j = i+1; j < m_cells.size(); ++j)
		{
				if ((copieCell.checkCollisionMyCells(m_cells[j])) && !(canMerge(splitTime)))
				{
					sf::Vector2f new_target = sf::Vector2f(m_cells[i].getCenter().x - m_cells[j].getCenter().x, m_cells[i].getCenter().y - m_cells[j].getCenter().y);
					//if (move.x > m_cells[j].move(m_target).x)
					//{
					move = m_cells[i].move(m_cells[i].getCenter() + new_target);
					//}
			}
		}
			
		m_cells[i].setCenter(sf::Vector2f(m_cells[i].getCenter().x + move.x, m_cells[i].getCenter().y + move.y));
	}
}

bool Player::canMerge(int splitTime)
{
	return (splitTime > merge_available);
}

void Player::split(int splitTime)
{
	int nb_cells = m_cells.size();
	//if (min_size > 10)
	for (int j = 0; j < nb_cells; ++j)
	{
		if (m_cells[j].getSize() > 40 && m_cells.size()<16)
		{
			int new_cell_id = m_cells.size();
			merge_available = splitTime + 10;
			this->addCell(m_cells[j].split(m_target)); //TODO supprimer target du split


			sf::Vector2f move = (m_cells[new_cell_id].move(m_target));
			float norme = sqrt(move.x*move.x + move.y*move.y);
			move = (m_cells[j].getSize()*2/norme)*move;
			m_cells[new_cell_id].setCenter(sf::Vector2f(m_cells[new_cell_id].getCenter().x + move.x, m_cells[new_cell_id].getCenter().y + move.y));
		}
	}

}

void Player::eject()
{
}

std::vector<Cell> &Player::getCells()
{
	return m_cells;
}

void Player::addCell(Cell cell)
{
	m_cells.push_back(cell);
}

void Player::delCell(int k)
{
	m_cells.erase(m_cells.begin() + k);
}

void Player::setTarget(sf::Vector2f target)
{
	m_target = target;
}

sf::Vector2f Player::getViewCenter()
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

void Player::splitIA(std::vector<Food> &lFood, std::vector<Player> &lPlayer, int Player_id, int splitTime)
{
	if (!strategy.at("split").at("human")) {
		Cell closestCell;
		int distance;
		std::tie(closestCell, distance) = getClosestCell(lPlayer, Player_id);

		if (strategy.at("split").at("aggro") && (m_cells.size() == 1) ) {
			for (int i = 0; i < lPlayer.size(); ++i) {
				if (distance <= 1.8 * m_cells[0].getSize() && m_cells[0].getSize() / sqrt(2) > 1.05* closestCell.getSize()) {
					setTarget(closestCell.getCenter());
					split(splitTime);
				}
			}
		}
		if (strategy.at("split").at("spread")) {
			if (distance > 500) {
				split(splitTime);
			}
		}
	}
}

void Player::setIATarget(sf::Vector2f mouseCoordonates, std::vector<Food> &lFood, std::vector<Player> &lPlayer, int Player_id)
{	
	if (strategy.at("target").at("human") == true)
	{
		setTarget(mouseCoordonates);
	}
	else
	{
		if (strategy.at("target").at("closest") == true)
		{
			setCellZone();
			setTarget(getClosestLocation(lFood));
		}
		if (strategy.at("target").at("flee") == true)
		{
			Cell closestCell;
			int distance;
			std::tie(closestCell, distance) = getClosestCell(lPlayer, Player_id);
			if (distance < 1,5* closestCell.getSize() && closestCell.getSize() > getSmallestCellSize()) {
				setTarget(getViewCenter() + getViewCenter() - closestCell.getCenter());
			}
		}
	}
}

std::tuple<Cell, int> Player::getClosestCell(std::vector<Player> &lPlayer, int Player_id)
{
	float minDistance = WORLD_SIZE;
	float distance;
	Cell closestCell;
	for (int i = 0; i < lPlayer.size(); ++i) {
		if (i != Player_id) {
			for (int j = 0; j < lPlayer[i].m_cells.size(); ++j)
			{
				distance = sqrt(pow((m_cells[0].getCenter().x - lPlayer[i].m_cells[j].getCenter().x), 2) +
					pow((m_cells[0].getCenter().y - lPlayer[i].m_cells[j].getCenter().y), 2));

				if (distance < minDistance)
				{
					minDistance = distance;
					closestCell = lPlayer[i].m_cells[j];
				}
			}
		}
	}
	return std::make_tuple(closestCell, minDistance);
}

sf::Vector2f Player::getClosestLocation(std::vector<Food> &lFood)
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
		if (m_cells[i].getCenter().x + m_cells[i].getSize() > m_cells_zone[0])
		{
			m_cells_zone[0] = marge + m_cells[i].getCenter().x + m_cells[i].getSize();
		}
		if (m_cells[i].getCenter().x - m_cells[i].getSize() < m_cells_zone[1])
		{
			m_cells_zone[1] = marge + m_cells[i].getCenter().x - m_cells[i].getSize();
		}
		if (m_cells[i].getCenter().y + m_cells[i].getSize() > m_cells_zone[2])
		{
			m_cells_zone[2] = marge +m_cells[i].getCenter().y + m_cells[i].getSize();
		}
		if (m_cells[i].getCenter().y - m_cells[i].getSize() < m_cells_zone[3])
		{
			m_cells_zone[3] = marge + m_cells[i].getCenter().y - m_cells[i].getSize();
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
{
	return m_cells_zone;
}
void Player::drawCells(sf::RenderWindow * ptrWindow)
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		ptrWindow->draw(m_cells[i]);
	}

}
void Player::drawName(sf::RenderWindow &window, sf::Font &font)
{

	for (int i = 0; i < m_cells.size(); ++i)
	{
		m_cells[i].drawName(window, font, this->getName());
	}
}

void Player::drawCellsScore(sf::RenderWindow & window, sf::Font & font)
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		m_cells[i].drawScore(window, font);
	}
}

void Player::checkCollision(std::vector<Food>* ptrlFood)
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		m_cells[i].checkCollision(m_cells[0]);
	}
}

std::string Player::getName()
{
	return m_name;
}

void Player::setName(std::string name)
{
	m_name = name;
}

void Player::setMoved(bool b)
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		m_cells[i].setMoved(b);
	}
}

void Player::setScore()
{
	float score = 0;
	for (int i = 0; i < m_cells.size(); ++i)
	{
		score+=m_cells[i].getSize();
	}
	m_score = roundf(score);

}

void Player::setStrategy(bool isIA)
{
	if (isIA) {
		strategy = { {"split", { { "spread", true }, { "aggro", true }, { "human", false } } }, { "target", { { "closest", true }, { "flee", true }, { "human", false } } } };
	}
	else { 
		strategy = { { "split",{ { "spread", false },{ "aggro", false }, { "human", true } } },{ "target",{ { "closest", false },{ "flee", false }, { "human", true } } } };
	}
}

float Player::getSmallestCellSize()
{
	float smallest = m_cells[0].getSize();
	for (int i = 1; i < m_cells.size(); ++i)
	{
		if (m_cells[i].getSize() < smallest) {
			smallest = m_cells[i].getSize();
		}
	}
	return smallest;
}


int Player::getScore()
{
	return m_score;
}
