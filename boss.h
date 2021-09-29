#pragma once
#include "enemy.h"

enum majinBuu_State
{
	M_FRONT = 1,
	M_ATTACK,
	M_DIE,
	M_AIRATTACK,
	M_CHOCOATTACK,
	M_ROLLATTACK
};

class boss :
	public enemy
{
public:
	boss() {};
	~boss() {};

	virtual HRESULT init(POINT position, float hp);
	virtual void release();
	virtual void update();
	virtual void render();

	progressBar* _hpBar;
};