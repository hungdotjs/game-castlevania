#pragma once
#include "GameObject.h"
#include "CollisionEffect.h"
#include "DeadEffect.h"

#define E_INVINCIBLE_TIME 0.4

class Enemy :public CGameObject
{
protected:
	CollisionEffect* collisionEffect;
	DeadEffect* deadEffect;

	int hp;
	int e_prevHP;
	int hpC;
	int damage;
	int point;
	bool isGrounded;
	static int score;
	void AddScore();

public:
	Enemy();
	Enemy(int hp);
	~Enemy();

	static int timestop;
	static DWORD timestop_start;
	static float stopTime;
	static bool isStop;
	DWORD respawnTime;

	static void StartTimeStop() { timestop = 1; timestop_start = GetTickCount(); }

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	CollisionEffect* GetCollisionEffect() { return collisionEffect; }
	DeadEffect* GetDeadEffect() { return deadEffect; }

	static void SetScore(int score);
	static int GetScore();

	int GetHP() { return this->hp; }
	void SetHP(int hp) { this->hp = hp; }

	int GetDamage() { return this->damage; }
	void SetDamage(int damage) { this->damage = damage; }

	int GetPoint() { return point; }

	void Reload();

	void Respawn();


};
