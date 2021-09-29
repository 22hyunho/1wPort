#pragma once
#include "enemy.h"

enum krillin_State
{
	K_FRONT = 1,
	K_ATTACK,
	K_DIE
};

class krillin :
	public enemy
{
public :
	krillin() {};
	~krillin() {};

	virtual HRESULT init(POINT position, float hp);
	virtual void release();
	virtual void update();
	virtual void render();
};