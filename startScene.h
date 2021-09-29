#pragma once
#include "gameNode.h"
class startScene : public gameNode
{
private :

	float _boxLX, _boxLY;
	float _boxRX, _boxRY;
public :
	startScene() {};
	~startScene() {};

	HRESULT init();
	void release();
	void update();
	void render();
};

