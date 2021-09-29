#include "stdafx.h"
#include "bullets.h"

// =====================플레이어 탄======================
HRESULT energyBullet::init(float range, int bulletMax)
{
	_range = range;
	_bulletMax = bulletMax;
	return S_OK;
}

void energyBullet::release()
{
}

void energyBullet::update(int bulletLevel)
{
	move();
	_bulletLevel = bulletLevel;
}

void energyBullet::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->frameRender(getMemDC(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->image->getFrameX(), 0);

		_viBullet->count++;

		if (_viBullet->count % 10 == 0)
		{
			_viBullet->image->setFrameX(_viBullet->image->getFrameX() + 1);

			if (_viBullet->image->getFrameX() >= _viBullet->image->getMaxFrameX())
			{
				_viBullet->image->setFrameX(0);
			}

			_viBullet->count = 0;
		}
	}
}

void energyBullet::fire(float x, float y, float angle)
{
	if (_bulletMax < _vBullet.size()) return;
	tagBullet egBullet;
	ZeroMemory(&egBullet, sizeof(tagBullet));

	egBullet.image = new image;
	if (_bulletLevel <= 1)
	{
		egBullet.image->init("image/gokuBullet.bmp", 0, 0, 72, 13, 3, 1,
							true, RGB(255, 0, 255));
	}
	if (_bulletLevel > 1)
	{
		egBullet.image->init("image/gokuBullet2.bmp", 0, 0, 120, 14, 3, 1,
							true, RGB(255, 0, 255));
	}
	egBullet.x = egBullet.fireX = x;
	egBullet.y = egBullet.fireY = y;
	egBullet.speed = 6.0f;
	egBullet.angle = angle;
	egBullet.rc = RectMakeCenter(egBullet.x, egBullet.y, egBullet.image->getFrameWidth(), egBullet.image->getFrameHeight());

	_vBullet.push_back(egBullet);
}

void energyBullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getFrameWidth(), _viBullet->image->getFrameHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			SAFE_RELEASE(_viBullet->image);
			SAFE_DELETE(_viBullet->image);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void energyBullet::remove(int num)
{
	_vBullet[num].image->release();
	_vBullet.erase(_vBullet.begin() + num);
}

// =====================플레이어 스킬======================

HRESULT energyWave::init()
{
	_skillCount = 0;
	_skillFrameX = _skillFrameY = 0;
	return S_OK;
}

void energyWave::release()
{
}

void energyWave::update()
{
	move();
}

void energyWave::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->frameRender(getMemDC(),
			WINSIZEX / 4 + 20, WINSIZEY / 2 - 130,
			_viBullet->image->getFrameX(), _skillFrameY);

		_viBullet->count++;
		_skillCount++;

		if (_skillCount > 30)
		{
			if (_viBullet->count % 9 == 0)
			{
				_skillFrameY++;
				_viBullet->image->setFrameY(_skillFrameY);

				if (_skillCount < 270)
				{
					if (_viBullet->image->getFrameY() > 9)
					{
						_skillFrameY = 6;
					}
				}
				else
				{
					if (_skillFrameY >= _viBullet->image->getMaxFrameY())
					{
						_skillFrameY = 13;
					}
				}

				_viBullet->count = 0;
			}
		}
	}
	for (int i = 0; i < _vBullet.size(); i++)
	{
		//Rectangle(getMemDC(), _vBullet[i].rc); //스킬 범위 렉트
	}
}

void energyWave::fire(float x, float y)
{
	if (1 < _vBullet.size()) return;
	tagBullet engWave;
	ZeroMemory(&engWave, sizeof(tagBullet));

	engWave.image = new image;
	engWave.image->init("image/energyWave1.bmp", 0, 0, 700, 3549, 1, 13,
		true, RGB(255, 0, 255));

	engWave.x = x;
	engWave.y = y;
	engWave.speed = 6.0f;
	engWave.rc = RectMakeCenter(engWave.x, engWave.y, WINSIZEX, WINSIZEY);

	_vBullet.push_back(engWave);
}

void energyWave::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->x < WINSIZEX / 3 * 2 - 170)
		{
			_viBullet->x += 6.3f;
		}
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, WINSIZEX, WINSIZEY);
	}
}

void energyWave::remove(int num)
{
	_vBullet[num].image->release();
	_vBullet.erase(_vBullet.begin() + num);
}


// =====================사탄 포탄======================

HRESULT satanMissile::init(float range, int bulletMax)
{
	IMAGEMANAGER->addImage("사탄포탄", "image/satanMissile.bmp", 29, 17, true, RGB(255, 0, 255));
	_range = range;
	_bulletMax = bulletMax;
	return S_OK;
}

void satanMissile::release()
{
}

void satanMissile::update()
{
	move();
}

void satanMissile::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->render(getMemDC(), _viBullet->rc.left, _viBullet->rc.top);
	}
}

void satanMissile::fire(float x, float y, float angle)
{
	if (_bulletMax < _vBullet.size()) return;
	tagBullet stMissile;
	ZeroMemory(&stMissile, sizeof(tagBullet));

	stMissile.image = IMAGEMANAGER->findImage("사탄포탄");
	stMissile.x = stMissile.fireX = x;
	stMissile.y = stMissile.fireY = y;
	stMissile.speed = 6.0f;
	stMissile.angle = angle;
	stMissile.rc = RectMakeCenter(stMissile.x, stMissile.y, stMissile.image->getWidth(), stMissile.image->getHeight());

	_vBullet.push_back(stMissile);
}

void satanMissile::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getWidth(), _viBullet->image->getHeight());


		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void satanMissile::remove(int num)
{
	_vBullet.erase(_vBullet.begin() + num);
}

// ===============에너미 기본 탄(크리링, 보스 공유)=================

HRESULT basicBullet::init(float range, int bulletMax)
{
	_range = range;
	_bulletMax = bulletMax;
	return S_OK;
}

void basicBullet::release()
{
}

void basicBullet::update()
{
	move();
}

void basicBullet::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->frameRender(getMemDC(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->image->getFrameX(), 0);

		_viBullet->count++;

		if (_viBullet->count % 5 == 0)
		{
			_viBullet->image->setFrameX(_viBullet->image->getFrameX() + 1);

			if (_viBullet->image->getFrameX() >= _viBullet->image->getMaxFrameX())
			{
				_viBullet->image->setFrameX(0);
			}

			_viBullet->count = 0;
		}
	}
}

void basicBullet::fire(float x, float y, float angle, float speed)
{
	if (_bulletMax < _vBullet.size()) return;
	tagBullet bBullet;
	ZeroMemory(&bBullet, sizeof(tagBullet));

	bBullet.image = new image;
	
	bBullet.image->init("image/basicBullet.bmp", 0, 0, 54, 18, 3, 1,
			true, RGB(255, 0, 255));
	
	bBullet.x = bBullet.fireX = x;
	bBullet.y = bBullet.fireY = y;
	bBullet.speed = speed;
	bBullet.angle = angle;
	bBullet.rc = RectMakeCenter(bBullet.x, bBullet.y, bBullet.image->getFrameWidth(), bBullet.image->getFrameHeight());

	_vBullet.push_back(bBullet);
}

void basicBullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getFrameWidth(), _viBullet->image->getFrameHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			SAFE_RELEASE(_viBullet->image);
			SAFE_DELETE(_viBullet->image);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void basicBullet::remove(int num)
{
	_vBullet[num].image->release();
	_vBullet.erase(_vBullet.begin() + num);
}

// =====================피콜로 빔=======================

HRESULT piccoloBeam::init(float range, int bulletMax)
{
	_range = range;
	_bulletMax = bulletMax;
	return S_OK;
}

void piccoloBeam::release()
{
}

void piccoloBeam::update(float gokuX, float gokuY)
{
	_gokuX = gokuX;
	_gokuY = gokuY;
	move();
}

void piccoloBeam::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->frameRender(getMemDC(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->image->getFrameX(), 0);

		_viBullet->count++;

		if (_viBullet->count % 5 == 0)
		{
			_viBullet->image->setFrameX(_viBullet->image->getFrameX() + 1);

			if (_viBullet->image->getFrameX() >= _viBullet->image->getMaxFrameX())
			{
				_viBullet->image->setFrameX(0);
			}

			_viBullet->count = 0;
		}
	}
}

void piccoloBeam::fire(float x, float y, float angle)
{
	if (_bulletMax < _vBullet.size()) return;
	tagBullet pBeam;
	ZeroMemory(&pBeam, sizeof(tagBullet));

	pBeam.image = new image;

	pBeam.image->init("image/piccoloBullet.bmp", 0, 0, 165, 16, 3, 1,
		true, RGB(255, 0, 255));

	pBeam.x = pBeam.fireX = x;
	pBeam.y = pBeam.fireY = y;
	pBeam.speed = 4.0f;
	pBeam.angle = angle;
	pBeam.rc = RectMakeCenter(pBeam.x, pBeam.y, pBeam.image->getFrameWidth(), pBeam.image->getFrameHeight());

	_vBullet.push_back(pBeam);
}

void piccoloBeam::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		if (_viBullet->x > _gokuX) _viBullet->y += -sinf(getAngle(_viBullet->x, _viBullet->y, _gokuX, _gokuY)) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getFrameWidth(), _viBullet->image->getFrameHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			SAFE_RELEASE(_viBullet->image);
			SAFE_DELETE(_viBullet->image);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void piccoloBeam::remove(int num)
{
	_vBullet[num].image->release();
	_vBullet.erase(_vBullet.begin() + num);
}

// =====================프리저 탄환=======================

HRESULT friezaBullet::init(float range, int bulletMax)
{
	_range = range;
	_bulletMax = bulletMax;
	return S_OK;
}

void friezaBullet::release()
{
}

void friezaBullet::update()
{
	move();
}

void friezaBullet::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->frameRender(getMemDC(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->image->getFrameX(), 0);

		_viBullet->count++;

		if (_viBullet->count % 5 == 0)
		{
			_viBullet->image->setFrameX(_viBullet->image->getFrameX() + 1);

			if (_viBullet->image->getFrameX() >= _viBullet->image->getMaxFrameX())
			{
				_viBullet->image->setFrameX(0);
			}

			_viBullet->count = 0;
		}
	}
}

void friezaBullet::fire(float x, float y, float angle)
{
	if (_bulletMax < _vBullet.size()) return;
	tagBullet violetBullet;
	ZeroMemory(&violetBullet, sizeof(tagBullet));

	violetBullet.image = new image;

	violetBullet.image->init("image/friezaBullet1.bmp", 0, 0, 79, 28, 4, 1,
		true, RGB(255, 0, 255));

	violetBullet.x = violetBullet.fireX = x;
	violetBullet.y = violetBullet.fireY = y;
	violetBullet.speed = 3.0f;
	violetBullet.angle = angle;
	_angle = violetBullet.angle;
	violetBullet.index = 0;
	violetBullet.rc = RectMakeCenter(violetBullet.x, violetBullet.y, violetBullet.image->getFrameWidth(), violetBullet.image->getFrameHeight());

	_vBullet.push_back(violetBullet);
}

void friezaBullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		RECT violet = _viBullet->rc;
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		if (violet.left < 0)
		{
			_viBullet->angle = PI - _viBullet->angle;
			_viBullet->x = (violet.right - violet.left) / 2;
		}
		if (violet.right > WINSIZEX)
		{
			_viBullet->angle = PI - _viBullet->angle;
			_viBullet->x = WINSIZEX - (violet.right - violet.left) / 2;
		}
		if (violet.top < 0)
		{
			_viBullet->angle = PI * 2 - _viBullet->angle;
			_viBullet->y = (violet.bottom - violet.top) / 2;
		}
		if (violet.bottom > WINSIZEY)
		{
			_viBullet->angle = PI * 2 - _viBullet->angle;
			_viBullet->y = WINSIZEY - (violet.bottom - violet.top) / 2;
		}

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getFrameWidth(), _viBullet->image->getFrameHeight());

		if (violet.left < 0 || violet.right > WINSIZEX || violet.top < 0 || violet.bottom > WINSIZEY) _viBullet->index++;

		if (_viBullet->index > 4)
		{
			SAFE_RELEASE(_viBullet->image);
			SAFE_DELETE(_viBullet->image);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void friezaBullet::remove(int num)
{
	_vBullet[num].image->release();
	_vBullet.erase(_vBullet.begin() + num);
}

// =====================보스 탄환 1(페이즈2)=======================

HRESULT bossBullet::init(float range, int bulletMax)
{
	_range = range;
	_bulletMax = bulletMax;
	return S_OK;
}

void bossBullet::release()
{
}

void bossBullet::update()
{
	move();
}

void bossBullet::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->frameRender(getMemDC(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->image->getFrameX(), 0);

		_viBullet->count++;

		if (_viBullet->count % 5 == 0)
		{
			_viBullet->image->setFrameX(_viBullet->image->getFrameX() + 1);

			if (_viBullet->image->getFrameX() >= _viBullet->image->getMaxFrameX())
			{
				_viBullet->image->setFrameX(0);
			}

			_viBullet->count = 0;
		}
	}
}

void bossBullet::fire(float x, float y, float angle)
{
	if (_bulletMax < _vBullet.size()) return;
	tagBullet bossBullet;
	ZeroMemory(&bossBullet, sizeof(tagBullet));

	bossBullet.image = new image;

	bossBullet.image->init("boss/bossBullet.bmp", 0, 0, 190, 50, 4, 1,
		true, RGB(255, 0, 255));

	bossBullet.x = bossBullet.fireX = x;
	bossBullet.y = bossBullet.fireY = y;
	bossBullet.speed = 3.0f;
	bossBullet.angle = angle;
	bossBullet.rc = RectMakeCenter(bossBullet.x, bossBullet.y, bossBullet.image->getFrameWidth(), bossBullet.image->getFrameHeight());

	_vBullet.push_back(bossBullet);
}

void bossBullet::move()
{
	_count++;
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		if (getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y) > 200)
		{
			if (_count % 100 == 0)
			{
				_viBullet->index = RND->getFromIntTo(-5, 5);
			}
			_viBullet->x -= _viBullet->speed;
			_viBullet->y += _viBullet->index;
		}
		else
		{
			_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
			_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
			_viBullet->index = RND->getFromIntTo(-5, 5);
		}

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getFrameWidth(), _viBullet->image->getFrameHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			SAFE_RELEASE(_viBullet->image);
			SAFE_DELETE(_viBullet->image);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void bossBullet::remove(int num)
{
	_vBullet[num].image->release();
	_vBullet.erase(_vBullet.begin() + num);
}

//======================초코렛 빔========================

HRESULT chocoBeam::init(float range, int bulletMax)
{
	_range = range;
	_bulletMax = bulletMax;
	return S_OK;
}

void chocoBeam::release()
{
}

void chocoBeam::update(float gokuX, float gokuY)
{
	_gokuX = gokuX;
	_gokuY = gokuY;
	move();
}

void chocoBeam::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->frameRender(getMemDC(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->image->getFrameX(), 0);

		_viBullet->count++;

		if (_viBullet->count % 5 == 0)
		{
			_viBullet->image->setFrameX(_viBullet->image->getFrameX() + 1);

			if (_viBullet->image->getFrameX() >= _viBullet->image->getMaxFrameX())
			{
				_viBullet->image->setFrameX(0);
			}

			_viBullet->count = 0;
		}
	}
}

void chocoBeam::fire(float x, float y, float angle)
{
	if (_bulletMax < _vBullet.size()) return;
	tagBullet cBeam;
	ZeroMemory(&cBeam, sizeof(tagBullet));

	cBeam.image = new image;

	cBeam.image->init("boss/bossBullet2.bmp", 0, 0, 99, 32, 3, 1,
		true, RGB(255, 0, 255));

	cBeam.x = cBeam.fireX = x;
	cBeam.y = cBeam.fireY = y;
	cBeam.speed = 12.0f;
	cBeam.angle = angle;
	cBeam.rc = RectMakeCenter(cBeam.x, cBeam.y, cBeam.image->getFrameWidth(), cBeam.image->getFrameHeight());

	_vBullet.push_back(cBeam);
}

void chocoBeam::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getFrameWidth(), _viBullet->image->getFrameHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			SAFE_RELEASE(_viBullet->image);
			SAFE_DELETE(_viBullet->image);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void chocoBeam::remove(int num)
{
	_vBullet[num].image->release();
	_vBullet.erase(_vBullet.begin() + num);
}

//======================보스 탄환2(페이즈 4)========================

HRESULT bossBullet2::init(float range, int bulletMax)
{
	_range = range;
	_bulletMax = bulletMax;
	return S_OK;
}

void bossBullet2::release()
{
}

void bossBullet2::update()
{
	move();
}

void bossBullet2::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->image->frameRender(getMemDC(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->image->getFrameX(), 0);

		_viBullet->count++;

		if (_viBullet->count % 5 == 0)
		{
			_viBullet->image->setFrameX(_viBullet->image->getFrameX() + 1);

			if (_viBullet->image->getFrameX() >= _viBullet->image->getMaxFrameX())
			{
				_viBullet->image->setFrameX(0);
			}

			_viBullet->count = 0;
		}
	}
}

void bossBullet2::fire(float x, float y, float angle)
{
	if (_bulletMax < _vBullet.size()) return;
	tagBullet bossBullet2;
	ZeroMemory(&bossBullet2, sizeof(tagBullet));

	bossBullet2.image = new image;

	bossBullet2.image->init("boss/bossBullet3.bmp", 0, 0, 130, 26, 4, 1,
		true, RGB(255, 0, 255));

	bossBullet2.x = bossBullet2.fireX = x;
	bossBullet2.y = bossBullet2.fireY = y;
	bossBullet2.speed = 3.0f;
	bossBullet2.angle = angle;
	bossBullet2.index = 0;
	bossBullet2.rc = RectMakeCenter(bossBullet2.x, bossBullet2.y, bossBullet2.image->getFrameWidth(), bossBullet2.image->getFrameHeight());

	_vBullet.push_back(bossBullet2);
}

void bossBullet2::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		RECT sky = _viBullet->rc;
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		if (sky.left < 0)
		{
			_viBullet->angle = PI - _viBullet->angle;
			_viBullet->x = (sky.right - sky.left) / 2;
		}
		if (sky.right > WINSIZEX)
		{
			_viBullet->angle = PI - _viBullet->angle;
			_viBullet->x = WINSIZEX - (sky.right - sky.left) / 2;
		}
		if (sky.top < 0)
		{
			_viBullet->angle = PI * 2 - _viBullet->angle;
			_viBullet->y = (sky.bottom - sky.top) / 2;
		}
		if (sky.bottom > WINSIZEY)
		{
			_viBullet->angle = PI * 2 - _viBullet->angle;
			_viBullet->y = WINSIZEY - (sky.bottom - sky.top) / 2;
		}

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y, _viBullet->image->getFrameWidth(), _viBullet->image->getFrameHeight());

		if (sky.left < 0 || sky.right > WINSIZEX || sky.top < 0 || sky.bottom > WINSIZEY) _viBullet->index++;

		if (_viBullet->index > 4)
		{
			SAFE_RELEASE(_viBullet->image);
			SAFE_DELETE(_viBullet->image);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void bossBullet2::remove(int num)
{
	_vBullet[num].image->release();
	_vBullet.erase(_vBullet.begin() + num);
}

// =========================페이즈 4 가이드용=========================

HRESULT guideBullet::init()
{
	return S_OK;
}

void guideBullet::release()
{
}

void guideBullet::update()
{
	move();
}

void guideBullet::render()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		Rectangle(getMemDC(), _vBullet[i].rc);
	}
}

void guideBullet::fire(float x, float y)
{
	if (0 < _vBullet.size()) return;
	tagBullet guideBullet;
	ZeroMemory(&guideBullet, sizeof(tagBullet));

	_rndAngle = RND->getInt(360);
	guideBullet.x = _x = x;
	guideBullet.y = _y = y;
	guideBullet.speed = 5.0f;
	guideBullet.angle = PI2 / 360 * _rndAngle;
	guideBullet.rc = RectMakeCenter(_x, _y, 22, 22);

	_vBullet.push_back(guideBullet);
}

void guideBullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		RECT guide = _viBullet->rc;
		_x += cosf(_viBullet->angle) * _viBullet->speed;
		_y += -sinf(_viBullet->angle) * _viBullet->speed;

		if (guide.left < 0)
		{
			_viBullet->angle = PI - _viBullet->angle;
			_x = (guide.right - guide.left) / 2;
		}
		if (guide.right > WINSIZEX)
		{
			_viBullet->angle = PI - _viBullet->angle;
			_x = WINSIZEX - (guide.right - guide.left) / 2;
		}
		if (guide.top < 0)
		{
			_viBullet->angle = PI * 2 - _viBullet->angle;
			_y = (guide.bottom - guide.top) / 2;
		}
		if (guide.bottom > WINSIZEY)
		{
			_viBullet->angle = PI * 2 - _viBullet->angle;
			_y = WINSIZEY - (guide.bottom - guide.top) / 2;
		}

		_viBullet->rc = RectMakeCenter(_x, _y, 22, 22);

		if (guide.left < 0 || guide.right > WINSIZEX || guide.top < 0 || guide.bottom > WINSIZEY) _viBullet->index++;
	}
}

void guideBullet::remove(int num)
{
	_vBullet.erase(_vBullet.begin() + num);
}
