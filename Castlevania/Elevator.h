#pragma once

#include "GameObject.h"

#define ELEVATOR_WALKING_SPEED	0.03f;

#define ELEVATOR_BBOX_WIDTH		32
#define ELEVATOR_BBOX_HEIGHT	8

class Elevator : public CGameObject
{
public:
	float minX, maxX;

	Elevator();
	Elevator(float minX, float maxX) {
		this->minX = minX;
		this->maxX = maxX;
		this->vx = ELEVATOR_WALKING_SPEED;
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};