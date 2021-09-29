#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//�ʱ�ȭ�� ����� �ϼ��� ����
HRESULT playGround::init()
{
	gameNode::init(true);

	SCENEMANAGER->addScene("��Ʈ��", new startScene);
	SCENEMANAGER->addScene("����ȭ��", new gameScreen);
	SCENEMANAGER->addScene("�¸�", new victoryScene);
	SCENEMANAGER->addScene("�й�", new gameoverScene);

	SCENEMANAGER->changeScene("��Ʈ��");

	return S_OK;
}

//�޸� ������ ����� �ϼ��� ����
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
	// ���� �ǵ�������
	//================���� �� ���̿� �� �׸��ô�==========================
	
	SCENEMANAGER->render();

	//==================================================
	//���⵵ �ǵ�������
	this->getBackBuffer()->render(getHDC(), 0, 0);
}
