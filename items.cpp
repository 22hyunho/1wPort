#include "stdafx.h"
#include "items.h"

// ==================================== 총알 아이템 ======================================

HRESULT upgradeItem::init(int itemMax)
{
	_itemMax = itemMax;
	return S_OK;
}

void upgradeItem::release()
{
}

void upgradeItem::update()
{
	move();
}

void upgradeItem::render()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		_viItem->image->frameRender(getMemDC(),
			_viItem->rc.left,
			_viItem->rc.top,
			_viItem->image->getFrameX(), 0);

		_viItem->count++;

		if (_viItem->count % 5 == 0)
		{
			_viItem->image->setFrameX(_viItem->image->getFrameX() + 1);

			if (_viItem->image->getFrameX() >= _viItem->image->getMaxFrameX())
			{
				_viItem->image->setFrameX(0);
			}
			_viItem->count = 0;
		}
	}

	for (int i = 0; i < _vItem.size(); i++)
	{
		//Rectangle(getMemDC(), _vItem[i].rc);
	}
}

void upgradeItem::create(float x, float y, float angle)
{
	
	if (_itemMax < _vItem.size()) return;
	tagItem UPItem;
	ZeroMemory(&UPItem, sizeof(tagItem));

	UPItem.image = new image;
	UPItem.image->init("image/dragonball1.bmp", 0, 0, 336, 40, 8, 1,
		true, RGB(255, 0, 255));
	UPItem.x = x;
	UPItem.y = y;
	UPItem.speed = 3.0f;
	UPItem.angle = angle;
	UPItem.rc = RectMakeCenter(UPItem.x, UPItem.y, UPItem.image->getWidth(), UPItem.image->getHeight());

	_vItem.push_back(UPItem);
}

void upgradeItem::move()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		RECT item = _viItem->rc;
		_viItem->x += cosf(_viItem->angle) * _viItem->speed;
		_viItem->y += -sinf(_viItem->angle) * _viItem->speed;

		if (item.left < 0)
		{
			_viItem->angle = PI - _viItem->angle;
			_viItem->x = (item.right - item.left) / 2;
		}
		if (item.right > WINSIZEX)
		{
			_viItem->angle = PI - _viItem->angle;
			_viItem->x = WINSIZEX - (item.right - item.left) / 2;
		}
		if (item.top < 0)
		{
			_viItem->angle = PI * 2 - _viItem->angle;
			_viItem->y = (item.bottom - item.top) / 2;
		}
		if (item.bottom > WINSIZEY)
		{
			_viItem->angle = PI * 2 - _viItem->angle;
			_viItem->y = WINSIZEY - (item.bottom - item.top) / 2;
		}

		_viItem->rc = RectMakeCenter(_viItem->x, _viItem->y, _viItem->image->getFrameWidth(), _viItem->image->getFrameHeight());

		if (item.left < 0 || item.right > WINSIZEX || item.top < 0 || item.bottom > WINSIZEY) _viItem->index++;

		if (_viItem->index > 8)
		{
			SAFE_RELEASE(_viItem->image);
			SAFE_DELETE(_viItem->image);
			_viItem = _vItem.erase(_viItem);
		}
	}
}

void upgradeItem::remove(int num)
{
	_vItem[num].image->release();
	_vItem.erase(_vItem.begin() + num);
}

// ==================================== HP 아이템 ======================================

HRESULT hpItem::init(int itemMax)
{
	IMAGEMANAGER->addImage("하트", "image/heart.bmp", 42, 40, true, RGB(255, 0, 255));
	_itemMax = itemMax;
	return S_OK;
}

void hpItem::release()
{
}

void hpItem::update()
{
	move();
}

void hpItem::render()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		_viItem->image->render(getMemDC(), _viItem->rc.left, _viItem->rc.top);
	}
}

void hpItem::create(float x, float y, float angle)
{
	if (_itemMax < _vItem.size()) return;
	tagItem HPItem;
	ZeroMemory(&HPItem, sizeof(tagItem));

	HPItem.image = IMAGEMANAGER->findImage("하트");
	HPItem.x = x;
	HPItem.y = y;
	HPItem.speed = 3.0f;
	HPItem.angle = angle;
	HPItem.rc = RectMakeCenter(HPItem.x, HPItem.y, HPItem.image->getWidth(), HPItem.image->getHeight());

	_vItem.push_back(HPItem);
}

void hpItem::move()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end();)
	{
		_viItem->x += cosf(_viItem->angle) * _viItem->speed;
		_viItem->y += -sinf(_viItem->angle) * _viItem->speed;

		if (_viItem->rc.left < 0)
		{
			_viItem->angle = PI - _viItem->angle;
			_viItem->x = (_viItem->rc.right - _viItem->rc.left) / 2;
		}
		if (_viItem->rc.right > WINSIZEX)
		{
			_viItem->angle = PI - _viItem->angle;
			_viItem->x = WINSIZEX - (_viItem->rc.right - _viItem->rc.left) / 2;
		}
		if (_viItem->rc.top < 0)
		{
			_viItem->angle = PI * 2 - _viItem->angle;
			_viItem->y = (_viItem->rc.bottom - _viItem->rc.top) / 2;
		}
		if (_viItem->rc.bottom > WINSIZEY)
		{
			_viItem->angle = PI * 2 - _viItem->angle;
			_viItem->y = WINSIZEY - (_viItem->rc.bottom - _viItem->rc.top) / 2;
		}

		_viItem->rc = RectMakeCenter(_viItem->x, _viItem->y, _viItem->image->getWidth(), _viItem->image->getHeight());

		if (_viItem->rc.left < 0 || _viItem->rc.right > WINSIZEX || _viItem->rc.top < 0 || _viItem->rc.bottom > WINSIZEY) _viItem->index++;

		if (_viItem->index > 2)
		{
			_viItem = _vItem.erase(_viItem);
		}
		else ++_viItem;
	}
}

void hpItem::remove(int num)
{
	_vItem.erase(_vItem.begin() + num);
}

// ==================================== 스킬 아이템 ======================================

HRESULT skillItem::init(int itemMax)
{
	IMAGEMANAGER->addImage("고기", "image/meat.bmp", 64, 40, true, RGB(255, 0, 255));
	_itemMax = itemMax;
	return S_OK;
}

void skillItem::release()
{
}

void skillItem::update()
{
	move();
}

void skillItem::render()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		_viItem->image->render(getMemDC(), _viItem->rc.left, _viItem->rc.top);
	}
}

void skillItem::create(float x, float y, float angle)
{
	if (_itemMax < _vItem.size()) return;
	tagItem skillItem;
	ZeroMemory(&skillItem, sizeof(tagItem));

	skillItem.image = IMAGEMANAGER->findImage("고기");
	skillItem.x = x;
	skillItem.y = y;
	skillItem.speed = 3.0f;
	skillItem.angle = angle;
	skillItem.rc = RectMakeCenter(skillItem.x, skillItem.y, skillItem.image->getWidth(), skillItem.image->getHeight());

	_vItem.push_back(skillItem);
}

void skillItem::move()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end();)
	{
		RECT skillTem = _viItem->rc;
		_viItem->x += cosf(_viItem->angle) * _viItem->speed;
		_viItem->y += -sinf(_viItem->angle) * _viItem->speed;

		if (skillTem.left < 0)
		{
			_viItem->angle = PI - _viItem->angle;
			_viItem->x = (skillTem.right - skillTem.left) / 2;
		}
		if (skillTem.right > WINSIZEX)
		{
			_viItem->angle = PI - _viItem->angle;
			_viItem->x = WINSIZEX - (skillTem.right - skillTem.left) / 2;
		}
		if (skillTem.top < 0)
		{
			_viItem->angle = PI * 2 - _viItem->angle;
			_viItem->y = (skillTem.bottom - skillTem.top) / 2;
		}
		if (skillTem.bottom > WINSIZEY)
		{
			_viItem->angle = PI * 2 - _viItem->angle;
			_viItem->y = WINSIZEY - (skillTem.bottom - skillTem.top) / 2;
		}

		_viItem->rc = RectMakeCenter(_viItem->x, _viItem->y, _viItem->image->getWidth(), _viItem->image->getHeight());

		if (skillTem.left < 0 || skillTem.right > WINSIZEX || skillTem.top < 0 || skillTem.bottom > WINSIZEY) _viItem->index++;

		if (_viItem->index > 8)
		{
			_viItem = _vItem.erase(_viItem);
		}
		else ++_viItem;
	}
}

void skillItem::remove(int num)
{
	_vItem.erase(_vItem.begin() + num);
}
