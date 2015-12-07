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
    if (m_buff=="Virus")
    {
        setColor(sf::Color::Green);
        //other parameters that are unique to the virus
    }
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
// This is a buff name generation method that is called by the constructor to initiate each food.
// The vast majority of foods should be basic food units, but, for more game functionality, we can devise different "buffs" that the food units can contain. Examples: a speed buff that lasts for a certain duration, an invincibility buff, etc... Imagination of the game's designers is the limit here.
// For now, only "viruses" are implemented : foods that, if consumed, split the player into 10 small cells. They should be avoided at all costs by players.
{
    float dice = ((float) rand()) / (float) RAND_MAX;
    if (dice>=0.995)
    {
        m_buff="Virus";
    }
}