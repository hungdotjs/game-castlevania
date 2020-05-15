#pragma once

#include "GameObject.h"

#define BAT_FLY_SPEED			0.03f

#define BAT_BBOX_WIDTH			16
#define BAT_BBOX_HEIGHT			16

#define BAT_STATE_IDLE			100
#define BAT_STATE_FLYING		200

#define BAT_ANI_IDLE			0
#define BAT_ANI_FLYING_RIGHT	1
#define BAT_ANI_FLYING_LEFT	2

class Bat : public CGameObject
{
public:
	bool isAttack;
	Bat();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);


};