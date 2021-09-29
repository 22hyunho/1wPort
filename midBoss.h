#pragma once
#include "enemy.h"

enum frieza_State
{
	F_FRONT = 1,
	F_ATTACK,
	F_DIE
};

class midBoss : 
	public enemy
{
public:
	midBoss() {};
	~midBoss() {};

	virtual HRESULT init(POINT position, float hp);
	virtual void release();
	virtual void update();
	virtual void render();

	progressBar* _hpBar;
};

