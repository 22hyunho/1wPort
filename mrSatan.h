#pragma once
#include "enemy.h"

enum SATAN_State
{
	SATAN_FRONT = 1,
	SATAN_ATTACK,
	SATAN_DIE
};

class mrSatan :
    public enemy
{
public :
	mrSatan() {};
    ~mrSatan() {};

	virtual HRESULT init(POINT position, float hp);
	virtual void release();
	virtual void update();
	virtual void render();
};