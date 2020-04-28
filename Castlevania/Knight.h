#pragma once

#include "GameObject.h"

#define KNIGHT_WALKING_SPEED 0.03f;

#define KNIGHT_BBOX_WIDTH	16
#define KNIGHT_BBOX_HEIGHT	32

#define KNIGHT_STATE_WALKING	100

#define KNIGHT_ANI_WALKING_RIGHT	0
#define KNIGHT_ANI_WALKING_LEFT		1

class Knight : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	Knight();
	virtual void SetState(int state);
};