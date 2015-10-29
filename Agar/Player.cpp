#include "Player.h"
#include "Global.h"
#include <iostream>
#include <math.h>

Player::Player()
{
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
		if (m_cells[j].getSize() > 10 && m_cells.size()<16)
		{
			this->addCell(m_cells[j].split(m_target)); //TODO supprimer target du split
			merge_available = splitTime + 10;
			bool collision = m_cells[nb_cells].checkCollisionMyCells(m_cells[j]);
			while(collision)
			{
				
				sf::Vector2f move = (m_cells[nb_cells].move(m_target));
				float norme = sqrt(move.x*move.x + move.y*move.y);
				move = (m_cells[j].getSize()*2/norme)*move;
				m_cells[nb_cells].setCenter(sf::Vector2f(m_cells[nb_cells].getCenter().x + move.x, m_cells[nb_cells].getCenter().y + move.y));
				collision = m_cells[nb_cells].checkCollisionMyCells(m_cells[j]);
			}
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

	if (centreCells.x < windowSize / 2)
	{
		centreView.x = windowSize / 2;
	}
	else if (centreCells.x > worldSize - windowSize / 2)
	{
		centreView.x = worldSize - windowSize / 2;
	}

	if (centreCells.y < windowSize / 2)
	{
		centreView.y = windowSize / 2;
	}
	else if (centreCells.y > worldSize - windowSize / 2)
	{
		centreView.y = worldSize - windowSize / 2;
	}

	return centreView;
}

sf::Vector2f Player::getClosestLocation(std::vector<Food> &lFood)
{
	float minDistance = worldSize;
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
	m_cells_zone.push_back(worldSize);
	m_cells_zone.push_back(0);
	m_cells_zone.push_back(worldSize);

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
			m_cells_zone[1] = worldSize;
			m_cells_zone[2] = 0;
			m_cells_zone[3] = worldSize;
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

int Player::getScore()
{
	return m_score;
}
