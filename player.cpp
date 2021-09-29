#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{
    IMAGEMANAGER->addFrameImage("�����⺻", "image/goku.bmp", 150, 83, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������", "image/gokuFront1.bmp", 50, 83, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������", "image/gokuBack1.bmp", 50, 83, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("��������", "image/gokuAttack.bmp", 252, 83, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("������ų", "image/gokuWave.bmp", 232, 83, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�����ٿ�", "image/gokuDie.bmp", 440, 82, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("�ڷ���Ʈ", "image/gokuTelepote.bmp", 102, 83, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("���ڷ���Ʈ", "image/gokuTelepoteR.bmp", 102, 83, 3, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("����", "image/choco.bmp", 40, 67, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��Ʈ", "image/heart.bmp", 42, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("����Ʈ", "image/nullHeart.bmp", 42, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���", "image/meat.bmp", 64, 40, true, RGB(255, 0, 255));

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

	for (int i = 0; i < 5; i++) // UI ������ ����
	{
		_HP[i] = IMAGEMANAGER->findImage("��Ʈ");
	}

	for (int i = 0; i < 5; i++)
	{
		_nullHP[i] = IMAGEMANAGER->findImage("����Ʈ");
	}

	for (int i = 0; i < 3; i++)
	{
		_skillMeat[i] = IMAGEMANAGER->findImage("���");
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
			_image = IMAGEMANAGER->findImage("�����⺻");
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

	if (_isInvincible) // ���������Ͻ� ���ĺ����� ������ ȿ��
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

	if (_state == R_TELEPOTE) // ��ų ���� �����ġ�� �÷��̾� ��ǥ �̵�
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
		SCENEMANAGER->changeScene("�й�");
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
			_image = IMAGEMANAGER->findImage("�����⺻");
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
			_image = IMAGEMANAGER->findImage("�����⺻");
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
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE)) // �ܹ߿�
		{
			_currentFrameX = 0;
			_image = IMAGEMANAGER->findImage("��������");
			_state = ATTACK;

			if (_bulletLevel == 0) _egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, 0); // 0�ܰ�� �Ѿ�

			else if (_bulletLevel == 1)														  // 1�ܰ�� �Ѿ�
			{
				for (int i = 0; i < 3; i++)
				{
					_egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, (PI / 180 * -20) + i * PI / 180 * 20);
				}
			}
			else if (_bulletLevel == 2)														  // 2�ܰ�� �Ѿ�
			{
				for (int i = 0; i < 5; i++)
				{
					_egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, (PI / 180 * -30) + i * PI / 180 * 15);
				}
			}
		}
		if (KEYMANAGER->isStayKeyDown('X')) // �����
		{
			_currentFrameX = 0;
			_image = IMAGEMANAGER->findImage("��������");
			_state = ATTACK;
			if (_fireCount % 15 == 0)
			{
				if (_bulletLevel == 0) _egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, 0); // 0�ܰ�� �Ѿ�

				else if (_bulletLevel == 1)														  // 1�ܰ�� �Ѿ�
				{
					for (int i = 0; i < 3; i++)
					{
						_egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, (PI / 180 * -20) + i * PI / 180 * 20);
					}
				}
				else if (_bulletLevel == 2)														  // 2�ܰ�� �Ѿ�
				{
					for (int i = 0; i < 5; i++)
					{
						_egBullet->fire(_rc.right, (_rc.top + _rc.bottom) / 2, (PI / 180 * -30) + i * PI / 180 * 15);
					}
				}
				_fireCount = 0;
			}
		}

		if (KEYMANAGER->isOnceKeyDown('Z') && _skillGauge > 0) // ��ų
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

void player::frameImage() // ���º� �̹��� ���� �� ���
{
	if (_state == IDLE)
	{
		_frameCount++;
		if (_frameCount % 20 == 0)
		{
			_image = IMAGEMANAGER->findImage("�����⺻");
			_image->setFrameY(0);
			_image->setFrameX(_currentFrameX);

			_currentFrameX++;
			if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;

			_frameCount = 0;
		}
	}

	if (_state == FRONT)
	{
		_image = IMAGEMANAGER->findImage("��������");
	}
	if (_state == BACK)
	{
		_image = IMAGEMANAGER->findImage("��������");
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
		_image = IMAGEMANAGER->findImage("�ڷ���Ʈ");
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
		_image = IMAGEMANAGER->findImage("���ڷ���Ʈ");
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
					_image = IMAGEMANAGER->findImage("������ų");
					_telpoRCount = 0;
					_telpoRFrameX = 0;
				}
			}
		}
	}

	if (_state == SKILL)
	{
		_image = IMAGEMANAGER->findImage("������ų");
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
		_image = IMAGEMANAGER->findImage("�����ٿ�");
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
		_image = IMAGEMANAGER->findImage("����");
	}
}

void player::hitDamage(int damage)
{
	_currentHP -= damage;
	if (_state != CHOCO) _isInvincible = false;
}

void player::collision() // ======================================�浹����
{
	for (int i = 0; i < _em->getVSatan().size(); i++) //================= ��ź�� �浹
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT satan = _em->getVSatan()[i]->getRect();
			if (satan.right < WINSIZEX && _em->getVSatan()[i]->getState() != 3) // ��ź�� ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	for (int i = 0; i < _em->getVSatan().size(); i++) //================= ��ź�� ��ų �浹
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT satan = _em->getVSatan()[i]->getRect();
			if (satan.right < WINSIZEX && _em->getVSatan()[i]->getState() != 3) // ��ź�� ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	for (int i = 0; i < _em->getVKrillin().size(); i++) //================= ũ������ �浹
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT krillin = _em->getVKrillin()[i]->getRect();
			if (krillin.right < WINSIZEX && _em->getVKrillin()[i]->getState() != 3) // ũ������ ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	for (int i = 0; i < _em->getVKrillin().size(); i++) //================= ũ������ ��ų �浹
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT krillin = _em->getVKrillin()[i]->getRect();
			if (krillin.right < WINSIZEX && _em->getVKrillin()[i]->getState() != 3) // ũ������ ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	for (int i = 0; i < _em->getVPiccolo().size(); i++) //================= ���ݷο� �浹
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT piccolo = _em->getVPiccolo()[i]->getRect();
			if (piccolo.right < WINSIZEX && _em->getVPiccolo()[i]->getState() != 3) // ���ݷΰ� ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	for (int i = 0; i < _em->getVPiccolo().size(); i++) //================= ���ݷο� ��ų �浹
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT piccolo = _em->getVPiccolo()[i]->getRect();
			if (piccolo.right < WINSIZEX && _em->getVPiccolo()[i]->getState() != 3) // ���ݷΰ� ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	for (int i = 0; i < _em->getVFrieza().size(); i++) //================= �������� �浹
	{
		for (int j = 0; j < _egBullet->getVEgBullet().size(); j++)
		{
			RECT temp;
			RECT frieza = _em->getVFrieza()[i]->getRect();
			if (frieza.right < WINSIZEX && _em->getVFrieza()[i]->getState() != 3) // �������� ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	for (int i = 0; i < _em->getVFrieza().size(); i++) //================= �������� ��ų �浹
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT frieza = _em->getVFrieza()[i]->getRect();
			if (frieza.right < WINSIZEX && _em->getVFrieza()[i]->getState() != 3) // ������ ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	for (int i = 0; i < _em->getVBuu().size(); i++) //================= ���κο�� �浹
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

	for (int i = 0; i < _em->getVBuu().size(); i++) //================= ���κο�� ��ų �浹
	{
		for (int j = 0; j < _wave->getVWave().size(); j++)
		{
			RECT temp;
			RECT buu = _em->getVBuu()[i]->getRect();
			if (buu.right < WINSIZEX && _em->getVBuu()[i]->getState() != 3) // ���κο찡 ȭ�� �ۿ� �ְų� �״� �̹��� ����� �浹 ���ϵ��� ����ó��
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

	//==================================== �����۰� �浹 =======================================

	for (int i = 0; i < _upItem->getVItem().size(); i++)  // �Ѿ� ���� ������
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

	for (int i = 0; i < _hpItem->getVItem().size(); i++) // HP ������
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

	for (int i = 0; i < _skillItem->getVItem().size(); i++) // ��ų ������
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
