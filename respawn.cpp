#include "stdafx.h"
#include "respawn.h"

HRESULT respawn::init()
{
	_delayCount1 = 1700;
	_delayCount2 = 1000;
	_delayCount3 = 0;
	
	return S_OK;
}

void respawn::release()
{
}

void respawn::update()
{
}

void respawn::render()
{
}

bool respawn::delayPattern1()
{
	_delayCount1++;
	if (_delayCount1 % 2000 == 0)
	{
		_delayCount1 = RND->getFromIntTo(1400, 1600);
		return true;
	}
	return false;
}

bool respawn::delayPattern2()
{
	_delayCount2++;
	if (_delayCount2 % 2000 == 0)
	{
		_delayCount2 = RND->getFromIntTo(600, 900);
		return true;
	}
	return false;
}

bool respawn::delayPattern3()
{
	_delayCount3++;
	if (_delayCount3 % 2000 == 0)
	{
		_delayCount3 = RND->getFromIntTo(200, 400);
		return true;
	}
	return false;
}
