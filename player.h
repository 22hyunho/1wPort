#pragma once
#include "gameNode.h"
#include "bullets.h"
#include "items.h"

class enemyManager;

enum state
{
	IDLE = 1,
	FRONT,
	BACK,
	ATTACK,
	SKILL,
	TELEPOTE,
	R_TELEPOTE,
	DIE,
	CHOCO
};

class player : public gameNode
{
private :
	RECT _rc;								// �÷��̾� ��Ʈ
	RECT _hitBox;
	image* _image;							// �÷��̾� �̹���
	float _x, _y;							// �÷��̾� ������ǥ
	float _damage;							// ���ʹ̿��� ���� ������
	float _skillDamage;						// ��ų ������
	int _currentHP, _maxHP;					// �÷��̾� ü��
	int _state;								// ���º� �̹��� �ֱ�����
	int _currentFrameX, _currentFrameY;		// �������̹��� �ѱ���
	int _frameCount, _skillFrameCount;		// �������̹��� ī��Ʈ
	int _skillStartCount;					// ��ų�߻� �̹��� ��ũ ���߱��
	int _skillCount;						// ��ų ������ ī��Ʈ
	int _skillFrameX, _skillFrameY;			// ��ų�̹��� ������ �ѱ���
	int _fireCount;							// ���� ������
	int _bulletLevel;						// �÷��̾� źȯ ����
	int _skillGauge;						// ��ų ����
	int _score;								// ���� ����
	int _alphaValue;						// �÷��̾� ���ĺ��� ����
	int _alphaCount;						// �÷��̾� ���ĺ��� ī��Ʈ
	int _telpoCount, _telpoRCount;			// �ڷ���Ʈ ��ũ ���߱� ī��Ʈ
	int _telpoFrameX, _telpoRFrameX;		// �ڷ���Ʈ �̹��� ������
	int _chocoCoolTime;						// ���ݷ� ���� ���� �ð�
	bool _isRevival;						// �÷��̾� ��Ȱ���ΰ�?
	bool _isInvincible;						// �÷��̾� ���������ΰ�?
	bool _isSkillOn;						// ��ų ���������?
	bool _gameOver;							// ���� �����ΰ�?

	energyBullet* _egBullet;
	energyWave* _wave;

	upgradeItem* _upItem;
	hpItem* _hpItem;
	skillItem* _skillItem;

	image* _number[10];
	image* _HP[5];
	image* _nullHP[5];
	image* _skillMeat[3];

	enemyManager* _em;

public :
	player() {};
	~player() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void move();
	void frameImage();
	void hitDamage(int damage);
	void collision();

	inline float getGokuX() { return _x; }
	inline float getGokuY() { return _y; }

	inline bool getRevival() { return _isRevival; }
	inline bool getInvincible() { return _isInvincible; }
	inline bool getGameOver() { return _gameOver; }

	inline int getState() { return _state; }
	void setState(int state) { _state = state; }

	void setEmLink(enemyManager* em) { _em = em; }

	inline RECT getRect() { return _rc; }
	inline RECT getHitBox() { return _hitBox; }

	energyWave* getWave() { return _wave; }
};

