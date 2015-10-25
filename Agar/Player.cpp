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
		std::cout << m_target.x << std::endl;
	}
}

void Player::split()
{
}

void Player::eject()
{
}

std::vector<Entity> Player::getCells()
{
	return std::vector<Entity>();
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
	return m_cells[0].getPosition();
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
		m_cells[i].checkCollision(ptrlFood);
	}
}