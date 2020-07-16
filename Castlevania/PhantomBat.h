#pragma once
#include "Enemy.h"
#include "Game.h"
#include "Simon.h"

class PhantomBat : public Enemy
{

public:
	Simon* simon;
	CGame* game;

	bool isHurt;
	bool isBleeding;
	bool isWait;
	bool isAttack;
	bool isTop;
	bool isBottom;
	bool transparent;

	DWORD waitTime;
	DWORD attackTime;
	DWORD hurtTime;

	float destination_x, destination_y;

	PhantomBat() { 
		SetState(PHANTOMBAT_STATE_WAIT);
		this->simon = Simon::GetInstance(); 
		this->game = CGame::GetInstance();
		maxHealth = 16;
		health = 16; 
	}

	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};
