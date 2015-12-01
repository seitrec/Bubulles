#include "Food.h"
#include "Cell.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace std;


Food::Food(int size):Entity(size),m_buff("Normal")
{
	setCenter(sf::Vector2f((rand() % WORLD_SIZE + 1), (rand() % WORLD_SIZE + 1)));
    setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
    randomizeBuff();
    if (m_buff=="Virus") {setColor(sf::Color::Green);}
}

Food::~Food()
{
}

void Food::getEaten(Entity& predator)
{
    predator.setSpeed(predator.getSize());
    predator.setSize(sqrt(pow(m_size, 2) + pow(predator.getSize(),2)));
    //liste conditionnelle des autres buffs (dont virus) ici.
        //if (m_buff == <nomdebuff>) ...
}
//A implementer
void Food:: randomizeBuff()
{
    
}
