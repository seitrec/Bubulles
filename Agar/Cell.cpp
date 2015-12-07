#include "Cell.h"
#include "Global.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <math.h>


Cell::Cell(float size):Entity(size)
// This class implements a subClass of Entity (controlled by a player/IA) capable of moving, splitting, and eating
// param size (float): the radius of the disc
// return null
{	
	int r = rand() % 256;
	int v = rand() % 256;
	int b = rand() % 256;
	setSpeed(m_size);
	setCenter(sf::Vector2f(rand() % WORLD_SIZE, rand() % WORLD_SIZE));
    setColor(sf::Color(r,v,b));
	setOutlineColor(sf::Color(fmax(0, r - 40), fmax(0, v - 40), fmax(0, b - 40	)));
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


void Cell::Eat(Entity &prey)
// Method to call when this cell eats by an entity called prey
// param prey (Entity): the Entity that is eaten by this
// return null
{
    // Conditional buff action list (for example : if the food gives a speed bonus "buff", 
	// or if the cell we attempt to eat is currently immune for a short while). For more information on buffs, see the Food class.
    if (prey.getBuff()=="Virus")
    {
        for (int i=0; i<10; i++)
        {
			float theta = i * 3.14 / 5;
            float direction_x = this->getCenter().x + this->getSize()*cos(theta);
            float direction_y = this->getCenter().y + this->getSize()*sin(theta);
            sf::Vector2f target(direction_x, direction_y);
            this->split(target);
        }
    }
    if (prey.getBuff()=="Speed")
    {
        m_buff="Faster";
    }
    else //default case
    {
        this->setSize(sqrt(pow(m_size, 2) + pow(prey.getSize(),2)));
        this->setSpeed(this->getSize());
    }
}


void Cell::split(sf::Vector2f target)
// Split an Cell in two, by dividing this entity's mass by 2, and creating a clone to the target location
// param target (Vector2f): target direction where to create the clone
// return child (Cell): Clone of the reduced cell, that appears close to it towards target location
{
    this->setSize(this->getSize()/sqrt(2));
    this->setSpeed(this->m_size);
    Cell child(this->getSize());
    Cell * ptrChild = &child;
    ptrChild->setColor(this->getColor());
    ptrChild->setOutlineColor(this->getOutlineColor());
    sf::Vector2f move = ptrChild->move(target);
    ptrChild->setCenter(sf::Vector2f(this->getCenter().x + move.x, this->getCenter().y + move.y));
    m_player->addCell(ptrChild);
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

void Cell::setPlayer(Player *player)
{
    m_player=player;
}
