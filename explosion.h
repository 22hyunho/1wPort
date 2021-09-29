#pragma once
#include "gameNode.h"
#include "effect.h"

class explosion : public gameNode
{
private:
	effect* _explosion;

public:
	explosion();
	~explosion();

	virtual HRESULT init();
	virtual void release();
	virtual void update(float x, float y);
	virtual void render();
};