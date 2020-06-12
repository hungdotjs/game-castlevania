#include "Skeleton.h"
#include "Simon.h"

void Skeleton::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case SKELETON_STATE_WALKING:
		if (nx < 0) {
			vx = -SKELETON_WALKING_SPEED;
		}
		else {
			vx = SKELETON_WALKING_SPEED;
		}
		vy = -SKELETON_JUMPING_SPEED;
		break;
	}
}

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SKELETON_BBOX_WIDTH;
	bottom = y + SKELETON_BBOX_HEIGHT;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{

				if (ny != 0) vy = 0;

			}
		}
	}

	
}

void Skeleton::Render()
{
	int ani = 0;
	if (nx < 0) {
		ani = SKELETON_ANI_WALKING_LEFT;
	}
	else {
		ani = SKELETON_ANI_WALKING_RIGHT;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

