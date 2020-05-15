#include "Bat.h"

Bat::Bat()
{
	SetState(BAT_STATE_IDLE);
}

void Bat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;
	bottom = y + BAT_BBOX_HEIGHT;
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

}

void Bat::Render()
{
	int ani = BAT_ANI_IDLE;

	if (vx > 0)
		ani = BAT_ANI_FLYING_RIGHT;
	else if (vx < 0)
		ani = BAT_ANI_FLYING_LEFT;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Bat::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BAT_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case BAT_STATE_FLYING:
		vx = BAT_FLY_SPEED;
	}

}