#include "Food.h"
#include "Cell.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace std;


Food::Food()
{
	setSize(5);
	setCenter(sf::Vector2f((rand() % worldSize + 1), (rand() % worldSize + 1)));
	setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
}

Food::~Food()
{
}

void Food::getEaten(Entity& predator)
{
    predator.setSize(sqrt(pow(m_size, 2) + pow(predator.getSize(),2)));
    //liste conditionnelle des autres buffs (dont virus) ici.
        //if (m_buff == <nomdebuff>) ...
}