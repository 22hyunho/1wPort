#include "stdafx.h"
#include "gameScreen.h"

HRESULT gameScreen::init()
{
	_goku = new player;
	_goku->init();

	_em = new enemyManager;
	_em->init();

	_goku->setEmLink(_em);
	_em->setGokuLink(_goku);
	return S_OK;
}

void gameScreen::release()
{
	SAFE_DELETE(_goku);
	SAFE_RELEASE(_goku);
	SAFE_DELETE(_em);
	SAFE_RELEASE(_em);
}

void gameScreen::update()
{
	_goku->update();
	_em->update();
}

void gameScreen::render()
{
	_em->render();
	_goku->render();
}
