#include "stdafx.h"
#include "enemy.h"

HRESULT enemy::init(POINT position, float hp)
{
	_currentFrameX = _currentFrameY = 0;
	_x = position.x;
	_y = position.y;
	_maxHP = _currentHP = hp;
	_rc = RectMakeCenter(_x, _y, 0, 0);
	_frameCount = 0;
	_fireNum = 35;
	_fireNum2 = 15;
	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
	_currentState = _state;
}

void enemy::render()
{
}
void enemy::move(float x, float y)
{
	_x += x;
	_y += y;
}

void enemy::moveAngle(float angle, float speed)
{
	_x += cosf(angle) * speed;
	_y += -sinf(angle) * speed;
}

void enemy::hitDamage(float damage)
{
	_currentHP -= damage;
}

bool enemy::fireDelay()
{
	_fireCount++;
	if (_fireCount % _fireNum == 0)
	{
		_fireNum = RND->getFromIntTo(80, 100);
		_fireCount = 0;
		return true;
	}
	return false;
}

bool enemy::fireDelay2()
{
	_fireCount2++;
	if (_fireCount2 % _fireNum2 == 0)
	{
		_fireNum2 = 70;
		_fireCount2 = 0;
		return true;
	}
	return false;
}

bool enemy::fireDelay3()
{
	return false;
}
