#include "Food.h"
#include "Cell.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
using namespace std;


Food::Food()
{
	setPosition2(sf::Vector2f((rand() % worldSize + 1), (rand() % worldSize + 1)));
	setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	setSize(5);
}

Food::~Food()
{
}
