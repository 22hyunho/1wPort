#pragma once
#include "gameNode.h"

class enemyManager;

class respawn : public gameNode
{
private :
	enemyManager* _em;
	int _delayCount1;
	int _delayCount2;
	int _delayCount3;

	int _fireCount;
	int _fireNum;

public :
	respawn() {};
	~respawn() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	bool delayPattern1();
	bool delayPattern2();
	bool delayPattern3();
};

