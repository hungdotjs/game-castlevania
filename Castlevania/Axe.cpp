#include "Axe.h"

void Axe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Weapon::Update(dt);

	if (isActivate)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Outrange
		if (nx > 0)
		{
			if (this->x > this->max_x)
				isExposed = true;
		}
		else if (nx < 0)
		{
			if (this->x < this->max_x)
				isExposed = true;
		}

		// Outtime
		if (GetTickCount() - firstCast > ITEM_LIVE_TIME / 2)
		{
			isExposed = true;
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		CalcPotentialCollisions(coObjects, coEvents);

		// Gravity
		vy += SIMON_GRAVITY * dt;

		x += dx;
		y += dy;

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{

		}
		else
		{
			float min_tx, min_ty, nx = 0, ny, rdx = 0, rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Enemy*>(e->obj))
				{
					Enemy* enemy = dynamic_cast<Enemy*>(e->obj);

					Simon::score += 100;
					enemy->isHitted = true;
					enemy->health -= 1;

				}
				else if (dynamic_cast<Torch*>(e->obj))
				{
					Torch* torch = dynamic_cast<Torch*>(e->obj);
					torch->isHitted = true;
				}
				else if (dynamic_cast<Candle*>(e->obj))
				{
					Candle* candle = dynamic_cast<Candle*>(e->obj);
					candle->isHitted = true;
				}
			}
		}
	}
}

void Axe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + AXE_BBOX_WIDTH;
	b = y + AXE_BBOX_HEIGHT;
}