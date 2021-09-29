#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{
    IMAGEMANAGER->addFrameImage("오공기본", "image/goku.bmp", 150, 83, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("오공전진", "image/gokuFront1.bmp", 50, 83, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("오공후진", "image/gokuBack1.bmp", 50, 83, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("오공공격", "image/gokuAttack.bmp", 252, 83, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("오공스킬", "image/gokuWave.bmp", 232, 83, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("오공다운", "image/gokuDie.bmp", 440, 82, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("텔레포트", "image/gokuTelepote.bmp", 102, 83, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("역텔레포트", "image/gokuTelepoteR.bmp", 102, 83, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("초코", "image/choco.bmp", 40, 67, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("하트", "image/heart.bmp", 42, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("빈하트", "image/nullHeart.bmp", 42, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("고기", "image/meat.bmp", 64, 40, true, RGB(255, 0, 255));

    _x = -100;
    _y = WINSIZEY / 2;
	_maxHP = _currentHP = 5;
	_currentFrameX = _currentFrameY = _frameCount = _skillCount = 0;
	_score = 0;
	_skillGauge = 3;
	_bulletLevel = 0;
	_alphaValue = 255;
	_skillDamage = 10;
    _rc = RectMakeCenter(_x, _y, 50, 83);
	_hitBox = RectMakeCenter(_x, _y, 25, 25);
	_isRevival = true;
	_isInvincible = false;
	_isSkillOn = false;

	_egBullet = new energyBullet;
	_egBullet->init(WINSIZEX, 100);

	_wave = new energyWave;
	_wave->init();

	_upItem = new upgradeItem;
	_upItem->init(10);
	_hpItem = new hpItem;
	_hpItem->init(1);
	_skillItem = new skillItem;
	_skillItem->init(3);

	_gameOver = false;

	char str[128];
	for (int i = 0; i < 10; i++)
	{
		sprintf_s(str, "score/%d.bmp", i);
		_number[i] = new image;
		_number[i]->init(str, 40, 47, true, RGB(255, 0, 255));
	}

	for (int i = 0; i < 5; i++) // UI 아이콘 설정
	{
		_HP[i] = IMAGEMANAGER->findImage("하트");
	}

	for (int i = 0; i < 5; i++)
	{
		_nullHP[i] = IMAGEMANAGER->findImage("빈하트");
	}

	for (int i = 0; i < 3; i++)
	{
		_skillMeat[i] = IMAGEMANAGER->findImage("고기");
	}
    return S_OK;
}

void player::release()
{
}

void player::update()
{
	if (_isRevival)
	{
		_isInvincible = true;
		_x += 5;
		_state = FRONT;
		if (_x >= WINSIZEX / 4)
		{
			_gameOver = false;
			_isRevival = false;
			_state = IDLE;
			_image = IMAGEMANAGER->findImage("오공기본");
		}
	}
	if (!_isRevival)
	{
		move();
	}

	if (_bulletLevel < 2) _damage = 10;
	if (_bulletLevel >= 2) _damage = 20;

	frameImage();
	
	_egBullet->update(_bulletLevel);
	_wave->update();

    _rc = RectMakeCenter(_x, _y, 50, 83);
	_hitBox = RectMakeCenter(_x, _y, 25, 25);
	_upItem->update();
	_hpItem->update();
	_skillItem->update();
	collision();

	if (_isInvincible) // 무적상태일시 알파블렌더로 깜빡임 효과
	{
		_alphaCount++;

		if (_alphaCount % 10 == 0)
		{
			_alphaValue += 170;
			if (_alphaValue > 255) _alphaValue = 0;
		}

		if (_alphaCount > 250)
		{
			_isInvincible = false;
			_alphaValue = 255;
			_alphaCount = 0;
		}
	}

	if (_state == R_TELEPOTE) // 스킬 사용시 사용위치로 플레이어 좌표 이동
	{
		_x = WINSIZEX / 4 - 20;
		_y = WINSIZEY / 2 + 15;
	}

	if (_state == CHOCO)
	{
		_chocoCoolTime++;
		if (_chocoCoolTime > 100)
		{
			_chocoCoolTime = 0;
			_state = IDLE;
		}
	}

	if (_currentHP <= 0) _state = DIE;
	if (_gameOver)
	{
		SCENEMANAGER->changeScene("패배");
	}
}

void player::render()
{
	if (_state == IDLE)
	{
		_image->frameAlphaRender(getMemDC(), _rc.left, _rc.top, _alphaValue, _currentFrameX, 0);
	}
	if (_state == FRONT || _state == BACK)
	{
		_image->alphaRender(getMemDC(), _rc.left, _rc.top, _alphaValue);
	}
	if (_state == ATTACK)
	{
		_image->frameAlphaRender(getMemDC(), _rc.left, _rc.top, _alphaValue, _currentFrameX, 0);
	}
	if (_state == SKILL)
	{
		_image->frameAlphaRender(getMemDC(), _rc.left, _rc.top, _alphaValue, _currentFrameX, 0);
	}
	if (_state == TELEPOTE)
	{
		_image->frameRender(getMemDC(), _rc.left, _rc.top, _telpoFrameX, 0);
	}
	if (_state == R_TELEPOTE)
	{
		_image->frameRender(getMemDC(), _rc.left, _rc.top, _telpoRFrameX, 0);
	}
	if (_state == DIE)
	{
		_image->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, 0);
	}
	if (_state == CHOCO)
	{
		_image->render(getMemDC(), _rc.left, _rc.top);
	}

	_egBullet->render();
	_wave->render();
	_upItem->render();
	_hpItem->render();
	_skillItem->render();

	if (_score < 10)
	{
		_number[_score]->render(getMemDC(), 200, 50);
	}
	else if (10 <= _score && 100 > _score)
	{
		_number[_score / 10]->render(getMemDC(), 170, 50);
		_number[_score % 10]->render(getMemDC(), 200, 50);
	}
	else if (100 <= _score && 1000 > _score)
	{
		_number[_score / 100]->render(getMemDC(), 140, 50);
		_number[_score % 100 / 10]->render(getMemDC(), 170, 50);
		_number[_score % 10]->render(getMemDC(), 200, 50);
	}
	else if (1000 <= _score && 10000 > _score)
	{
		_number[_score / 1000]->render(getMemDC(), 110, 50);
		_number[_score % 1000 / 100]->render(getMemDC(), 140, 50);
		_number[_score % 100 / 10]->render(getMemDC(), 170, 50);
		_number[_score % 10]->render(getMemDC(), 200, 50);
	}

	for (int i = 0; i < _maxHP; i++)
	{
		_nullHP[i]->render(getMemDC(), 30 + i * 40, WINSIZEY - 50);
	}
	
	for (int i = 0; i < _currentHP; i++)
	{
		_HP[i]->render(getMemDC(), 30 + i * 40, WINSIZEY - 50);
	}

	for (int i = 0; i < _skillGauge; i++)
	{
		_skillMeat[i]->render(getMemDC(), 30 + i * 70, WINSIZEY - 100);
	}

}

void player::move()
{
	++_fireCount;
	if (_state != TELEPOTE && _state != R_TELEPOTE && _state != SKILL && _state != CHOCO && _state != DIE)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _rc.left > 0)
		{
			_x -= 5;
			_state = BACK;
			if (_state == ATTACK) _state = ATTACK;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
		{
			_currentFrameX = 0;
			_state = IDLE;
			_image = IMAGEMANAGER->findImage("오공기본");
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _rc.right < WINSIZEX)
		{
			_x += 5;
			_state = FRONT;
			if (_state == ATTACK) _state = ATTACK;
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
		{
			_currentFrameX = 0;
			_state = IDLE;
			_image = IMAGEMANAGER->findImage("오공기본");
		}
		if (KEYMANAGER->isStayKeyDown(VK_UP) && _rc.top > 0)
		{
			_currentFrameX = 0;
			//_state = IDLE;
			_y -= 5;
		}
		if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _rc.bottom < WINSIZEY)
		{
			_currentFrameX = 0;
			//_state = IDLE;
			_y += 5;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE)) // 단발용
		{
			_currentFrameX = 0;
			_image = IMAGEMANAGER->findImage("오공공격");
			_state = ATTACK;

			if (_bulletLevel == 0) _egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, 0); // 0단계시 총알

			else if (_bulletLevel == 1)														  // 1단계시 총알
			{
				for (int i = 0; i < 3; i++)
				{
					_egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, (PI / 180 * -20) + i * PI / 180 * 20);
				}
			}
			else if (_bulletLevel == 2)														  // 2단계시 총알
			{
				for (int i = 0; i < 5; i++)
				{
					_egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, (PI / 180 * -30) + i * PI / 180 * 15);
				}
			}
		}
		if (KEYMANAGER->isStayKeyDown('X')) // 연사용
		{
			_currentFrameX = 0;
			_image = IMAGEMANAGER->findImage("오공공격");
			_state = ATTACK;
			if (_fireCount % 15 == 0)
			{
				if (_bulletLevel == 0) _egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, 0); // 0단계시 총알

				else if (_bulletLevel == 1)														  // 1단계시 총알
				{
					for (int i = 0; i < 3; i++)
					{
						_egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, (PI / 180 * -20) + i * PI / 180 * 20);
					}
				}
				else if (_bulletLevel == 2)														  // 2단계시 총알
				{
					for (int i = 0; i < 5; i++)
					{
						_egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, (PI / 180 * -30) + i * PI / 180 * 15);
					}
				}
				_fireCount = 0;
			}
		}

		if (KEYMANAGER->isOnceKeyDown('Z') && _skillGauge > 0) // 스킬
		{
			_wave->setSkillCount(0);
			_currentFrameX = 0;
			_skillStartCount = 0;
			_state = TELEPOTE;
			_isSkillOn = true;
			_skillGauge--;
		}
	}

	if (_isSkillOn)
	{
		_skillStartCount++;
		if (_skillStartCount == 100)
		{
			_wave->fire(-100 , WINSIZEY / 2);
		}
	}
}

void player::frameImage() // 상태별 이미지 생성 및 재생
{
	if (_state == IDLE)
	{
		_frameCount++;
		if (_frameCount % 20 == 0)
		{
			_image = IMAGEMANAGER->findImage("오공기본");
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			_currentFrameX++;
			if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;

			_frameCount = 0;
		}
	}

	if (_state == FRONT)
	{
		_image = IMAGEMANAGER->findImage("오공전진");
	}
	if (_state == BACK)
	{
		_image = IMAGEMANAGER->findImage("오공후진");
	}

	if (_state == ATTACK)
	{
		_frameCount++;
		if (_frameCount % 10 == 0)
		{
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			_currentFrameX++;

			if (_currentFrameX >= 2)
			{
				_state = IDLE;
			}
			_frameCount = 0;
		}
	}

	if (_state == TELEPOTE)
	{
		_image = IMAGEMANAGER->findImage("텔레포트");
		_telpoCount++;
		if (_telpoCount % 12 == 0)
		{
			_image->setFrameY(0);
			_image->setFrameX(_telpoFrameX);

			_telpoFrameX++;

			if (_telpoFrameX >= _image->getMaxFrameX())
			{
				_telpoFrameX = 3;
				if (_telpoCount > 45)
				{
					_state = R_TELEPOTE;
					_telpoCount = 0;
					_telpoFrameX = 0;
				}
			}
		}
	}

	if (_state == R_TELEPOTE)
	{
		_image = IMAGEMANAGER->findImage("역텔레포트");
		_telpoRCount++;
		if (_telpoRCount % 12 == 0)
		{
			_image->setFrameY(0);
			_image->setFrameX(_telpoRFrameX);

			_telpoRFrameX++;
			if (_telpoRFrameX >= _image->getMaxFrameX())
			{
				_telpoRFrameX = 2;
				if (_telpoRCount > 45)
				{
					_state = SKILL;
					_image = IMAGEMANAGER->findImage("오공스킬");
					_telpoRCount = 0;
					_telpoRFrameX = 0;
				}
			}
		}
	}

	if (_state == SKILL)
	{
		_image = IMAGEMANAGER->findImage("오공스킬");
		_frameCount++;
		_skillCount++;
		if (_frameCount % 10 == 0)
		{
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			_currentFrameX++;

			if (_currentFrameX >= _image->getMaxFrameX())
			{
				_currentFrameX = 2;
			}
			_frameCount = 0;
		}
		if (_skillCount > 310)
		{
			_state = IDLE;
			_skillCount = 0;
			for (int i = 0; i < _wave->getVWave().size(); i++)
			{
				_wave->remove(i);
				_wave->setFrameY(0);
				_isSkillOn = false;
			}
		}
	}

	if (_state == DIE)
	{
		_image = IMAGEMANAGER->findImage("오공다운");
		_frameCount++;
		if (_frameCount % 10 == 0)
		{
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			_currentFrameX++;

			if (_currentFrameX >= _image->getMaxFrameX())
			{
				_currentFrameX = _image->getMaxFrameX() + 1;
				_x = -100;
				_y = WINSIZEY / 2;

				_gameOver = true;
			}
			_frameCount = 0;
		}
	}

	if (_state == CHOCO)
	{
		_image = IMAGEMANAGER->findImage("초코");
	}
}

void player::hitDamage(int damage)
{
	_currentHP -= damage;
	if (_state != CHOCO) _isInvincible = false;
}

void player::collision() // ======================================충돌관련
{
	for (int i = 0; i < _em->getVSatan().size(); i++) //================= 사탄과 충돌
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT satan = _em->getVSatan()[i]->getRect();
			if (satan.right < WINSIZEX && _em->getVSatan()[i]->getState() != 3) // 사탄이 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_egBullet->getVEgBullet()[j].rc, &satan))
				{
					_egBullet->remove(j);
					_em->getVSatan()[i]->hitDamage(_damage);
					if (_em->getVSatan()[i]->getHP() <= 0)
					{
						int rnd = RND->getInt(20);
						if (rnd < 3) _upItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						if (rnd >= 3 && rnd < 5)_hpItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						if (rnd >= 5 && rnd < 7)_skillItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						_em->getVSatan()[i]->setState(3);
						_score += 10;
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVSatan().size(); i++) //================= 사탄과 스킬 충돌
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT satan = _em->getVSatan()[i]->getRect();
			if (satan.right < WINSIZEX && _em->getVSatan()[i]->getState() != 3) // 사탄이 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_wave->getVWave()[j].rc, &satan))
				{
					_em->getVSatan()[i]->hitDamage(_skillDamage);
					if (_em->getVSatan()[i]->getHP() <= 0)
					{
						int rnd = RND->getInt(20);
						if (rnd < 3) _upItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						if (rnd >= 3 && rnd < 5)_hpItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						if (rnd >= 5 && rnd < 7)_skillItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						_em->getVSatan()[i]->setState(3);
						_score += 10;
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVKrillin().size(); i++) //================= 크리링과 충돌
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT krillin = _em->getVKrillin()[i]->getRect();
			if (krillin.right < WINSIZEX && _em->getVKrillin()[i]->getState() != 3) // 크리링이 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_egBullet->getVEgBullet()[j].rc, &krillin))
				{
					_egBullet->remove(j);
					_em->getVKrillin()[i]->hitDamage(_damage);
					if (_em->getVKrillin()[i]->getHP() <= 0)
					{
						int rnd = RND->getInt(20);
						if (rnd < 3) _upItem->create(_em->getVKrillin()[i]->getX(), _em->getVKrillin()[i]->getY(), PI / 180 * 120);
						if (rnd >= 3 && rnd < 5)_hpItem->create(_em->getVKrillin()[i]->getX(), _em->getVKrillin()[i]->getY(), PI / 180 * 120);
						if (rnd >= 5 && rnd < 7)_skillItem->create(_em->getVKrillin()[i]->getX(), _em->getVKrillin()[i]->getY(), PI / 180 * 120);
						_em->getVKrillin()[i]->setState(3);
						_score += 10;
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVKrillin().size(); i++) //================= 크리링과 스킬 충돌
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT krillin = _em->getVKrillin()[i]->getRect();
			if (krillin.right < WINSIZEX && _em->getVKrillin()[i]->getState() != 3) // 크리링이 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_wave->getVWave()[j].rc, &krillin))
				{
					_em->getVKrillin()[i]->hitDamage(_skillDamage);
					if (_em->getVKrillin()[i]->getHP() <= 0)
					{
						int rnd = RND->getInt(20);
						if (rnd < 3) _upItem->create(_em->getVKrillin()[i]->getX(), _em->getVKrillin()[i]->getY(), PI / 180 * 120);
						if (rnd >= 3 && rnd < 5)_hpItem->create(_em->getVKrillin()[i]->getX(), _em->getVKrillin()[i]->getY(), PI / 180 * 120);
						if (rnd >= 5 && rnd < 7)_skillItem->create(_em->getVKrillin()[i]->getX(), _em->getVKrillin()[i]->getY(), PI / 180 * 120);
						_em->getVKrillin()[i]->setState(3);
						_score += 10;
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVPiccolo().size(); i++) //================= 피콜로와 충돌
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT piccolo = _em->getVPiccolo()[i]->getRect();
			if (piccolo.right < WINSIZEX && _em->getVPiccolo()[i]->getState() != 3) // 피콜로가 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_egBullet->getVEgBullet()[j].rc, &piccolo))
				{
					_egBullet->remove(j);
					_em->getVPiccolo()[i]->hitDamage(_damage);
					if (_em->getVPiccolo()[i]->getHP() <= 0)
					{
						int rnd = RND->getInt(20);
						if (rnd < 3) _upItem->create(_em->getVPiccolo()[i]->getX(), _em->getVPiccolo()[i]->getY(), PI / 180 * 120);
						if (rnd >= 3 && rnd < 5)_hpItem->create(_em->getVPiccolo()[i]->getX(), _em->getVPiccolo()[i]->getY(), PI / 180 * 120);
						if (rnd >= 5 && rnd < 7)_skillItem->create(_em->getVPiccolo()[i]->getX(), _em->getVPiccolo()[i]->getY(), PI / 180 * 120);
						_em->getVPiccolo()[i]->setState(3);
						_score += 10;
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVPiccolo().size(); i++) //================= 피콜로와 스킬 충돌
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT piccolo = _em->getVPiccolo()[i]->getRect();
			if (piccolo.right < WINSIZEX && _em->getVPiccolo()[i]->getState() != 3) // 피콜로가 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_wave->getVWave()[j].rc, &piccolo))
				{
					_em->getVPiccolo()[i]->hitDamage(_skillDamage);
					if (_em->getVPiccolo()[i]->getHP() <= 0)
					{
						int rnd = RND->getInt(20);
						if (rnd < 3) _upItem->create(_em->getVPiccolo()[i]->getX(), _em->getVPiccolo()[i]->getY(), PI / 180 * 120);
						if (rnd >= 3 && rnd < 5)_hpItem->create(_em->getVPiccolo()[i]->getX(), _em->getVPiccolo()[i]->getY(), PI / 180 * 120);
						if (rnd >= 5 && rnd < 7)_skillItem->create(_em->getVPiccolo()[i]->getX(), _em->getVPiccolo()[i]->getY(), PI / 180 * 120);
						_em->getVPiccolo()[i]->setState(3);
						_score += 10;
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVFrieza().size(); i++) //================= 프리저와 충돌
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT frieza = _em->getVFrieza()[i]->getRect();
			if (frieza.right < WINSIZEX && _em->getVFrieza()[i]->getState() != 3) // 프리저가 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_egBullet->getVEgBullet()[j].rc, &frieza))
				{
					_egBullet->remove(j);
					_em->getVFrieza()[i]->hitDamage(_damage);
					if (_em->getVFrieza()[i]->getHP() <= 0)
					{
						_hpItem->create(_em->getVFrieza()[i]->getX(), _em->getVFrieza()[i]->getY(), PI / 180 * 120);
						_skillItem->create(_em->getVFrieza()[i]->getX(), _em->getVFrieza()[i]->getY(), PI / 180 * 120);
						_em->getVFrieza()[i]->setState(3);
						_score += 100;
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVFrieza().size(); i++) //================= 프리저와 스킬 충돌
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT frieza = _em->getVFrieza()[i]->getRect();
			if (frieza.right < WINSIZEX && _em->getVFrieza()[i]->getState() != 3) // 프리저 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_wave->getVWave()[j].rc, &frieza))
				{
					_em->getVFrieza()[i]->hitDamage(_skillDamage);
					if (_em->getVFrieza()[i]->getHP() <= 0)
					{
						_hpItem->create(_em->getVFrieza()[i]->getX(), _em->getVFrieza()[i]->getY(), PI / 180 * 120);
						_skillItem->create(_em->getVFrieza()[i]->getX(), _em->getVFrieza()[i]->getY(), PI / 180 * 120);
						_em->getVFrieza()[i]->setState(3);
						_score += 100;
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVBuu().size(); i++) //================= 마인부우와 충돌
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT buu = _em->getVBuu()[i]->getRect();
			if (buu.right < WINSIZEX)
			{
				if (IntersectRect(&temp, &_egBullet->getVEgBullet()[j].rc, &buu))
				{
					_egBullet->remove(j);
					_em->getVBuu()[i]->hitDamage(_damage);
					if (_em->getVBuu()[i]->getHP() <= 0)
					{
						_em->getVBuu()[i]->setState(M_DIE);
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < _em->getVBuu().size(); i++) //================= 마인부우와 스킬 충돌
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT buu = _em->getVBuu()[i]->getRect();
			if (buu.right < WINSIZEX && _em->getVBuu()[i]->getState() != 3) // 마인부우가 화면 밖에 있거나 죽는 이미지 재생시 충돌 안하도록 예외처리
			{
				if (IntersectRect(&temp, &_wave->getVWave()[j].rc, &buu))
				{
					_em->getVBuu()[i]->hitDamage(_skillDamage);
					if (_em->getVBuu()[i]->getHP() <= 0)
					{
						//_upItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						//_hpItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						//_skillItem->create(_em->getVSatan()[i]->getX(), _em->getVSatan()[i]->getY(), PI / 180 * 120);
						_em->getVBuu()[i]->setState(M_DIE);
					}
					break;
				}
			}
		}
	}

	//==================================== 아이템과 충돌 =======================================

	for (int i = 0; i < _upItem->getVItem().size(); i++)  // 총알 업글 아이템
	{
		RECT temp;

		if (IntersectRect(&temp, &_rc, &_upItem->getVItem()[i].rc))
		{
			_upItem->remove(i);
			_bulletLevel++;
			if (_bulletLevel > 2)
			{
				_bulletLevel = 2;
				_score += 10;
			}
			break;
		}
	}

	for (int i = 0; i < _hpItem->getVItem().size(); i++) // HP 아이템
	{
		RECT temp;

		if (IntersectRect(&temp, &_rc, &_hpItem->getVItem()[i].rc))
		{
			_hpItem->remove(i);
			_currentHP++;
			if (_currentHP > 5)
			{
				_currentHP = 5;
				_score += 10;
			}
			break;
		}
	}

	for (int i = 0; i < _skillItem->getVItem().size(); i++) // 스킬 아이템
	{
		RECT temp;

		if (IntersectRect(&temp, &_rc, &_skillItem->getVItem()[i].rc))
		{
			_skillItem->remove(i);
			_skillGauge++;
			if (_skillGauge > 3)
			{
				_skillGauge = 3;
				_score += 10;
			}
			break;
		}
	}
}
