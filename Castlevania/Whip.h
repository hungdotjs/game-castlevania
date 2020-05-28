#pragma once
#include "GameObject.h"
#include "Sprites.h"
#include "Utils.h"

#define WHIP_RIGHT 						0
#define WHIP_LEFT						1
#define WHIP_RIGHT_1					2
#define WHIP_LEFT_1						3
#define WHIP_RIGHT_2					4
#define WHIP_LEFT_2						5


#define WHIP_LV0_BBOX_WIDTH				46
#define WHIP_LV0_BBOX_HEIGHT			12
#define WHIP_LV1_BBOX_WIDTH				46
#define WHIP_LV1_BBOX_HEIGHT			12
#define WHIP_LV2_BBOX_WIDTH				62
#define WHIP_LV2_BBOX_HEIGHT			12

class Whip :public CGameObject
{
	int step = 0;

public:
	int level = 0;
	Whip(int level) :CGameObject()
	{
		this->level = level;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void UpLevel() { level += 1; }
	void SetLevel(int level) {
		this->level = level;
	}
	~Whip();
};

