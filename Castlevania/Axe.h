#pragma once
#include "Weapon.h"

#define AXE_ANIMATION_ID				3015

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