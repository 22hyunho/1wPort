#include "stdafx.h"
#include "piccolo.h"

HRESULT piccolo::init(POINT position, float hp)
{
	enemy::init(position, hp);
	IMAGEMANAGER->addImage("피콜로", "image/piccoloFront.bmp", 117, 106, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("피콜로공격", "image/piccoloAttack.bmp", 328, 109, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("피콜로다운", "image/piccoloDie.bmp", 677, 95, 5, 1, true, RGB(255, 0, 255));

	return S_OK;
}

void piccolo::release()
{
	enemy::release();
}

void piccolo::update()
{
	enemy::update();
	if (_currentState == P_FRONT)
	{
		_image = IMAGEMANAGER->findImage("피콜로");
		_rc = RectMakeCenter(_x, _y, _image->getWidth(), _image->getHeight());
	}
	if (_currentState == P_ATTACK)
	{
		_frameCount++;
		if (_frameCount % 5 == 0)
		{
			_currentFrameX++;
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			if (_currentFrameX > _image->getMaxFrameX()) _currentFrameX = _image->getMaxFrameX();
			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("피콜로공격");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == P_DIE)
	{
		_frameCount++;
		if (_frameCount % 5 == 0)
		{
			_dieFrameCount++;
			_image->setFrameX(_dieFrameCount);

			if (_dieFrameCount > _image->getMaxFrameX()) _dieFrameCount = _image->getMaxFrameX() + 1;

			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("피콜로다운");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
}

void piccolo::render()
{
	enemy::render();
	if (_currentState == P_FRONT) _image->render(getMemDC(), _rc.left, _rc.top);
	if (_currentState == P_ATTACK) _image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
	if (_currentState == P_DIE) _image->frameRender(getMemDC(), _rc.left, _rc.top, _dieFrameCount, 0);
}