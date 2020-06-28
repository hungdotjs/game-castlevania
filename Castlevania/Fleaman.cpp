#include "Fleaman.h"

Fleaman::Fleaman(float x, float y)
{
	SetState(FLEAMAN_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	health = FLEAMAN_HEALTH;
	lastTimeAttack = 0;
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
	Enemy::Update(dt, coObjects);

	vy += FLEAMAN_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

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

		bool isCollision = false;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx);

		// block every object first!

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Goomba
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				vx = 0;
				vy = 0;

				if (state == FLEAMAN_STATE_ATTACK && GetTickCount() > lastTimeAttack + FLEAMAN_ATTACK_TIME)
					SetState(FLEAMAN_STATE_IDLE);

				isCollision = true;
			}
		}

		if (!isCollision)
		{
			x += dx;
			y += dy;
		}
	}

}


void Fleaman::Render()
{
	int ani = FLEAMAN_ANI_RIGHT;

	if (nx < 0)
	{
		ani = FLEAMAN_ANI_LEFT;
	}

	if (isFrozen)
		animation_set->at(ani)->ResetFrame();

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
	case FLEAMAN_STATE_ATTACK:
		lastTimeAttack = GetTickCount();
		vy = -FLEAMAN_FLY_SPEED_Y;
		vx = FLEAMAN_FLY_SPEED_X;
		if (nx < 0)
			vx = -FLEAMAN_FLY_SPEED_X;
		break;
	}

}