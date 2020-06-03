#pragma once
#include "GameObject.h"
#include "Sprites.h"
#include "Utils.h"
#include "Enemy.h"

#define WHIP_LEVEL_1		1
#define WHIP_LEVEL_2		2
#define WHIP_LEVEL_3		3

#define WHIP_SITTING		8

#define WHIP_RIGHT 						0
#define WHIP_LEFT						1
#define WHIP_RIGHT_1					2
#define WHIP_LEFT_1						3
#define WHIP_RIGHT_2					4
#define WHIP_LEFT_2						5

#define WHIP_STATE_HIT		1

#define WHIP_LV0_BBOX_WIDTH				46
#define WHIP_LV0_BBOX_HEIGHT			12
#define WHIP_LV1_BBOX_WIDTH				46
#define WHIP_LV1_BBOX_HEIGHT			12
#define WHIP_LV2_BBOX_WIDTH				62
#define WHIP_LV2_BBOX_HEIGHT			12

class Whip :public CGameObject
{
	CAnimation* ani;

	int indexAni;
	int damage;
	int level;

public:
	Whip();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render(bool isRight);
	void Render();

	void SetPosition(float x, float y, bool isSitting);

	void SetLevel(int level) { this->level = level; }
	int GetLevel() { return this->level; }

	int GetCurrentFrame();

	void ResetAnimation();

	~Whip();
};

