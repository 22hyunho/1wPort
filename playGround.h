#pragma once
#include "gameNode.h"
#include "startScene.h"
#include "gameScreen.h"
#include "victoryScene.h"
#include "gameoverScene.h"
#include "player.h"
#include "enemyManager.h"

class playGround : public gameNode
{
private:
	
	
public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

