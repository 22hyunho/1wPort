#pragma once
#include "gameNode.h"
#include "bullets.h"
#include "mrSatan.h"
#include "krillin.h"
#include "piccolo.h"
#include "midBoss.h"
#include "boss.h"
#include "respawn.h"
#include "explosion.h"

class player;

struct enemyOption
{
	int state;
	int summon;
	int dieCount;
	int count;
	int count2;
	int phase;
	float rnd;
	float angle;
	bool isDown;
	bool isLeft;
};

enum State
{
	E_FRONT = 1,
	E_ATTACK,
	E_DIE
};

class enemyManager : public gameNode
{
private :
	typedef vector<enemy*>			 _vEnemy;
	typedef vector<enemy*>::iterator _viEnemy;

	_vEnemy _vSatan;
	_viEnemy _viSatan;
	enemyOption satan;
	satanMissile* _stMissile;

	_vEnemy _vKrillin;
	_viEnemy _viKrillin;
	enemyOption sKrillin;
	basicBullet* _bBullet;

	_vEnemy _vPiccolo;
	_viEnemy _viPiccolo;
	enemyOption sPiccolo;
	piccoloBeam* _pBeam;

	_vEnemy _vFrieza;
	_viEnemy _viFrieza;
	enemyOption sFrieza;
	friezaBullet* _fBullet;

	_vEnemy _vBuu;
	_viEnemy _viBuu;
	enemyOption sBuu;
	bossBullet* _bossBullet;
	chocoBeam* _cBeam;
	bossBullet2* _bossBullet2;
	guideBullet* _guide;
	
	respawn* _delayCount;
	explosion* _explo;
	float _loopX, _loopY;
	int _stage;
	
	bool _victory;
	player* _goku;
	
public :
	enemyManager() {};
	~enemyManager() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();


	void moveMinion();
	void hitDamage(float enemyHp, float damage);

	void setSatan(float x, float y);
	void removeSatan(int num);
	void satanShotFire();

	void setKrillin(float x, float y);
	void removeKrillin(int num);
	void krillinShotFire();

	void setPiccolo(float x, float y);
	void removePiccolo(int num);
	void piccoloShotFire();

	void setFrieza(float x, float y);
	void removeFrieza(int num);
	void friezaShotFire();

	void setBuu(float x, float y);
	void removeBuu(int num);
	void buuShotFire();
	void buuAirAttack();
	void buuChocoAttack();
	void buuSkyBulletFire(float x, float y);
	void guideFire();

	void collision();

	void setGokuLink(player* goku) { _goku = goku; }

	vector<enemy*>				getVSatan() { return _vSatan; }
	vector<enemy*>::iterator	getVISatan() { return _viSatan; }
	vector<enemy*>				getVKrillin() { return _vKrillin; }
	vector<enemy*>::iterator	getVIKrillin() { return _viKrillin; }
	vector<enemy*>				getVPiccolo() { return _vPiccolo; }
	vector<enemy*>::iterator	getVIPiccolo() { return _viPiccolo; }
	vector<enemy*>				getVFrieza() { return _vFrieza; }
	vector<enemy*>::iterator	getVIFrieza() { return _viFrieza; }
	vector<enemy*>				getVBuu() { return _vBuu; }
	vector<enemy*>::iterator	getVIBuu() { return _viBuu; }
};

