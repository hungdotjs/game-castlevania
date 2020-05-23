#pragma once

#include "GameObject.h"
#include "Simon.h"

#define FLEAMAN_FLY_SPEED			0.05f

#define FLEAMAN_BBOX_WIDTH			16
#define FLEAMAN_BBOX_HEIGHT			16

#define FLEAMAN_STATE_IDLE			100
#define FLEAMAN_STATE_ATTACK		200

#define FLEAMAN_ANI_RIGHT			0
#define FLEAMAN_ANI_LEFT			1

class Fleaman : public CGameObject
{
public:
	bool isHitted;
	float start_x;
	float start_y;

	Fleaman(float x = 0.0f, float y = 0.0f);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

};