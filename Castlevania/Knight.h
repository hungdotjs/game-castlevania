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
public:
	float minX, maxX;
	bool isHitted;
	int hitCount = 0;

	Knight();
	Knight(float minX, float maxX) {
		this->minX = minX;
		this->maxX = maxX;
		SetState(KNIGHT_STATE_WALKING);
	}
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};