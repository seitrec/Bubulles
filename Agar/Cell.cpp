#include "Cell.h"
#include "Global.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <math.h>


Cell::Cell(float size):Entity(size), wasMoved(0)
// This class implements an subClass of Entity (controlled by a player/IA) capable of moving, splitting, and eating
// param size (float): the radius of the disc
// return null
{	
	int r = rand() % 256;
	int v = rand() % 256;
	int b = rand() % 256;
	setSpeed(m_size);
	setCenter(sf::Vector2f(rand() % WORLD_SIZE, rand() % WORLD_SIZE));
    setColor(sf::Color(r,v,b));
	setOutlineColor(sf::Color(fmax(0, r - 40), fmax(0, v - 40), fmax(0, b - 40)));
	setOutlineThickness(size/8);
}


Cell::~Cell()
// Destructor
{
}


void Cell::drawName(sf::RenderWindow & window, sf::Font & font, std::string name)
// Draw this cell's name on itself
// param window (RenderWindow): window in which the cell is drawn
// param font (Font): Font used to draw the name
// param name (String): the name to draw
// return null
{
	sf::Text text;
	text.setFont(font); // font est un sf::Font
	text.setCharacterSize(this->getSize() / 2); // exprimée en pixels, pas en points !
	text.setColor(sf::Color::White);
	text.setString(name);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(this->getCenter());
	window.draw(text);
}


void Cell::drawScore(sf::RenderWindow & window, sf::Font & font)
// Draw this cell's score (eq mass) on itself
// param window (RenderWindow): window in which the cell is drawn
// param font (Font): Font used to draw the score
// return null
{
	sf::Text text;
	text.setFont(font); // font est un sf::Font
	text.setCharacterSize(this->getSize() / 3); // exprimée en pixels, pas en points
	text.setColor(sf::Color::White);
	text.setString(std::to_string(static_cast<int>(round(this->getSize()))));
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(this->getCenter().x, this->getCenter().y + m_size / 2);
	window.draw(text);
}


void Cell::getEaten(Entity &predator)
// Method to call when the this entity is eaten by another entity predator
// param predator (Entity): the Entity (likely a Cell) that eats this entity
// return null
{
    predator.setSize(sqrt(pow(m_size, 2) + pow(predator.getSize(),2)));
    predator.setSpeed(predator.getSize());
}


Cell Cell::split(sf::Vector2f target)
// Split a Cell in two, by dividing this cell's mass by 2, and creating a clone to the target location
// param target (Vector2f): target direction where to create the clone
// return child (Cell): Clone of the reduced cell, that appears close to it towards target location
{
	
    this->setSize(this->getSize()/sqrt(2));
	this->setSpeed(this->m_size);
	Cell child;
	child.setSize(this->getSize());
	child.setSpeed(m_size);
	child.setColor(this->getColor());
	child.setOutlineColor(this->getOutlineColor());
	sf::Vector2f move = child.move(target);
	child.setCenter(sf::Vector2f(this->getCenter().x + move.x, this->getCenter().y + move.y));
	return child;
}


bool Cell::checkCollisionCovering(Entity &entity)
// Check if this Cell is covering the Entity entity enough to eat it (or to be eaten)
// A collision is considered covering as soon as the distance between center is lower than the biggest radius
// param entity (Entity): The entity to check the collision with
// return unnamed (bool): 1 if the collision is considered covering, 0 else
{
	return (sqrt(pow((this->getCenter().x - entity.getCenter().x), 2) +
		pow((this->getCenter().y - entity.getCenter().y), 2))
		< fabs(std::max(this->getSize(), entity.getSize())));
}


bool Cell::checkStrictCollision(Cell &cell)
// Check if any collision exists between this cell and another one (only used among a single player's cells)
// A collision exists if the distance between the two centers is lower than the sum of the radiuses
// param cell (Cell): The cell to check the collision with
// return unnamed (bool): 1 if a collision exists, 0 else
{
	return (sqrt(pow((this->getCenter().x - cell.getCenter().x), 2) +
		pow((this->getCenter().y - cell.getCenter().y), 2))
		< fabs(this->getSize() + cell.getSize()));
}


void Cell::setMoved(bool b)
// Setter of the boolean wasMoved, used to determine if this cell was moved this frame
// param b (bool): the value to set
// return null
{
	wasMoved = b;
}


bool Cell::getWasMoved()
// Getter of the boolean wasMoved, used to determine if this cell was moved this frame
// return wasMoved (bool): 1 if it was actually moved, 0 else
{
	return wasMoved;
}