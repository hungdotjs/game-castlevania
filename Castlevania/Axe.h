#pragma once
#include "Weapon.h"

#define AXE_BBOX_WIDTH					15
#define AXE_BBOX_HEIGHT					15

#define AXE_SPEED_X						0.15f
#define AXE_SPEED_Y						0.45f
#define AXE_ANIMATION_ID			3015

class Axe : public Weapon
{
public:

	Axe(Simon* simon, float max_width = SCREEN_WIDTH) {
		this->simon = simon;
		this->max_width = max_width;
		SetAnimationSet(AXE_ANIMATION_ID);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};