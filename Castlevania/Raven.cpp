#include "Raven.h"
#include "Simon.h"
#include <ctime>
#include <math.h>

void Raven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + RAVEN_BBOX_WIDTH;
	bottom = y + RAVEN_BBOX_HEIGHT;
}

void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (!isRest)
	{

		if (GetTickCount() - lastRestTime > RAVEN_FLYING_TIME)
		{
			isRest = true;
			SetState(RAVEN_STATE_IDLE);
			restCat = GetTickCount();
		}
	}

	if (isRest && GetTickCount() - restCat > RAVEN_REST_TIME)
	{
		isRest = false;
		lastRestTime = GetTickCount();
		SetState(RAVEN_STATE_FLY);

	}
}

void Raven::Render()
{
	int ani = -1;

	if (nx > 0)
	{
		ani = RAVEN_ANI_FLYING_RIGHT;
	}
	else if (nx < 0)
	{
		ani = RAVEN_ANI_FLYING_LEFT;
	}


	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Raven::SetState(int state)
{
	float simonX, simonY;
	Simon::GetInstance()->GetPosition(simonX, simonY);

	Enemy::SetState(state);

	switch (state)
	{
	case RAVEN_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case RAVEN_STATE_FLY:
		if (x < simonX) {
			vx = RAVEN_FLYING_SPEED_X;
			nx = 1;
		}
		else {
			nx = -1;
			vx = -RAVEN_FLYING_SPEED_X;

		}

		if (y < simonY) {
			vy = RAVEN_FLYING_SPEED_Y;
		}
		else {
			vy = -RAVEN_FLYING_SPEED_Y;
		}
		break;
	}
}