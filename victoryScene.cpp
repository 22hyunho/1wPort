#include "stdafx.h"
#include "victoryScene.h"

HRESULT victoryScene::init()
{
    IMAGEMANAGER->addImage("ºòÅä¸®", "image/youWin.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
    return S_OK;
}

void victoryScene::release()
{
}

void victoryScene::update()
{
}

void victoryScene::render()
{
    IMAGEMANAGER->findImage("ºòÅä¸®")->render(getMemDC());
}
