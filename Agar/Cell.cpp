#include "Cell.h"
#include "Global.h"
//#include "Food.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <math.h>

Cell::Cell(int size):Entity(size), wasMoved(0)
{
	int r = rand() % 256;
	int v = rand() % 256;
	int b = rand() % 256;
	setSpeed(m_size);
	setCenter(sf::Vector2f(rand() % WORLD_SIZE, rand() % WORLD_SIZE));
    setColor(sf::Color(r,v,b));
	setOutlineColor(sf::Color(fmax(0, r - 40), fmax(0, v - 40), fmax(0, b - 40)));
	setOutlineThickness(getSize()/8);
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
	child.setSpeed(m_size);
	child.setColor(this->getColor());
	child.setOutlineColor(this->getOutlineColor());
	sf::Vector2f move = child.move(target);
	child.setCenter(sf::Vector2f(this->getCenter().x + move.x, this->getCenter().y + move.y));
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
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
	textRect.top + textRect.height / 2.0f);
	text.setPosition(this->getCenter());
	window.draw(text);
}

void Cell::drawScore(sf::RenderWindow & window, sf::Font & font)
{
	sf::Text text;
	text.setFont(font); // font est un sf::Font
	text.setCharacterSize(this->getSize() / 3); // exprimée en pixels, pas en points !
	text.setColor(sf::Color::White);
	text.setString(std::to_string(static_cast<int>(round(this->getSize()))));
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
	text.setPosition(this->getCenter().x, this->getCenter().y+m_size/2);
	window.draw(text);
}

void Cell::eject()
{
}


bool Cell::checkCollision(Food &food) //ne faudrait-il mieux pas utiliser const& ptrlFood ? on passe l'adresse du vecteur, et on le laisse constant vu qu'on ne va pas le modifier, juste le parcourir
{
	return (sqrt(pow((this->getCenter().x - food.getCenter().x), 2) +
		pow((this->getCenter().y - food.getCenter().y), 2))
		< fabs((this->getSize() + food.getSize())));
}
void Cell::setMoved(bool b)
{
	wasMoved = b;
}
bool Cell::getWasMoved()
{
	return wasMoved;
}
bool Cell::checkCollision(Cell &cell) //ne faudrait-il mieux pas utiliser const& ptrlFood ? on passe l'adresse du vecteur, et on le laisse constant vu qu'on ne va pas le modifier, juste le parcourir
{
	return (sqrt(pow((this->getCenter().x - cell.getCenter().x), 2) +
		pow((this->getCenter().y - cell.getCenter().y), 2))
		< fabs(this->getSize() - cell.getSize() / 2));
}
bool Cell::checkCollisionMyCells(Cell &cell) //ne faudrait-il mieux pas utiliser const& ptrlFood ? on passe l'adresse du vecteur, et on le laisse constant vu qu'on ne va pas le modifier, juste le parcourir
{
	return (sqrt(pow((this->getCenter().x - cell.getCenter().x), 2) +
		pow((this->getCenter().y - cell.getCenter().y), 2))
		< fabs(this->getSize() + cell.getSize()));
}