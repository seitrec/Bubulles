#pragma once
#include <vector>
#include "Food.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
class Player;

class Cell :
	public Entity
	
{
public:
    Cell( Player* player, float size=10);
	~Cell();
	void drawName(sf::RenderWindow &window, sf::Font &font, std::string name);
	void drawScore(sf::RenderWindow &window, sf::Font &font);
    void Eat(Entity &prey, int splitTime=0);
	bool checkStrictCollision(Cell &cell);
	bool checkCollisionCovering(Entity &entity);
    void split(sf::Vector2f target, int splitTime);
    void setPlayer (Player *player);

private:
    Player* m_player;
};

