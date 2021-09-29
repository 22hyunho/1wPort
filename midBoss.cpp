#include "stdafx.h"
#include "midBoss.h"

HRESULT midBoss::init(POINT position, float hp)
{
	enemy::init(position, hp);
	IMAGEMANAGER->addFrameImage("������", "image/frieza.bmp", 395, 94, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("����������", "image/friezaAttack.bmp", 420, 110, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�������ٿ�", "image/friezaDie.bmp", 600, 83, 6, 1, true, RGB(255, 0, 255));

	_hpBar = new progressBar();
	_hpBar->init(_rc.left + 10, _rc.top - 20, 80, 15);
	_hpBar->setGauge(_currentHP, _maxHP);
	return S_OK;
}

void midBoss::release()
{
	enemy::release();
}

void midBoss::update()
{
	enemy::update();
	if (_currentState == F_FRONT)
	{
		_frameCount++;
		if (_frameCount % 7 == 0)
		{
			_currentFrameX++;
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			if (_currentFrameX > _image->getMaxFrameX()) _currentFrameX = 0;
			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("������");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	//if (fireDelay2()) _currentFrameX = 0; //���ڽ� ������ ���� ������ �˻��ؼ� ���¼����Ҷ� ����..
	if (_currentState == F_ATTACK)
	{
		_frameCount++;
		if (_frameCount % 7 == 0)
		{
			_currentFrameX++;
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			if (_currentFrameX > _image->getMaxFrameX()) _currentFrameX = 0;
			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("����������");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == F_DIE)
	{
		_frameCount++;
		if (_frameCount % 9 == 0)
		{
			_dieFrameCount++;
			_image->setFrameX(_dieFrameCount);

			if (_dieFrameCount > _image->getMaxFrameX()) _dieFrameCount = _image->getMaxFrameX() + 1;

			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("�������ٿ�");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}

	_hpBar->setX(_rc.left + 10);
	_hpBar->setY(_rc.top - 20);
	_hpBar->setGauge(_currentHP, _maxHP);
	_hpBar->update();
}

void midBoss::render()
{
	enemy::render();
	if (_currentState == F_FRONT) _image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
	if (_currentState == F_ATTACK) _image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
	if (_currentState == F_DIE) _image->frameRender(getMemDC(), _rc.left, _rc.top, _dieFrameCount, 0);
	_hpBar->render();
}