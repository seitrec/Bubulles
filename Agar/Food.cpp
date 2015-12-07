#include "Food.h"
#include "Cell.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace std;


Food::Food(int size):Entity(size)
// This class implements a subClass of Entity capable of holding a buff, but no movement nor size changes
// param size (float): the radius of the disc
// return null
{
	setCenter(sf::Vector2f((rand() % WORLD_SIZE + 1), (rand() % WORLD_SIZE + 1)));
    randomizeBuff();
    
    //Conditional buff initiation list (see randomizeBuff method for more info on buffs)
    if (m_buff=="Virus")
    {
        setColor(sf::Color::Green);
        setSize((VIRUS_MAX_SIZE - VIRUS_MIN_SIZE) * ((((float) rand()) / (float) RAND_MAX)) + VIRUS_MIN_SIZE ); //Sets virus size as a random between MAX_SIZE and MIN_SIZE
    }
    //else if (m_buff== <buff_name>)
    //etc
    else
    {
        setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
    }
    
}


Food::~Food()
// Destructor
{
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

