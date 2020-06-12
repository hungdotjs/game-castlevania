#pragma once

#include "GameObject.h"
#include "Utils.h"

#define EFFECT_WHIP_TIME				200
#define EFFECT_WHIP_ANIMATION_ID		9
#define EFFECT_WHIP_WIDTH				8
#define EFFECT_WHIP_HEIGHT				10

class EffectWhip : public CGameObject
{
	bool isExposed;
	DWORD firstRenderTime;
public:
	EffectWhip() {
		firstRenderTime = GetTickCount();
		isExposed = false;
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(EFFECT_WHIP_ANIMATION_ID);
		SetAnimationSet(ani_set);
	}
	bool GetExposed() { return isExposed; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

