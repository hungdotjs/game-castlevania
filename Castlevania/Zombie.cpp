#include "Zombie.h"
#include "Utils.h"
#include "Game.h"

void Zombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;
	bottom = y + ZOMBIE_BBOX_HEIGHT;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	float camX, camY;
	CGame::GetInstance()->GetCamPos(camX, camY);

	if (x < camX) {
		x = camX + SCREEN_WIDTH;
	}

	// Trạng thái chết
	if (health <= 0)
	{
		isDie = true;
		return;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;

	// Sẽ luôn luôn chạy thẳng
	x += dx;

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny, rdx = 0, rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		bool willBlock = false;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				if (e->ny < 0)
				{
					willBlock = true;

					// Xét va chạm cứng
					vy = 0;
				}
			}
		}

		//// block 
		//if (willBlock)
		//{
		//	x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//	y += min_ty * dy + ny * 0.4f;
		//}
		//else
		//{
		//	x += dx;
		//	y += dy;
		//}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Zombie::Render()
{
	int ani = 0;

	if (vx < 0)
		ani = ZOMBIE_ANI_WALKING_LEFT;
	else if (vx > 0)
		ani = ZOMBIE_ANI_WALKING_RIGHT;

	if (isFrozen)
		animation_set->at(ani)->ResetFrame();

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Zombie::SetState(int state, int nx)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ZOMBIE_STATE_WALKING:
		if (nx == 0)
			vx = -ZOMBIE_WALKING_SPEED;
		else
			vx = ZOMBIE_WALKING_SPEED;
		break;
	}
}

