#include "Food.h"
#include "Cell.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
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
