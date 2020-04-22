#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Utils.h"
#include "Brick.h"
#include "Goomba.h"
#include "Portal.h"
#include "Whip.h"
#include "Torch.h"
#include "Item.h"

#define SIMON_WALKING_SPEED			0.1f 
#define SIMON_JUMP_SPEED_Y			0.42f
#define SIMON_JUMP_DEFLECT_SPEED	0.2f
#define SIMON_GRAVITY				0.0015f
#define SIMON_DIE_DEFLECT_SPEED		0.5f
#define SIMON_SIT_TO_STAND			15.0f

#define SIMON_STATE_IDLE			10
#define SIMON_STATE_WALK			20
#define SIMON_STATE_JUMP			30
#define SIMON_STATE_SIT				40
#define SIMON_STATE_DIE				90


// action
#define SIMON_ACTION_JUMP			3
#define SIMON_ACTION_ATTACK			6

// animation
#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1
#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3
#define SIMON_ANI_JUMP_RIGHT		4
#define SIMON_ANI_JUMP_LEFT			5
#define SIMON_ANI_SIT_RIGHT			6
#define SIMON_ANI_SIT_LEFT			7
#define SIMON_ANI_DIE_RIGHT			8
#define SIMON_ANI_DIE_LEFT			9
#define SIMON_ANI_SIT_ATTACK_RIGHT	10
#define SIMON_ANI_SIT_ATTACK_LEFT	11
#define SIMON_ANI_ATTACK_RIGHT		12
#define SIMON_ANI_ATTACK_LEFT		13

#define SIMON_LEVEL			1

#define SIMON_BBOX_WIDTH  16
#define SIMON_BBOX_HEIGHT 30
#define SIMON_SIT_BBOX_WIDTH			16
#define SIMON_SIT_BBOX_HEIGHT			23

#define SIMON_UNTOUCHABLE_TIME		5000
#define SIMON_TIMER_ATTACK			450

class Simon : public CGameObject
{
	int level;
	int action;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

public:
	bool isDead;
	bool isAttack;
	bool isSit;
	bool isJump;
	bool isMoving;
	bool isExitSit;

	int ny;

	DWORD attackTime;

	Whip* whip;
	int life;
	int preHP;
	bool isLevelUp;
	bool isBonus;
	static int score;
	static int heartsAmount;
	int currentWeapon;

public:
	Simon(float x = 0.0f, float y = 0.0f);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	void SetAction(int action);
	int GetAction() { return action; }

	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();


	void SetPosition(float x, float y);

	void SetCurrentWeapon(int weapon) { currentWeapon = weapon; }
	int GetCurrentWeapon() { return currentWeapon; }

	void AddHeart(int num) { heartsAmount += num; }
	void AddScore(int num) { score += num; }

	int GetScore() { return score; }
	int GetLife() { return life; }
	int GetHP() { return preHP; }
	int GetHeart() { return heartsAmount; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};