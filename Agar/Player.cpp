#include "Player.h"
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
		m_cells[i].move(m_target);
	}
}

void Player::split()
{
}

void Player::eject()
{
}

std::vector<Cell> Player::getCells()
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
	//TO DO faire un truc intelligent (le barycentre des cellules et du poids ?)
	return m_cells[0].getCenter();
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
