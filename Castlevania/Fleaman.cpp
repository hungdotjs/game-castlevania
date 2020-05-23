#include "Fleaman.h"

Fleaman::Fleaman(float x, float y) : CGameObject()
{
	SetState(FLEAMAN_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void Fleaman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLEAMAN_BBOX_WIDTH;
	bottom = y + FLEAMAN_BBOX_HEIGHT;
}

void Fleaman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;


	if (y >= start_y + 40) {
		y = start_y + 40;
	}
}

void Fleaman::Render()
{
	int ani;

	if (vx > 0)
	{
		ani = FLEAMAN_ANI_RIGHT;
	}
	else if (vx < 0)
	{
		ani = FLEAMAN_ANI_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Fleaman::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLEAMAN_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	}

}