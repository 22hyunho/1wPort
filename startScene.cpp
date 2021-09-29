#include "stdafx.h"
#include "startScene.h"

HRESULT startScene::init()
{
	IMAGEMANAGER->addImage("인트로", "image/intro.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스타트", "image/start.bmp", 123, 34, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("종료", "image/exit.bmp", 205, 34, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("좌하박스", "image/boxLB.bmp", 15, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("좌상박스", "image/boxLT.bmp", 15, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("우하박스", "image/boxRB.bmp", 15, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("우상박스", "image/boxRT.bmp", 15, 15, true, RGB(255, 0, 255));
	
	_boxLX = WINSIZEX - 240;
	_boxLY = WINSIZEY - 260;
	_boxRX = WINSIZEX - 110;
	_boxRY = WINSIZEY - 260;
	
	
	return S_OK;
}

void startScene::release()
{
}

void startScene::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		_boxLX = WINSIZEX - 240;
		_boxLY = WINSIZEY - 260;
		_boxRX = WINSIZEX - 110;
		_boxRY = WINSIZEY - 260;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		_boxLX = WINSIZEX - 285;
		_boxLY = WINSIZEY - 170;
		_boxRX = WINSIZEX - 70;
		_boxRY = WINSIZEY - 170;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		if (_boxLX == WINSIZEX - 240) SCENEMANAGER->changeScene("게임화면");
		if (_boxLX == WINSIZEX - 285) PostQuitMessage(0);
	}
}

void startScene::render()
{
	IMAGEMANAGER->findImage("인트로")->render(getMemDC());
	IMAGEMANAGER->findImage("스타트")->render(getMemDC(), WINSIZEX - 230, WINSIZEY - 290);
	IMAGEMANAGER->findImage("종료")->render(getMemDC(), WINSIZEX - 275, WINSIZEY - 200);
	IMAGEMANAGER->findImage("좌하박스")->render(getMemDC(), _boxLX, _boxLY);
	IMAGEMANAGER->findImage("좌상박스")->render(getMemDC(), _boxLX, _boxLY - 40);
	IMAGEMANAGER->findImage("우하박스")->render(getMemDC(), _boxRX, _boxRY);
	IMAGEMANAGER->findImage("우상박스")->render(getMemDC(), _boxRX, _boxRY - 40);
}
