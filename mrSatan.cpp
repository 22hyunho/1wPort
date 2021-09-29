#include "stdafx.h"
#include "mrSatan.h"

HRESULT mrSatan::init(POINT position, float hp)
{
	enemy::init(position, hp);
	IMAGEMANAGER->addImage("»çÅº", "image/satanFront.bmp", 64, 115, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("»çÅº°ø°Ý", "image/satanAttack.bmp", 350, 132, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("»çÅº´Ù¿î", "image/satanDie.bmp", 565, 91, 5, 1, true, RGB(255, 0, 255));

	return S_OK;
}

void mrSatan::release()
{
	enemy::release();
}

void mrSatan::update()
{
	enemy::update();
	if (_currentState == SATAN_FRONT)
	{
		_image = IMAGEMANAGER->findImage("»çÅº");
		_rc = RectMakeCenter(_x, _y, _image->getWidth(), _image->getHeight());
	}
	if (_currentState == SATAN_ATTACK)
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
		_image = IMAGEMANAGER->findImage("»çÅº°ø°Ý");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
	if (_currentState == SATAN_DIE)
	{
		_frameCount++;
		if (_frameCount % 5 == 0)
		{
			_dieFrameCount++;
			_image->setFrameX(_dieFrameCount);

			if (_dieFrameCount > _image->getMaxFrameX()) _dieFrameCount = _image->getMaxFrameX() + 1;

			_frameCount = 0;
		}
		_image = IMAGEMANAGER->findImage("»çÅº´Ù¿î");
		_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	}
}

void mrSatan::render()
{
	enemy::render();
	if (_currentState == SATAN_FRONT) _image->render(getMemDC(), _rc.left, _rc.top);
	if (_currentState == SATAN_ATTACK) _image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
	if (_currentState == SATAN_DIE) _image->frameRender(getMemDC(), _rc.left, _rc.top, _dieFrameCount, 0);
}