#include "Elevator.h"

Elevator::Elevator()
{
	minX = 0;
	maxX = 290;
}

void Elevator::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ELEVATOR_BBOX_WIDTH;
	bottom = y + ELEVATOR_BBOX_HEIGHT;
}

void Elevator::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < minX) {
		x = minX;
		vx = -vx;
	}

	if (vx > 0 && x + ELEVATOR_BBOX_WIDTH > maxX) {
		x = maxX - ELEVATOR_BBOX_WIDTH;
		vx = -vx;
	}
}

void Elevator::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
