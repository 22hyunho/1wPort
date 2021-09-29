#include "stdafx.h"
#include "explosion.h"

explosion::explosion()
{
}

explosion::~explosion()
{
}

HRESULT explosion::init()
{
	EFFECTMANAGER->addEffect("Æø¹ß", "image/explosion.bmp", 1908, 72, 106, 72, 1, 0.7f, 10);
	return S_OK;
}

void explosion::release()
{
}

void explosion::update(float x, float y)
{
	EFFECTMANAGER->play("Æø¹ß", x - 100 + RND->getFromIntTo(0, 200), y - 50 + RND->getFromIntTo(0, 200));
	EFFECTMANAGER->update();
}

void explosion::render()
{
	EFFECTMANAGER->render();
}
