#pragma once
#include "gameNode.h"
#include <vector>

struct tagItem
{
	image* image;
	RECT rc;
	float x, y;
	float angle;
	float speed;
	int count;
	int index;
	bool isCreate;
};

class upgradeItem : public gameNode
{
private:
	vector<tagItem>			  _vItem;
	vector<tagItem>::iterator _viItem;

	int _itemMax;

public:
	upgradeItem() {};
	~upgradeItem() {};

	virtual HRESULT init(int itemMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void create(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagItem>				getVItem() { return _vItem; }
	vector<tagItem>::iterator	getVIItem() { return _viItem; }
};

class hpItem : public gameNode
{
private:
	vector<tagItem>			  _vItem;
	vector<tagItem>::iterator _viItem;

	int _itemMax;

public:
	hpItem() {};
	~hpItem() {};

	virtual HRESULT init(int itemMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void create(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagItem>				getVItem() { return _vItem; }
	vector<tagItem>::iterator	getVIItem() { return _viItem; }
};

class skillItem : public gameNode
{
private:
	vector<tagItem>			  _vItem;
	vector<tagItem>::iterator _viItem;

	int _itemMax;

public:
	skillItem() {};
	~skillItem() {};

	virtual HRESULT init(int itemMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void create(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagItem>				getVItem() { return _vItem; }
	vector<tagItem>::iterator	getVIItem() { return _viItem; }
};