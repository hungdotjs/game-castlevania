#pragma once
#include "Weapon.h"

#define HOLYWATER_ANIMATION_ID	3019

#define HOLYWATER_ANI_RIGHT 0
#define HOLYWATER_ANI_LEFT	1
#define HOLYWATER_ANI_BURN	2

#define HOLYWATER_BURN_TIME	500

class HolyWater : public Weapon
{

public:
	bool isBurn;

	HolyWater(Simon* simon, float max_width = SCREEN_WIDTH) { 
		this->simon = simon; 
		this->max_width = max_width;
		SetAnimationSet(HOLYWATER_ANIMATION_ID);
	}

	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

