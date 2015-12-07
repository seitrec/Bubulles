#pragma once
#include<vector>
#include "Food.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
class Player;

class Cell :
	public Entity
	
{
public:
	Cell(float size=10 , Player *playerptr=nullptr);
	~Cell();
	void drawName(sf::RenderWindow &window, sf::Font &font, std::string name);
	void drawScore(sf::RenderWindow &window, sf::Font &font);
    void Eat(Entity &prey);
	bool checkStrictCollision(Cell &cell);
	bool checkCollisionCovering(Entity &entity);
	void setMoved(bool b);
	bool getWasMoved();
    Cell split(sf::Vector2f target);

private:
	bool wasMoved;
    Player* m_player;
};

