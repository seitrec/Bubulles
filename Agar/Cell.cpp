#include "Cell.h"
#include "Global.h"
#include "Food.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <math.h>

Cell::Cell()
{
	int r = rand() % 256;
	int v = rand() % 256;
	int b = rand() % 256;
	//SetSize avant SetCenter sinon impossible de d�finir le centre....
	setSize(30);
	setSpeed(m_size);
	setCenter(sf::Vector2f(rand() % worldSize, rand() % worldSize));
	setColor(sf::Color(r,v,b));
	setOutlineColor(sf::Color(fmax(0, r - 40), fmax(0, v - 40), fmax(0, b - 40)));
	setOutlineThickness(getSize()/8);
	setPointCount(100);
}

Cell::~Cell()
{
}

float Cell::getSpeed()
{
	return m_speed;
}

Cell Cell::split(sf::Vector2f target)
{
	this->setSize(this->getSize()/sqrt(2));
	Cell child;
	child.setSize(this->getSize());
	child.setColor(this->getColor());
	child.setOutlineColor(this->getOutlineColor());
	float relativeCenterMouseX;
	relativeCenterMouseX = target.x - (this->getCenter().x + m_size);
	float relativeCenterMouseY;
	relativeCenterMouseY = target.y - (this->getCenter().y + m_size);
	if (fabs(relativeCenterMouseX) > 3 || fabs(relativeCenterMouseY) > 3)
	{
		float c = sqrt(((m_speed + 3 * m_size) * (m_speed + 3 * m_size)) / (relativeCenterMouseX * relativeCenterMouseX + relativeCenterMouseY * relativeCenterMouseY));
		child.setCenter(sf::Vector2f (this->getCenter().x + c * relativeCenterMouseX, this->getCenter().y + c * relativeCenterMouseY));
	}

	return child;
}

void Cell::eat(Entity &entity) 
{
	m_size = sqrt(pow(m_size, 2) + pow(entity.getSize(),2));
	this->setSize(m_size);
	this->setSpeed(m_size);
}

void Cell::setSpeed(float size)
{
	m_speed = 200 / size;
}

void Cell::drawName(sf::RenderWindow & window, sf::Font & font, std::string name)
{
	sf::Text text;
	text.setFont(font); // font est un sf::Font
	text.setCharacterSize(this->getSize()/2); // exprimée en pixels, pas en points !
	text.setColor(sf::Color::White);
	text.setString(name);
	text.setPosition(this->getCenter()-sf::Vector2f(m_size/2,m_size/2));
	window.draw(text);
}

void Cell::eject()
{
}


bool Cell::checkCollision(Entity &entity) //ne faudrait-il mieux pas utiliser const& ptrlFood ? on passe l'adresse du vecteur, et on le laisse constant vu qu'on ne va pas le modifier, juste le parcourir
{
	return (sqrt(pow((this->getCenter().x - entity.getCenter().x), 2) +
		pow((this->getCenter().y - entity.getCenter().y), 2))
		< fabs(this->getSize() + entity.getSize()));
}
/*
void Cell::actionCollision(Entity & entity, Player & player, std::vector<Food>& lFood, std::vector<Player>& lPlayer)
{
	float difEatMin = 1.1;
	if (this->getSize() >= entity.getSize()*difEatMin)
	{
		entity.getEaten(*this, player, lPlayer);

	}
	else
	{
		//Faire qqch de graphique
	}
}
*/