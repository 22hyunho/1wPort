#pragma once
#include "gameNode.h"
#include "progressBar.h"

class enemy : public gameNode
{
protected :
	RECT _rc;
	image* _image;
	float _x, _y;
	float _maxHP, _currentHP;
	int _state, _currentState;
	int _currentFrameX, _currentFrameY;
	int _frameCount;
	int _dieFrameCount;
	int _dieFrameCount2;
	int _fireCount;
	int _fireNum;
	int _fireCount2;
	int _fireNum2;
	int _fireCount3;
	int _fireNum3;

public :
	enemy() {};
	~enemy() {};

	virtual HRESULT init(POINT position, float hp);
	virtual void release();
	virtual void update();
	virtual void render();


	void move(float x, float y);
	void moveAngle(float angle, float speed);
	void hitDamage(float damage);

	bool fireDelay();
	bool fireDelay2();
	bool fireDelay3();
	
	float getX() { return _x; }
	float getY() { return _y; }
	void setX(float x) { _x = x; }

	inline float getHP() { return _currentHP; }
	inline float getMaxHP() { return _maxHP; }

	inline RECT getRect() { return _rc; }

	inline int getState() { return _state; }
	void setState(int state) { _state = state; }

	inline int getDieCount() { return _dieFrameCount; }
	inline int getFrameCount() { return _currentFrameX; }
	void setFrameCount(int frameCount) { _currentFrameX = frameCount; }
};