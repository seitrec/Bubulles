#pragma once
#include "Entity.h"

class Food :
	public Entity
{
public:
	Food();
	~Food();
    virtual void getEaten(Entity& predator);
private:
    std::string m_buff;
};

