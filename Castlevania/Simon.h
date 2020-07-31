#pragma once
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "GameObject.h"
#include "Game.h"
#include "Utils.h"
#include "Brick.h"
#include "CheckStairTop.h"
#include "CheckStair.h"
#include "Candle.h"
#include "Elevator.h"
#include "Fleaman.h"
#include "Goomba.h"
#include "Portal.h"
#include "Bat.h"
#include "Knight.h"
#include "Whip.h"
#include "Torch.h"
#include "Item.h"
#include "HiddenObject.h"
#include "Skeleton.h"

class Simon : public CGameObject
{
public:
	int level;
	int action;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	bool isDead;
	bool isAttack;
	bool isSit;
	bool isJump;
	bool isMoving;
	bool isExitSit;
	bool isOnCheckStairUp;
	bool isOnCheckStairDown;
	bool isOnStair;
	bool isLeftToRight;
	bool isHurt;
	bool isUntouchable;
	bool isUseWhip;
	int ny;

	DWORD attackTime;
	DWORD resetTime;

	Whip* whip;
	int life;
	int preHP;
	int hp;
	bool isLevelUp;
	bool willHurt;
	bool transparent;
	bool isCollideHorizontal;
	static int score;
	static int heartsAmount;
	int currentWeapon;
	bool isWin;

	static Simon* __instance;
	bool isClockWeaponUsed;
public:
	Simon(float x = 0.0f, float y = 0.0f);

	static Simon* GetInstance() {
		if (__instance == NULL)
			__instance = new Simon();
		return __instance;
	}

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_SETTINGS(string line);


	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { isUntouchable = true; untouchable_start = GetTickCount(); }

	void Reset();
	void Load(LPCWSTR simonFile);
	void SetWhipAnimationSet(LPANIMATION_SET ani_set);

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

	void ResetAnimationAttack();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};