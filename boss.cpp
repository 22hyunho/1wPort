#include "stdafx.h"
#include "boss.h"

HRESULT boss::init(POINT position, float hp)
{
	enemy::init(position, hp);
	IMAGEMANAGER->addFrameImage("���κο�", "boss/buuIdle.bmp", 270, 117, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�ο����", "boss/buuAttack1.bmp", 550, 124, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�ο�ٶ�", "boss/buuAirAttack.bmp", 824, 116, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�ο�����", "boss/buuChoco.bmp", 696, 96, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�ο��", "boss/buuRoll.bmp", 702, 123, 6, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�ο�ٿ�", "boss/buuDie.bmp", 416, 118, 4, 1, true, RGB(255, 0, 255));

	_hpBar = new progressBar();
	_hpBar->init(_rc.left + 10, _rc.top - 20, 100, 15);
	_hpBar->setGauge(_currentHP, _maxHP);
	return S_OK;
}

void boss::release()
{
	enemy::release();
}

void boss::update()
{
	enemy::update();
	if (_currentState == M_FRONT)
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
		_image = IMAGEMANAGER->findImage("���κο�");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == M_ATTACK)
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
		_image = IMAGEMANAGER->findImage("�ο����");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == M_AIRATTACK)
	{
		_frameCount++;
		if (_frameCount % 10 == 0)
		{
			_currentFrameX++;
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			if (_currentFrameX > _image->getMaxFrameX()) _currentFrameX = 0;
			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("�ο�ٶ�");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == M_CHOCOATTACK)
	{
		_frameCount++;
		if (_frameCount % 10 == 0)
		{
			_currentFrameX++;
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			if (_currentFrameX > _image->getMaxFrameX()) _currentFrameX = 0;
			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("�ο�����");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == M_ROLLATTACK)
	{
		_frameCount++;
		if (_frameCount % 7 == 0)
		{
			_currentFrameX++;
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			if (_currentFrameX > _image->getMaxFrameX() - 1) _currentFrameX = 2;
			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("�ο��");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == M_DIE)
	{
		_frameCount++;
		_dieFrameCount2++;
		if (_frameCount % 10 == 0)
		{
			
			_image->setFrameX(_dieFrameCount);

			if (_dieFrameCount2 < 200) _dieFrameCount = 0;
			else _dieFrameCount++;
			if (_dieFrameCount > _image->getMaxFrameX()) _dieFrameCount = _image->getMaxFrameX() + 1;

			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("�ο�ٿ�");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}

	_hpBar->setX(_rc.left + 10);
	_hpBar->setY(_rc.top - 20);
	_hpBar->setGauge(_currentHP, _maxHP);
	_hpBar->update();
}

void boss::render()
{
	enemy::render();
	if (_currentState == M_FRONT || _currentState == M_ATTACK || _currentState == M_AIRATTACK || 
		_currentState == M_CHOCOATTACK || _currentState == M_ROLLATTACK)
	{
		_image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
	}
	if (_currentState == M_DIE) _image->frameRender(getMemDC(), _rc.left, _rc.top, _dieFrameCount, 0);

	_hpBar->render();
}
