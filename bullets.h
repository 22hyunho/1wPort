#pragma once
#include "gameNode.h"
#include <vector>

struct tagBullet
{
	image* image;		
	RECT rc;				
	float x, y;				
	float angle;			
	float speed;			
	float fireX, fireY;		
	int count;				
	int index;
};

class energyBullet : public gameNode
{
private : 
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	int _bulletMax;
	int _bulletLevel;

public :
	energyBullet() {};
	~energyBullet() {};

	virtual HRESULT init(float range, int bulletMax);
	virtual void release();
	virtual void update(int bulletLevel);
	virtual void render();

	void fire(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagBullet>				getVEgBullet() { return _vBullet; }
	vector<tagBullet>::iterator		getVIEgBullet() { return _viBullet; }
};

class energyWave : public gameNode
{
private:
	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	int _currentFrameX, _currentFrameY;
	int _skillCount;
	int _skillFrameX, _skillFrameY;

public:
	energyWave() {};
	~energyWave() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y);

	void move();
	void remove(int num);

	void setFrameY(int num) { _skillFrameY = num; }
	void setSkillCount(int num) { _skillCount = num; }
	vector<tagBullet>				getVWave() { return _vBullet; }
	vector<tagBullet>::iterator		getVIWave() { return _viBullet; }
};

class satanMissile : public gameNode
{
private :
	vector<tagBullet>			 _vBullet;
	vector<tagBullet>::iterator  _viBullet;

	float _range;
	int _bulletMax;

public :
	satanMissile() {};
	~satanMissile() {};

	virtual HRESULT init(float range, int bulletMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
};

class basicBullet : public gameNode
{
private :
	vector<tagBullet>			 _vBullet;
	vector<tagBullet>::iterator  _viBullet;

	float _range;
	int _bulletMax;

public :
	basicBullet() {};
	~basicBullet() {};

	virtual HRESULT init(float range, int bulletMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y, float angle, float speed);

	void move();
	void remove(int num);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
};

class piccoloBeam : public gameNode
{
private:
	vector<tagBullet>			 _vBullet;
	vector<tagBullet>::iterator  _viBullet;

	float _range;
	float _gokuX;
	float _gokuY;

	int _bulletMax;

public:
	piccoloBeam() {};
	~piccoloBeam() {};

	virtual HRESULT init(float range, int bulletMax);
	virtual void release();
	virtual void update(float gokuX, float gokuY);
	virtual void render();

	void fire(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
};

class friezaBullet : public gameNode
{
private:
	vector<tagBullet>			 _vBullet;
	vector<tagBullet>::iterator  _viBullet;

	float _range;
	float _angle;

	int _bulletMax;

public:
	friezaBullet() {};
	~friezaBullet() {};

	virtual HRESULT init(float range, int bulletMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y, float angle);

	void move();
	void remove(int num);

	inline float getVBAngle() { return _angle; }

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
	
};

class bossBullet : public gameNode
{
	private:
		vector<tagBullet>			 _vBullet;
		vector<tagBullet>::iterator  _viBullet;

		float _range;
		float _angle;

		int _bulletMax;
		int _count;

public:
	bossBullet() {};
	~bossBullet() {};

	virtual HRESULT init(float range, int bulletMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
};

class chocoBeam : public gameNode
{
private:
	vector<tagBullet>			 _vBullet;
	vector<tagBullet>::iterator  _viBullet;

	float _range;
	float _angle;
	float _gokuX;
	float _gokuY;

	int _bulletMax;
	int _count;

public:
	chocoBeam() {};
	~chocoBeam() {};

	virtual HRESULT init(float range, int bulletMax);
	virtual void release();
	virtual void update(float gokuX, float gokuY);
	virtual void render();

	void fire(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
};

class bossBullet2 : public gameNode
{
private :
	vector<tagBullet>			 _vBullet;
	vector<tagBullet>::iterator  _viBullet;

	float _range;
	float _angle;

	int _bulletMax;
	int _count;

public:
	bossBullet2() {};
	~bossBullet2() {};

	virtual HRESULT init(float range, int bulletMax);
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y, float angle);

	void move();
	void remove(int num);

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
};

class guideBullet : public gameNode
{
private :
	vector<tagBullet>			 _vBullet;
	vector<tagBullet>::iterator  _viBullet;

	float _range;
	float _angle;
	float _rndAngle;

	float _x, _y;

public:
	guideBullet() {};
	~guideBullet() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void fire(float x, float y);

	void move();
	void remove(int num);

	inline float getX() { return _x; }
	inline float getY() { return _y; }

	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getViBullet() { return _viBullet; }
};