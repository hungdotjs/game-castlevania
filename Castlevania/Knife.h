#pragma once
#include "Weapon.h"

// Knife
#define KNIFE_BBOX_WIDTH				16
#define KNIFE_BBOX_HEIGHT				9

#define KNIFE_SPEED						0.3f
#define KNIFE_ANIMATION_ID				3011			

class Knife : public Weapon
{

public:

	Knife(Simon* simon, float max_width = SCREEN_WIDTH) 
	{ 
		this->simon = simon; 
		this->max_width = max_width;
		SetAnimationSet(KNIFE_ANIMATION_ID);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};