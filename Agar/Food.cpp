#include "Food.h"
#include "Cell.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace std;


Food::Food(int size):Entity(size),m_buff("Normal")
// This class implements a subClass of Entity capable of holding a buff, but no movement nor size changes
// param size (float): the radius of the disc
// return null
{
	setCenter(sf::Vector2f((rand() % WORLD_SIZE + 1), (rand() % WORLD_SIZE + 1)));
    setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
    randomizeBuff();
    if (m_buff=="Virus") {setColor(sf::Color::Green);}
}


Food::~Food()
// Destructor
{
}


void Food::getEaten(Entity& predator)
// Method to call when the this food is eaten by another entity predator
// param predator (Entity): the Entity (likely a Cell) that eats this entity
// return null
{
    predator.setSpeed(predator.getSize());
    predator.setSize(sqrt(pow(m_size, 2) + pow(predator.getSize(),2)));
    // Conditional buff action list
        //if (m_buff == <buff_name>) ...
}


void Food:: randomizeBuff()
// Please build this doc while implementing the method
{
    
}
