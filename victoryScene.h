#pragma once
#include "gameNode.h"
class victoryScene : public gameNode
{
public:
	victoryScene() {};
	~victoryScene() {};

	HRESULT init();
	void release();
	void update();
	void render();
};

