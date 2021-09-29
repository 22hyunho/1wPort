#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"
class gameScreen : public gameNode
{
private :
	player* _goku;
	enemyManager* _em;

public :
	gameScreen() {};
	~gameScreen() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};