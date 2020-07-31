#pragma once

#include "GameObject.h"
#include "Utils.h"

#define EFFECT_BROKEN_TIME				1000
#define EFFECT_BROKEN_ANIMATION_ID		14
#define EFFECT_BROKEN_WIDTH				7
#define EFFECT_BROKEN_HEIGHT			8

class BrokenEffect : public CGameObject
{
	bool isExposed;
	DWORD firstRenderTime;
public:
	BrokenEffect() {
		firstRenderTime = GetTickCount();
		isExposed = false;
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(EFFECT_BROKEN_ANIMATION_ID);
		SetAnimationSet(ani_set);
	}
	bool GetExposed() { return isExposed; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

