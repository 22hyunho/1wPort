#include "stdafx.h"
#include "startScene.h"

HRESULT startScene::init()
{
	IMAGEMANAGER->addImage("��Ʈ��", "image/intro.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��ŸƮ", "image/start.bmp", 123, 34, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("����", "image/exit.bmp", 205, 34, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���Ϲڽ�", "image/boxLB.bmp", 15, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�»�ڽ�", "image/boxLT.bmp", 15, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���Ϲڽ�", "image/boxRB.bmp", 15, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���ڽ�", "image/boxRT.bmp", 15, 15, true, RGB(255, 0, 255));
	
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
		if (_boxLX == WINSIZEX - 240) SCENEMANAGER->changeScene("����ȭ��");
		if (_boxLX == WINSIZEX - 285) PostQuitMessage(0);
	}
}

void startScene::render()
{
	IMAGEMANAGER->findImage("��Ʈ��")->render(getMemDC());
	IMAGEMANAGER->findImage("��ŸƮ")->render(getMemDC(), WINSIZEX - 230, WINSIZEY - 290);
	IMAGEMANAGER->findImage("����")->render(getMemDC(), WINSIZEX - 275, WINSIZEY - 200);
	IMAGEMANAGER->findImage("���Ϲڽ�")->render(getMemDC(), _boxLX, _boxLY);
	IMAGEMANAGER->findImage("�»�ڽ�")->render(getMemDC(), _boxLX, _boxLY - 40);
	IMAGEMANAGER->findImage("���Ϲڽ�")->render(getMemDC(), _boxRX, _boxRY);
	IMAGEMANAGER->findImage("���ڽ�")->render(getMemDC(), _boxRX, _boxRY - 40);
}
