#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화는 여기다 하세요 제발
HRESULT playGround::init()
{
	gameNode::init(true);

	SCENEMANAGER->addScene("인트로", new startScene);
	SCENEMANAGER->addScene("게임화면", new gameScreen);
	SCENEMANAGER->addScene("승리", new victoryScene);
	SCENEMANAGER->addScene("패배", new gameoverScene);

	SCENEMANAGER->changeScene("인트로");

	return S_OK;
}

//메모리 해제는 여기다 하세요 제발
void playGround::release()
{
	gameNode::release();
}


void playGround::update()
{
	gameNode::update();

	SCENEMANAGER->update();
}


void playGround::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	// 위에 건들지마라
	//================제발 이 사이에 좀 그립시다==========================
	
	SCENEMANAGER->render();

	//==================================================
	//여기도 건들지마라
	this->getBackBuffer()->render(getHDC(), 0, 0);
}
