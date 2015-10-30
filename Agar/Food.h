#pragma once
#include "Entity.h"


class Food :
	public Entity
{
public:
    Food(int size=5);
	~Food();
    void randomizeBuff ();
private:
    std::string m_buff;
    
};

