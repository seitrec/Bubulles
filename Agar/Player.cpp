#include "Player.h"
#include "Global.h"
#include <iostream>

Player::Player()
{
}


Player::~Player()
{
}

void Player::move()
{

	for (int i = 0; i < m_cells.size(); ++i)
	{
		sf::Vector2f relativeCentertoTarget((m_target.x) - (m_cells[i].getCenter().x),
			(m_target.y) - (m_cells[i].getCenter().y));

		float moveX = 0;
		float moveY = 0;
		if ((fabs(relativeCentertoTarget.x)>3 || fabs(relativeCentertoTarget.y)>3))
		{
			float c = sqrt((m_cells[i].getSpeed()*m_cells[i].getSpeed()) / (relativeCentertoTarget.x*relativeCentertoTarget.x + relativeCentertoTarget.y*relativeCentertoTarget.y));
			moveX = c*relativeCentertoTarget.x;
			moveY = c*relativeCentertoTarget.y;

			if (m_cells[i].getCenter().x - m_cells[i].getSize() <0)
			{
				moveX = fmax(moveX, 0.0);
			}
			else if (m_cells[i].getCenter().x + m_cells[i].getSize()>worldSize)
			{
				moveX = fmin(moveX, 0);
			}
			if (m_cells[i].getCenter().y - m_cells[i].getSize() <0)
			{
				moveY = fmax(moveY, 0.0);
			}
			else if (m_cells[i].getCenter().y + m_cells[i].getSize() >worldSize)
			{
				moveY = fmin(moveY, 0);
			}

			Cell copieCell = m_cells[i];
			copieCell.setSize(copieCell.getSize());
			for (int j = 0; j < m_cells.size(); ++j)
			{
				if (j != i) { 
					copieCell.setCenter(sf::Vector2f(m_cells[i].getCenter().x + moveX, m_cells[i].getCenter().y));
					if (copieCell.checkCollision(m_cells[j]))
					{
						moveX = 0;
					}
				
					copieCell.setCenter(sf::Vector2f(m_cells[i].getCenter().x, m_cells[i].getCenter().y + moveY));
					if (copieCell.checkCollision(m_cells[j]))
					{
						moveY = 0;
					}
				}
			}
			
			m_cells[i].setCenter(sf::Vector2f(m_cells[i].getCenter().x + moveX, m_cells[i].getCenter().y + moveY));
		}
	}
}

void Player::split()
{
	float min_size = m_cells[0].getSize();
	int i=0;
	while (i < m_cells.size() && min_size < 10)
	{
		if (m_cells[i].getSize() < min_size)
		{
			min_size = m_cells[i].getSize();
			++i;
		}
	}
	int nb_cells = m_cells.size();
	if (min_size > 10)
	{
		for (int j = 0; j < nb_cells; ++j)
		{
			this->addCell(m_cells[j].split(m_target));
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

void Player::delCell(Cell cell)
{
}

void Player::setTarget(sf::Vector2f target)
{
	m_target = target;
}

sf::Vector2f Player::getViewCenter()
{
	return sf::Vector2f(((m_cells_zone[0] + m_cells_zone[1]) / 2), ((m_cells_zone[2] + m_cells_zone[3]) / 2));
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

void Player::checkCollision(std::vector<Food>* ptrlFood)
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		m_cells[i].checkCollision(m_cells[0]);
	}
}
