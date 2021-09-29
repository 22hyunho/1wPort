#pragma once
#include "enemy.h"

enum PICCOLO_STATE
{
	P_FRONT = 1,
	P_ATTACK,
	P_DIE
};

class piccolo :
	public enemy
{
public:
	piccolo() {};
	~piccolo() {};

	virtual HRESULT init(POINT position, float hp);
	virtual void release();
	virtual void update();
	virtual void render();
};