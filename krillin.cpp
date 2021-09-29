#include "stdafx.h"
#include "krillin.h"

HRESULT krillin::init(POINT position, float hp)
{
	enemy::init(position, hp);
	IMAGEMANAGER->addImage("农府傅", "image/krillinFront.bmp", 101, 98, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("农府傅傍拜", "image/krillinAttack.bmp", 440, 98, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("农府傅促款", "image/krillinDie.bmp", 460, 109, 4, 1, true, RGB(255, 0, 255));

	return S_OK;
}

void krillin::release()
{
	enemy::release();
}

void krillin::update()
{
	enemy::update();
	if (_currentState == K_FRONT)
	{
		_image = IMAGEMANAGER->findImage("农府傅");
		_rc = RectMakeCenter(_x, _y, _image->getWidth(), _image->getHeight());
	}
	if (_currentState == K_ATTACK)
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
		_image = IMAGEMANAGER->findImage("农府傅傍拜");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == K_DIE)
	{
		_frameCount++;
		if (_frameCount % 5 == 0)
		{
			_dieFrameCount++;
			_image->setFrameX(_dieFrameCount);

			if (_dieFrameCount > _image->getMaxFrameX()) _dieFrameCount = _image->getMaxFrameX() + 1;

			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("农府傅促款");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
}

void krillin::render()
{
	enemy::render();
	if (_currentState == K_FRONT) _image->render(getMemDC(), _rc.left, _rc.top);
	if (_currentState == K_ATTACK) _image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
	if (_currentState == K_DIE) _image->frameRender(getMemDC(), _rc.left, _rc.top, _dieFrameCount, 0);
}