#include "Cell.h"
#include "Global.h"
#include "Food.h"
#include <SFML/Graphics.hpp>
#include <math.h>

Cell::Cell()
{
	setPosition2(sf::Vector2f((rand() % worldSize + 1), (rand() % worldSize + 1)));
	setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	setSize(20);
	}

Cell::~Cell()
{
}

void Cell::split(sf::Vector2f target)
{
}

void Cell::eat(Entity entity) //melchior : je pense que Victor a raison, il faut faire cela dans la fonction checkcollision, auquel cas la fonction eat n'a plus de sens.
{
   // entity.getEaten(*this);
}

void Cell::getEaten(Entity entity)
{}

void Cell::eject()
{
}

void Cell::checkCollision(std::vector<Food>* ptrlFood) //ne faudrait-il mieux pas utiliser const& ptrlFood ? on passe l'adresse du vecteur, et on le laisse constant vu qu'on ne va pas le modifier, juste le parcourir
{
	for (unsigned i = 0; i < ptrlFood->size(); ++i) //mieux vaut faire a avec un iterateur, pas un int i. cf. http://pastebin.com/NP5eyVNZ
	{
		if (sqrt(pow((this->getPosition().x - ptrlFood->at(i).getPosition().x), 2) +
			pow((this->getPosition().y - ptrlFood->at(i).getPosition().y), 2))
			< fabs(this->getSize() - ptrlFood->at(i).getSize() / 2))
		{
			//TODO rajouter un argument à get eaten
			ptrlFood->at(i).getEaten();
			ptrlFood->erase(ptrlFood->begin()+i);
		}
	}

}