#pragma once

#include "GameObject.h"
#include "Utils.h"

#define EFFECT_TIME	 600
#define EFFECT_ANIMATION_ID						10
#define EFFECT_MONEY_1000PTS_ANIMATION_ID		516
#define EFFECT_2000PTS_ANIMATION_ID				518
#define EFFECT_700PTS_ANIMATION_ID				520

#define EFFECT_WIDTH	15
#define EFFECT_HEIGHT	15

class Effect : public CGameObject
{
	bool isExposed;
	DWORD firstRenderTime;
public:
	Effect(int ani = EFFECT_ANIMATION_ID) {
		firstRenderTime = GetTickCount(); 
		isExposed = false; 
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(ani);
		SetAnimationSet(ani_set);
	}
	bool GetExposed() { return isExposed; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

