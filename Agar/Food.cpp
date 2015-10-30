#include "Food.h"
#include "Cell.h"
#include "Global.h"
#include <SFML/Graphics.hpp>
using namespace std;


Food::Food(int size):Entity(size),m_buff("Normal")
{
	setCenter(sf::Vector2f((rand() % WORLD_SIZE + 1), (rand() % WORLD_SIZE + 1)));
    setColor(sf::Color(sf::Color::Red));//rand() % 256, rand() % 256, rand() % 256));
    randomizeBuff();
    if (m_buff=="Virus") {setColor(sf::Color::Green);}
}

Food::~Food()
{
}

//A implementer
void Food:: randomizeBuff()
{
    
}