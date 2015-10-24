#pragma once
#include "Entity.h"

class Food :
	public Entity
{
public:
	Food();
	~Food();
    void getEaten(Entity cell);
};

