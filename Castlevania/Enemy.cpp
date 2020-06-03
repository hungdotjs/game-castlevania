#include "Enemy.h"

int Enemy::score = 0;
float Enemy::stopTime = 0;
int Enemy::timestop = 0;
DWORD Enemy::timestop_start = 0;
bool Enemy::isStop = false;


void Enemy::AddScore()
{
	Enemy::score += point;
}

int Enemy::GetScore()
{
	return score;
}


Enemy::Enemy()
{
	collisionEffect = new CollisionEffect();
	deadEffect = new DeadEffect();
	isEnable = true;
	isDeadth = false;
}

Enemy::Enemy(int hp)
{
	this->hp = hp;
	this->respawnTime = 1000;
	hpC = hp;
	e_prevHP = hp;
	isEnable = true;
	isDeadth = false;
}

Enemy::~Enemy()
{
}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isStop)
		CGameObject::Update(dt, coObjects);
	if (!isDeadth)
	{
		if (isEnable)
		{
			respawnTime = 0;
			if (hp < e_prevHP) {
				e_prevHP = hp;
			}

			if (hp <= 0)
			{
				isDeadth = true;
				isEnable = false;
				GetDeadEffect()->SetEnable(true);
				AddScore();
				return;
			}
			if (isStop == true) {

				if (GetTickCount() - timestop_start > 5000)
				{
					isStop = false;
					timestop = 0;
					timestop_start = 0;
				}
			}
			collisionEffect->SetPosition(x, y + 10);
			deadEffect->SetPosition(x + 3, y - 3);
		}

	}
	if (!isEnable)
		respawnTime += dt;

	collisionEffect->Update(dt);
	deadEffect->Update(dt);
}

void Enemy::Render()
{
	collisionEffect->Render();
	deadEffect->Render();
}

void Enemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}


void Enemy::Reload()
{
	isEnable = false;
	isDeadth = true;
	hp = hpC;
	e_prevHP = hp;
}

void Enemy::Respawn()
{
	isEnable = true;
	isDeadth = false;
	hp = hpC;
	e_prevHP = hp;
}

void Enemy::SetScore(int score)
{
	Enemy::score += score;
}
