#pragma once
#include "gameNode.h"
class gameoverScene : public gameNode
{
public :
	gameoverScene() {};
	~gameoverScene() {};

	HRESULT init();
	void release();
	void update();
	void render();
};

