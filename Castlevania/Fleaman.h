#pragma once

#include "GameObject.h"
#include "Simon.h"
#include "Brick.h"
#include "Enemy.h"

#define FLEAMAN_HEALTH				1
#define FLEAMAN_FLY_SPEED_Y			0.2f
#define FLEAMAN_FLY_SPEED_X			0.1f

#define FLEAMAN_GRAVITY				0.001f
#define FLEAMAN_ATTACK_TIME			1000
#define FLEAMAN_DISTANCE_ATTACK_X	160.0f

#define FLEAMAN_BBOX_WIDTH			16
#define FLEAMAN_BBOX_HEIGHT			16

#define FLEAMAN_STATE_IDLE			100
#define FLEAMAN_STATE_ATTACK		200

#define FLEAMAN_ANI_LEFT			0
#define FLEAMAN_ANI_RIGHT			1

class Fleaman : public Enemy
{
public:
	bool isHitted;
	float start_x;
	float start_y;
	DWORD lastTimeAttack;

	Fleaman(float x = 0.0f, float y = 0.0f);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};