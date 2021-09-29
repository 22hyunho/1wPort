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
	RECT _rc;								// 플레이어 렉트
	RECT _hitBox;
	image* _image;							// 플레이어 이미지
	float _x, _y;							// 플레이어 중점좌표
	float _damage;							// 에너미에게 입힐 데미지
	float _skillDamage;						// 스킬 데미지
	int _currentHP, _maxHP;					// 플레이어 체력
	int _state;								// 상태별 이미지 넣기위해
	int _currentFrameX, _currentFrameY;		// 프레임이미지 넘기기용
	int _frameCount, _skillFrameCount;		// 프레임이미지 카운트
	int _skillStartCount;					// 스킬발사 이미지 싱크 맞추기용
	int _skillCount;						// 스킬 프레임 카운트
	int _skillFrameX, _skillFrameY;			// 스킬이미지 프레임 넘기기용
	int _fireCount;							// 연사 조절용
	int _bulletLevel;						// 플레이어 탄환 레벨
	int _skillGauge;						// 스킬 갯수
	int _score;								// 게임 점수
	int _alphaValue;						// 플레이어 알파블렌더 조절
	int _alphaCount;						// 플레이어 알파블렌더 카운트
	int _telpoCount, _telpoRCount;			// 텔레포트 싱크 맞추기 카운트
	int _telpoFrameX, _telpoRFrameX;		// 텔레포트 이미지 프레용
	int _chocoCoolTime;						// 초콜렛 상태 해제 시간
	bool _isRevival;						// 플레이어 부활중인가?
	bool _isInvincible;						// 플레이어 무적상태인가?
	bool _isSkillOn;						// 스킬 사용중인지?
	bool _gameOver;							// 게임 오버인가?

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

