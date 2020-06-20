#pragma once
#include "Enemy.h"
#include "Brick.h"

#define RAVEN_ANI_FLYING_LEFT			0
#define RAVEN_ANI_FLYING_RIGHT			1

#define RAVEN_BBOX_WIDTH				16	
#define RAVEN_BBOX_HEIGHT				16

#define RAVEN_REST_TIME					1000
#define RAVEN_FLYING_TIME				500
#define RAVEN_FLYING_SPEED				0.07f

#define RAVEN_STATE_IDLE	0
#define RAVEN_STATE_FLY		1


class Raven : public Enemy
{
public:

	bool isRest;
	DWORD restCat;
	DWORD lastRestTime;

	Raven(float nx = 1)
	{
		this->nx = nx;
		SetState(RAVEN_STATE_IDLE);
	}

	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};





