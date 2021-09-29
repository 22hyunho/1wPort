#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"

HRESULT enemyManager::init()
{
	IMAGEMANAGER->addImage("기본배경", "image/backGround1.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("보스배경", "image/backGround2.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	_stMissile = new satanMissile;
	_stMissile->init(WINSIZEX, 100);

	_bBullet = new basicBullet;
	_bBullet->init(WINSIZEX, 60);

	_pBeam = new piccoloBeam;
	_pBeam->init(WINSIZEX, 20);

	_fBullet = new friezaBullet;
	_fBullet->init(WINSIZEX, 40);

	_bossBullet = new bossBullet;
	_bossBullet->init(WINSIZEX, 40);

	_cBeam = new chocoBeam;
	_cBeam->init(WINSIZEX, 10);

	_bossBullet2 = new bossBullet2;
	_bossBullet2->init(WINSIZEX, 30);

	_guide = new guideBullet;
	_guide->init();

	_delayCount = new respawn;
	_delayCount->init();

	_explo = new explosion;
	_explo->init();

	satan.summon = 0;
	sFrieza.isDown = false;

	sBuu.isDown = false;
	sBuu.isLeft = true;
	sBuu.phase = 0;
	
	_victory = false;
	return S_OK;
}

void enemyManager::release()
{
}

void enemyManager::update()
{
	if (KEYMANAGER->isOnceKeyDown('V'))
	{
		_stage++;
		if (_stage > 2) _stage = 0;
	}
	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		sBuu.phase++;
		if (sBuu.phase > 3) sBuu.phase = 0;
	}
	if (KEYMANAGER->isOnceKeyDown('Q')) setSatan(WINSIZEX + 50, 80);
	if (KEYMANAGER->isOnceKeyDown('W')) setKrillin(WINSIZEX + 50, 150);
	if (KEYMANAGER->isOnceKeyDown('E')) setPiccolo(WINSIZEX + 50, 250);

	for (_viSatan = _vSatan.begin(); _viSatan != _vSatan.end(); ++_viSatan)
	{
		(*_viSatan)->update();
	}

	for (_viKrillin = _vKrillin.begin(); _viKrillin != _vKrillin.end(); ++_viKrillin)
	{
		(*_viKrillin)->update();
	}

	for (_viPiccolo = _vPiccolo.begin(); _viPiccolo != _vPiccolo.end(); ++_viPiccolo)
	{
		(*_viPiccolo)->update();
	}

	for (_viFrieza = _vFrieza.begin(); _viFrieza != _vFrieza.end(); ++_viFrieza)
	{
		(*_viFrieza)->update();
	}

	for (_viBuu = _vBuu.begin(); _viBuu != _vBuu.end(); ++_viBuu)
	{
		(*_viBuu)->update();
	}

	switch (_stage) // ==========================================스테이지 설정
	{
	case 0 :
		if (!_goku->getRevival())
		{
			if (_vSatan.size() == 0 && sPiccolo.summon < 6) // 사탄
			{
				setSatan(WINSIZEX + 50, 80);
				satan.summon++;
			}
			if (_delayCount->delayPattern2() && sPiccolo.summon < 6) // 크리링
			{
				setKrillin(WINSIZEX + 50, 150);
				sKrillin.summon++;
			}
			if (_delayCount->delayPattern3() && sPiccolo.summon < 6) // 피콜로
			{
				setPiccolo(WINSIZEX + 50, 250);
				sPiccolo.summon++;
			}
		}

		if (sPiccolo.summon >= 5) // 피콜로 5번 소환되면 미드보스 스테이지로
		{
			_stage++;
		}
		break;
		
	case 1 :
		if (sFrieza.summon < 1) // 프리저 (미드보스)
		{
			setFrieza(WINSIZEX + 200, WINSIZEY / 2);
			sFrieza.summon++;
		}

		if (sFrieza.summon >= 1 && _vFrieza.size() == 0) // 프리저가 1번이상 소환됐고 프리저의 벡터사이즈가 0이면 보스스테이지로
		{
			_stage++;
		}
		break;

	case 2 :
		if (sBuu.summon < 1) // 마인부우 (보스)
		{
			setBuu(WINSIZEX + 200, WINSIZEY / 2);
			sBuu.summon++;
		}
		break;
	}

	moveMinion();
	_stMissile->update();
	_bBullet->update();
	_pBeam->update(_goku->getGokuX(), _goku->getGokuY());
	_fBullet->update();
	_bossBullet->update();
	_cBeam->update(_goku->getGokuX(), _goku->getGokuY());
	_bossBullet2->update();
	_guide->update();
	satanShotFire();
	krillinShotFire();
	piccoloShotFire();
	friezaShotFire();
	buuShotFire();
	buuAirAttack();
	buuChocoAttack();
	buuSkyBulletFire(_guide->getX(), _guide->getY());
	guideFire();
	collision();


	if (_stage < 2) _loopX += 3;
	if (_stage == 2)_loopX += 0.3f;

	if (_victory) SCENEMANAGER->changeScene("승리");

}

void enemyManager::render()
{
	RECT loop = RectMake(0, 0, WINSIZEX, WINSIZEY);
	if (_stage < 2) IMAGEMANAGER->findImage("기본배경")->loopRender(getMemDC(), &loop, _loopX, _loopY);
	if (_stage == 2)IMAGEMANAGER->findImage("보스배경")->loopRender(getMemDC(), &loop, _loopX, _loopY); // 스테이지별 배경을 다르게

	for (_viSatan = _vSatan.begin(); _viSatan != _vSatan.end(); ++_viSatan) // 사탄 렌더
	{
		(*_viSatan)->render();
	}

	for (_viKrillin = _vKrillin.begin(); _viKrillin != _vKrillin.end(); ++_viKrillin) // 크리링
	{
		(*_viKrillin)->render();
	}

	for (_viPiccolo = _vPiccolo.begin(); _viPiccolo != _vPiccolo.end(); ++_viPiccolo) // 피콜로
	{
		(*_viPiccolo)->render();
	}

	for (_viFrieza = _vFrieza.begin(); _viFrieza != _vFrieza.end(); ++_viFrieza) // 프리저
	{
		(*_viFrieza)->render();
	}

	for (_viBuu = _vBuu.begin(); _viBuu != _vBuu.end(); ++_viBuu) // 마인부우
	{
		(*_viBuu)->render();
	}

	_stMissile->render();
	_bBullet->render();
	_pBeam->render();
	_fBullet->render();
	_bossBullet->render();
	_cBeam->render();
	_bossBullet2->render();

	if (sBuu.dieCount < 200) _explo->render();
}


void enemyManager::moveMinion() // ============================= 에너미 무브함수
{
	for (int i = 0; i < _vSatan.size(); i++)
	{
		if (_vSatan[i]->getState() != SATAN_DIE) // ====사탄 무브
		{
			if (_vSatan[i]->getX() <= WINSIZEX - 100)
			{
				_vSatan[i]->move(-1, 0);
				_vSatan[i]->setState(E_ATTACK);
			}
			else
			{
				_vSatan[i]->move(-4, 0);
				_vSatan[i]->setState(E_FRONT);
			}
		}
		else
		{
			satan.dieCount++;
			if (satan.dieCount >= 100)
			{
				_vSatan[i]->setX(0);
				satan.dieCount = 0;
			}
		}
		if (_vSatan[i]->getX() <= 0)
		{
			removeSatan(i);
		}
	}
	
	for (int i = 0; i < _vKrillin.size(); i++) // =====크리링 무브
	{
		if (_vKrillin[i]->getState() != K_DIE)
		{
			if (_vKrillin[i]->getX() <= WINSIZEX / 2 + 150 + i / 3 * 100)
			{
				_vKrillin[i]->move(0, 0);
				_vKrillin[i]->setState(E_ATTACK);
			}
			else
			{
				_vKrillin[i]->move(-4, 0);
				_vKrillin[i]->setState(E_FRONT);
			}
		}
		else
		{
			sKrillin.dieCount++;
			if (sKrillin.dieCount >= 100)
			{
				_vKrillin[i]->setX(0);
				sKrillin.dieCount = 0;
			}
		}
		if (_vKrillin[i]->getX() <= 0)
		{
			removeKrillin(i);
		}
	}

	for (int i = 0; i < _vPiccolo.size(); i++) // ======피콜로 무브
	{
		if (_vPiccolo[i]->getState() != P_DIE)
		{
			if (_vPiccolo[i]->getX() <= WINSIZEX - 200)
			{
				_vPiccolo[i]->move(0, 0);
				_vPiccolo[i]->setState(E_ATTACK);
			}
			else
			{
				_vPiccolo[i]->move(-4, 0);
				_vPiccolo[i]->setState(E_FRONT);
			}
		}
		else
		{
			sPiccolo.dieCount++;
			if (sPiccolo.dieCount >= 100)
			{
				_vPiccolo[i]->setX(0);
				sPiccolo.dieCount = 0;
			}
		}
		if (_vPiccolo[i]->getX() <= 0)
		{
			removePiccolo(i);
		}
	}

	for (int i = 0; i < _vFrieza.size(); i++) // ======프리저 무브
	{
		if (_vFrieza[i]->getState() != F_DIE)
		{
			if (_fBullet->getVBullet().size() <= 40)
			{
				if (_vFrieza[i]->getX() <= WINSIZEX - 200)
				{
					if (_vFrieza[i]->getY() < 200) sFrieza.isDown = true;
					if (_vFrieza[i]->getY() > WINSIZEY - 200) sFrieza.isDown = false;
					if (sFrieza.isDown) _vFrieza[i]->move(0, 1);
					if (!sFrieza.isDown) _vFrieza[i]->move(0, -1);
					_vFrieza[i]->setState(E_ATTACK);
				}
				else
				{
					_vFrieza[i]->move(-2, 0);
					_vFrieza[i]->setState(E_FRONT);
				}
			}
			else
			{
				if (_vFrieza[i]->getY() < 200) sFrieza.isDown = true;
				if (_vFrieza[i]->getY() > WINSIZEY - 200) sFrieza.isDown = false;
				if (sFrieza.isDown) _vFrieza[i]->move(0, 1);
				if (!sFrieza.isDown) _vFrieza[i]->move(0, -1);
				_vFrieza[i]->setState(E_FRONT);
			}
		}
		else
		{
			for (int i = 0; i < _fBullet->getVBullet().size(); i++)
			{
				_fBullet->remove(i); // 리무브함수로 이미지까지 같이 지워야 안터지는건데 벡터에 담은 불렛만 지우면 되는줄 알고 1시간 헤맴
			}
			sFrieza.dieCount++;
			if (sFrieza.dieCount >= 100)
			{
				_vFrieza[i]->setX(0);
			}
		}
		if (_vFrieza[i]->getX() <= 0)
		{
			removeFrieza(i);
		}
	}
	
	for (int i = 0; i < _vBuu.size(); i++) // ======마인부우 무브
	{
		switch (sBuu.phase)
		{
		case 0 : // =========================================== 페이즈 0
			if (_vBuu[i]->getX() <= WINSIZEX - 200)
			{
				if (_vBuu[i]->getY() < 200) sBuu.isDown = true;
				if (_vBuu[i]->getY() > WINSIZEY - 200) sBuu.isDown = false;
				if (sBuu.isDown) _vBuu[i]->move(0, 2);
				if (!sBuu.isDown) _vBuu[i]->move(0, -2);
				_vBuu[i]->setState(M_ATTACK);
			}
			else
			{
				_vBuu[i]->move(-4, 0);
				_vBuu[i]->setState(M_FRONT);
			}

			if (_vBuu[i]->getHP() < _vBuu[i]->getMaxHP() / 4 * 3) sBuu.phase++;
			break;

		case 1 : // =========================================== 페이즈 1
			
			_vBuu[i]->setState(M_AIRATTACK);
			
			if (_vBuu[i]->getY() < 200) sBuu.isDown = true;
			if (_vBuu[i]->getY() > WINSIZEY - 200) sBuu.isDown = false;
			if (sBuu.isDown) _vBuu[i]->move(0, 3);
			if (!sBuu.isDown) _vBuu[i]->move(0, -3);
			
		

			if (_vBuu[i]->getHP() < _vBuu[i]->getMaxHP() / 4 * 2) sBuu.phase++;
			break;

		case 2 : // =========================================== 페이즈 2
				
			if (_vBuu[i]->getY() > WINSIZEY / 2)
			{
				_vBuu[i]->setState(M_FRONT);
				if (_vBuu[i]->getX() < WINSIZEX - 100) _vBuu[i]->move(2, -1);
				else _vBuu[i]->move(0, -1);
			}
			if (_vBuu[i]->getY() < WINSIZEY / 2)
			{
				_vBuu[i]->setState(M_FRONT);
				if (_vBuu[i]->getX() < WINSIZEX - 100) _vBuu[i]->move(2, 1);
				else _vBuu[i]->move(0, 1);
			}
			if (_vBuu[i]->getY() == WINSIZEY / 2)
			{
				_vBuu[i]->setState(M_CHOCOATTACK);
				if (_vBuu[i]->getX() < WINSIZEX - 100) _vBuu[i]->move(2, 0);
				else _vBuu[i]->move(0, 0);
			}

			if (_vBuu[i]->getHP() == _vBuu[i]->getMaxHP() / 4) sBuu.phase++;
			break;

		case 3 : // =========================================== 페이즈 3

			if (_vBuu[i]->getState() != M_DIE)
			{
				_vBuu[i]->setState(M_ROLLATTACK);
				RECT rc = _vBuu[i]->getRect();
				if (sBuu.isDown == false && sBuu.isLeft == true) _vBuu[i]->moveAngle(getAngle(_vBuu[i]->getX(), _vBuu[i]->getY(), WINSIZEX / 2, 0), 3);
				else if (sBuu.isDown == true && sBuu.isLeft == true) _vBuu[i]->moveAngle(getAngle(_vBuu[i]->getX(), _vBuu[i]->getY(), 0, WINSIZEY / 2), 3);
				else if (sBuu.isDown == true && sBuu.isLeft == false) _vBuu[i]->moveAngle(getAngle(_vBuu[i]->getX(), _vBuu[i]->getY(), WINSIZEX / 2, WINSIZEY), 3);
				else if (sBuu.isDown == false && sBuu.isLeft == false) _vBuu[i]->moveAngle(getAngle(_vBuu[i]->getX(), _vBuu[i]->getY(), WINSIZEX, WINSIZEY / 2), 3);

				if (_vBuu[i]->getY() < 0 + (rc.bottom - rc.top) / 2) { sBuu.isDown = true; sBuu.isLeft = true; }
				if (_vBuu[i]->getX() < 0 + (rc.right - rc.left) / 2) { sBuu.isDown = true; sBuu.isLeft = false; }
				if (_vBuu[i]->getY() > WINSIZEY - (rc.bottom - rc.top) / 2) { sBuu.isDown = false; sBuu.isLeft = false; }
				if (_vBuu[i]->getX() > WINSIZEX - (rc.right - rc.left) / 2) { sBuu.isDown = false; sBuu.isLeft = true; }
			}
			else
			{
				_explo->update(_vBuu[i]->getX(), _vBuu[i]->getY());
				for (int i = 0; i < _bossBullet2->getVBullet().size(); i++)
				{
					_bossBullet2->remove(i);
				}
				sBuu.dieCount++;
				if (sBuu.dieCount >= 300)
				{
					_vBuu[i]->setX(0);
				}
			}
			if (_vBuu[i]->getX() <= 0)
			{
				removeBuu(i);
				_victory = true;
			}


			break;
		}
	}

}

void enemyManager::hitDamage(float enemyHp, float damage)
{
	enemyHp -= damage;
}

//=======================사탄=========================

void enemyManager::setSatan(float x, float y) // 사탄 소환
{
	for (int i = 0; i < 5; i++)
	{
		enemy* satan;
		satan = new mrSatan;
		satan->init(PointMake(x, y + (i * 150)), 50);

		_vSatan.push_back(satan);
	}
}
void enemyManager::removeSatan(int num) // === 사탄 리무브
{
	_vSatan.erase(_vSatan.begin() + num);
}
void enemyManager::satanShotFire() // ======== 사탄 포탄발사
{
	for (_viSatan = _vSatan.begin(); _viSatan != _vSatan.end(); ++_viSatan)
	{
		if ((*_viSatan)->getState() == E_ATTACK && ((*_viSatan)->fireDelay()))
		{
			RECT rc = (*_viSatan)->getRect();
			_stMissile->fire(rc.left - 10, (rc.top + rc.bottom) / 2 - 13, PI);
		}
	}
}

//=======================크리링=========================

void enemyManager::setKrillin(float x, float y) // 크리링 소환
{
	for (int i = 0; i < 3; i++)
	{
		enemy* eKrillin;
		eKrillin = new krillin;
		eKrillin->init(PointMake(x, y + (i * 230)), 300);

		_vKrillin.push_back(eKrillin);
	}
}
void enemyManager::removeKrillin(int num) // === 크리링 리무브
{
	_vKrillin.erase(_vKrillin.begin() + num);
}
void enemyManager::krillinShotFire() // ======== 크리링 포탄발사
{
	for (_viKrillin = _vKrillin.begin(); _viKrillin != _vKrillin.end(); ++_viKrillin)
	{
		if ((*_viKrillin)->getState() == E_ATTACK && ((*_viKrillin)->fireDelay2()))
		{
			RECT rc = (*_viKrillin)->getRect();
			for (int i = 0; i < 2; i++)
			{
				int angle = RND->getFromIntTo(20, 40);
				_bBullet->fire(rc.left + 5, (rc.top + rc.bottom) / 2 - 17, (PI - PI / 180 * angle) + i * (PI / 180 * 40), 10.0f);
			}
		}
	}
}

//=======================피콜로=========================

void enemyManager::setPiccolo(float x, float y) // 피콜로 소환
{
	for (int i = 0; i < 2; i++)
	{
		enemy* ePiccolo;
		ePiccolo = new piccolo;
		ePiccolo->init(PointMake(x, y + (i * 290)), 400);

		_vPiccolo.push_back(ePiccolo);
	}
}

void enemyManager::removePiccolo(int num) // === 피콜로 리무브
{
	_vPiccolo.erase(_vPiccolo.begin() + num);
}

void enemyManager::piccoloShotFire() // ========피콜로 포탄발사
{
	for (_viPiccolo = _vPiccolo.begin(); _viPiccolo != _vPiccolo.end(); ++_viPiccolo)
	{
		if ((*_viPiccolo)->getState() == E_ATTACK && ((*_viPiccolo)->fireDelay2()))
		{
			RECT rc = (*_viPiccolo)->getRect();
			for (int i = 0; i < 2; i++)
			{
				_pBeam->fire(rc.left, (rc.top + rc.bottom) / 2 - 32, PI);
			}
		}
	}
}

//=======================프리저=========================

void enemyManager::setFrieza(float x, float y) // 프리저 소환
{
	enemy* mbFrieza;
	mbFrieza = new midBoss;
	mbFrieza->init(PointMake(x, y), 5000);

	_vFrieza.reserve(1);
	_vFrieza.push_back(mbFrieza);
}

void enemyManager::removeFrieza(int num) // === 프리저 리무브
{
	_vFrieza.erase(_vFrieza.begin() + num);
}

void enemyManager::friezaShotFire() // ========프리저 포탄발사
{
	for (_viFrieza = _vFrieza.begin(); _viFrieza != _vFrieza.end(); ++_viFrieza)
	{
		if ((*_viFrieza)->getState() == E_ATTACK && (*_viFrieza)->getFrameCount() == 3)
		{
			sFrieza.count++;
			RECT rc = (*_viFrieza)->getRect();
			int angle = RND->getFromIntTo(70, 250);
			for (int i = 0; i < 5; i++)
			{
				if (sFrieza.count % 7 == 0)
				_fBullet->fire(rc.left, (rc.top + rc.bottom) / 2, PI2 / 360 * angle + i * PI2 / 360 * 25);
			}
		}
	}
}

void enemyManager::setBuu(float x, float y)
{
	enemy* bBuu;
	bBuu = new boss;
	bBuu->init(PointMake(x, y), 10000);

	_vBuu.reserve(1);
	_vBuu.push_back(bBuu);
}

void enemyManager::removeBuu(int num)
{
	_vBuu.erase(_vBuu.begin() + num);
}

void enemyManager::buuShotFire()
{
	for (_viBuu = _vBuu.begin(); _viBuu != _vBuu.end(); ++_viBuu)
	{
		if ((*_viBuu)->getState() == M_ATTACK && (*_viBuu)->getFrameCount() == 4)
		{
			sBuu.count++;
			RECT rc = (*_viBuu)->getRect();
			int rnd = RND->getFromIntTo(8, 12);
			for (int i = 0; i < 9; i++)
			{
				if (sBuu.count % 7 == 0)
				{
					_bBullet->fire(rc.left + 10, (rc.top + rc.bottom) / 2 + 10, (PI - PI / 180 * 60) + i * (PI2 / 360 * 15), rnd);
				}
			}
		}
		break;
	}
}

void enemyManager::buuAirAttack()
{
	for (_viBuu = _vBuu.begin(); _viBuu != _vBuu.end(); ++_viBuu)
	{
		if ((*_viBuu)->getState() == M_AIRATTACK && (*_viBuu)->getFrameCount() == 6)
		{
			sBuu.count++;
			RECT rc = (*_viBuu)->getRect();
			for (int i = 0; i < 4; i++)
			{
				if (sBuu.count % 10 == 0)
				{
					_bossBullet->fire(rc.left, (rc.top + rc.bottom) / 2, PI - PI / 180 * 60 + i * PI / 180 * 30);
				}
			}
		}
	}
}

void enemyManager::buuChocoAttack()
{
	for (_viBuu = _vBuu.begin(); _viBuu != _vBuu.end(); ++_viBuu)
	{
		if ((*_viBuu)->getState() == M_CHOCOATTACK && (*_viBuu)->getFrameCount() == 3)
		{
			sBuu.count++;
			RECT rc = (*_viBuu)->getRect();
			if (sBuu.count % 10 == 0)
			{
				_cBeam->fire(rc.left, (rc.top + rc.bottom) / 2, getAngle(rc.left, (rc.top+rc.bottom)/2, _goku->getGokuX(), _goku->getGokuY()));
				
				sBuu.count = 0;
			}
		}
		if (((*_viBuu)->getState() == M_CHOCOATTACK))
		{
			sBuu.count2++;
			for (int i = 0; i < _cBeam->getVBullet().size(); i++)
			{
				if (_cBeam->getVBullet()[i].x > WINSIZEX / 2 && _cBeam->getVBullet()[i].x < WINSIZEX / 2 + 50)
				{
					if (sBuu.count2 % 4 == 0)
					{
						for (int j = 0; j < 9; j++)
						{
							_bBullet->fire(_cBeam->getVBullet()[i].x, _cBeam->getVBullet()[i].y,
								getAngle(_cBeam->getVBullet()[i].x, _cBeam->getVBullet()[i].y, _goku->getGokuX(), _goku->getGokuY())
								+ j * PI2 / 360 * 40, 10);
						}
					}
				}
			}
		}
	}
}

void enemyManager::buuSkyBulletFire(float x, float y)
{
	for (_viBuu = _vBuu.begin(); _viBuu != _vBuu.end(); ++_viBuu)
	{
		if ((*_viBuu)->getState() == M_ROLLATTACK)
		{
			sBuu.count++;
			for (int i = 0; i < 4; i++)
			{
				if (sBuu.count % 70 == 0)
					_bossBullet2->fire(x, y, PI2 / 360 * 90 + i * PI2 / 360 * 90);
			}
		}
	}
}

void enemyManager::guideFire()
{
	for (_viBuu = _vBuu.begin(); _viBuu != _vBuu.end(); ++_viBuu)
	{
		if ((*_viBuu)->getState() == M_ROLLATTACK) _guide->fire(WINSIZEX / 2, WINSIZEY / 2);
	}
}

void enemyManager::collision() // =================충돌함수
{
	for (int i = 0; i < _vSatan.size(); i++) // ==============사탄과 플레이어 충돌
	{
		RECT temp;
		RECT satan = _vSatan[i]->getRect();
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &satan, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE && _goku->getState() != SKILL
			&& !_goku->getInvincible())
		{
			if (_vSatan[i]->getState() != SATAN_DIE) _goku->hitDamage(1);
			_vSatan[i]->setState(SATAN_DIE);
		}
	}

	for (int i = 0; i < _stMissile->getVBullet().size(); i++) // ============사탄의 총알과 플레이어 충돌
	{
		RECT temp;
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &_stMissile->getVBullet()[i].rc, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE && 
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			_goku->hitDamage(1);
			_stMissile->remove(i);
			break;
		}
	}

	for (int i = 0; i < _stMissile->getVBullet().size(); i++) // ===========사탄의 총알과 플레이어 스킬 충돌
	{
		for (int j = 0; j < _goku->getWave()->getVWave().size(); j++)
		{
			RECT temp;
			RECT wave = _goku->getWave()->getVWave()[j].rc;

			if (IntersectRect(&temp, &_stMissile->getVBullet()[i].rc, &wave))
			{
				_stMissile->remove(i);
				break;
			}
		}
	}

	for (int i = 0; i < _vKrillin.size(); i++) // ==============크리링과 플레이어 충돌
	{
		RECT temp;
		RECT krillin = _vKrillin[i]->getRect();
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &krillin, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE && 
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			if (_vKrillin[i]->getState() != K_DIE) _goku->hitDamage(1);
			_vKrillin[i]->setState(K_DIE);
		}
	}

	for (int i = 0; i < _bBullet->getVBullet().size(); i++)  // ============크리링의 총알과 플레이어 충돌
	{
		RECT temp;
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &_bBullet->getVBullet()[i].rc, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE && 
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			_goku->hitDamage(1);
			_bBullet->remove(i);
			break;
		}
	}

	for (int i = 0; i < _bBullet->getVBullet().size(); i++) // ===========크리링의 총알과 플레이어 스킬 충돌
	{
		for (int j = 0; j < _goku->getWave()->getVWave().size(); j++)
		{
			RECT temp;
			RECT wave = _goku->getWave()->getVWave()[j].rc;

			if (IntersectRect(&temp, &_bBullet->getVBullet()[i].rc, &wave))
			{
				_bBullet->remove(i);
				break;
			}
		}
	}

	for (int i = 0; i < _vPiccolo.size(); i++) // ==============피콜로와 플레이어 충돌
	{
		RECT temp;
		RECT piccolo = _vPiccolo[i]->getRect();
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &piccolo, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE && 
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			if (_vPiccolo[i]->getState() != P_DIE) _goku->hitDamage(1);
			_vPiccolo[i]->setState(P_DIE);
		}
	}

	for (int i = 0; i < _pBeam->getVBullet().size(); i++) // ============피콜로의 총알과 플레이어 충돌
	{
		RECT temp;
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &_pBeam->getVBullet()[i].rc, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE &&
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			_goku->hitDamage(1);
			_pBeam->remove(i);
			break;
		}
	}

	for (int i = 0; i < _pBeam->getVBullet().size(); i++) // ===========피콜로의 총알과 플레이어 스킬 충돌
	{
		for (int j = 0; j < _goku->getWave()->getVWave().size(); j++)
		{
			RECT temp;
			RECT wave = _goku->getWave()->getVWave()[j].rc;

			if (IntersectRect(&temp, &_pBeam->getVBullet()[i].rc, &wave))
			{
				_pBeam->remove(i);
				break;
			}
		}
	}

	for (int i = 0; i < _fBullet->getVBullet().size(); i++) // ==============프리저의 총알과 플레이어 충돌
	{
		RECT temp;
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &_fBullet->getVBullet()[i].rc, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE && 
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			_goku->hitDamage(1);
			_fBullet->remove(i);
			break;
		}
	}

	for (int i = 0; i < _fBullet->getVBullet().size(); i++) // ============프리저의 총알과 플레이어 스킬 충돌
	{
		for (int j = 0; j < _goku->getWave()->getVWave().size(); j++)
		{
			RECT temp;
			RECT wave = _goku->getWave()->getVWave()[j].rc;

			if (IntersectRect(&temp, &_fBullet->getVBullet()[i].rc, &wave))
			{
				_fBullet->remove(i);
				break;
			}
		}
	}

	for (int i = 0; i < _bossBullet->getVBullet().size(); i++) // ==============마인부우의 총알과 플레이어 충돌
	{
		RECT temp;
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &_bossBullet->getVBullet()[i].rc, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE &&
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			_goku->hitDamage(1);
			_bossBullet->remove(i);
			break;
		}
	}

	for (int i = 0; i < _bossBullet->getVBullet().size(); i++) // ============마인부우의 총알과 플레이어 스킬 충돌
	{
		for (int j = 0; j < _goku->getWave()->getVWave().size(); j++)
		{
			RECT temp;
			RECT wave = _goku->getWave()->getVWave()[j].rc;

			if (IntersectRect(&temp, &_bossBullet->getVBullet()[i].rc, &wave))
			{
				_bossBullet->remove(i);
				break;
			}
		}
	}

	for (int i = 0; i < _cBeam->getVBullet().size(); i++) // ============마인부우의 총알과 플레이어 충돌
	{
		RECT temp;
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &_cBeam->getVBullet()[i].rc, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE && 
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			if (_goku->getState() != CHOCO) _goku->setState(CHOCO);
			_cBeam->remove(i);
			break;
		}
	}

	for (int i = 0; i < _cBeam->getVBullet().size(); i++) // ============마인부우의 총알과 플레이어 스킬 충돌
	{
		for (int j = 0; j < _goku->getWave()->getVWave().size(); j++)
		{
			RECT temp;
			RECT wave = _goku->getWave()->getVWave()[j].rc;

			if (IntersectRect(&temp, &_cBeam->getVBullet()[i].rc, &wave))
			{
				_cBeam->remove(i);
				break;
			}
		}
	}

	for (int i = 0; i < _bossBullet2->getVBullet().size(); i++) // ============마인부우의 총알과 플레이어 충돌
	{
		RECT temp;
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &_bossBullet2->getVBullet()[i].rc, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE &&
			_goku->getState() != SKILL && !_goku->getInvincible())
		{
			_goku->hitDamage(1);
			_bossBullet2->remove(i);
			break;
		}
	}

	for (int i = 0; i < _bossBullet2->getVBullet().size(); i++) // ============마인부우의 총알과 플레이어 스킬 충돌
	{
		for (int j = 0; j < _goku->getWave()->getVWave().size(); j++)
		{
			RECT temp;
			RECT wave = _goku->getWave()->getVWave()[j].rc;

			if (IntersectRect(&temp, &_bossBullet2->getVBullet()[i].rc, &wave))
			{
				_bossBullet2->remove(i);
				break;
			}
		}
	}

	for (int i = 0; i < _vBuu.size(); i++) // ============마인부우 페이즈 4일때 마인부우와 플레이어 충돌
	{
		RECT temp;
		RECT buu = _vBuu[i]->getRect();
		RECT goku = _goku->getHitBox();

		if (IntersectRect(&temp, &buu, &goku) && _goku->getState() != TELEPOTE && _goku->getState() != R_TELEPOTE &&
			_goku->getState() != SKILL && !_goku->getInvincible() && _vBuu[i]->getState() == M_ROLLATTACK)
		{
			_goku->hitDamage(1);
		}
	}
}

