#pragma once

#include "GameObject.h"
#include "Utils.h"

#define EFFECT_TIME	 600
#define EFFECT_ANIMATION_ID	10
#define EFFECT_WIDTH	15
#define EFFECT_HEIGHT	15

class Effect : public CGameObject
{
	bool isExposed;
	DWORD firstRenderTime;
public:
	Effect(DWORD time) { 
		firstRenderTime = time; 
		isExposed = false; 
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(EFFECT_ANIMATION_ID);
		SetAnimationSet(ani_set);
	}
	bool GetExposed() { return isExposed; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

