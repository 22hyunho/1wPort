#include "stdafx.h"
#include "gameoverScene.h"

HRESULT gameoverScene::init()
{
    IMAGEMANAGER->addImage("게임오버", "image/gameOver.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
    return S_OK;
}

void gameoverScene::release()
{
}

void gameoverScene::update()
{
}

void gameoverScene::render()
{
    IMAGEMANAGER->findImage("게임오버")->render(getMemDC());
}
