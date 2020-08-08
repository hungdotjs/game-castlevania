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
		vy += WEAPON_GRAVITY * dt;

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
					sound->Play(SOUND_HIT);

					if (dynamic_cast<PhantomBat*>(e->obj))
					{
						PhantomBat* phantomBat = dynamic_cast<PhantomBat*>(e->obj);

						if (!phantomBat->isUntouchable)
						{
							CGame::GetInstance()->bossHeath -= WEAPON_DAME;
							phantomBat->isHurt = true;
							phantomBat->isHitted = true;
							phantomBat->hurtTime = GetTickCount();
							phantomBat->StartUntouchable();
						}

						if (CGame::GetInstance()->bossHeath <= 0)
						{
							phantomBat->isDie = true;
							Simon::score += 1000;
							CGame::GetInstance()->startFightBoss = false;
						}
					}
					else {
						enemy->isHitted = true;
						enemy->health -= 1;

					}

					this->isExposed = true;

				}
				else if (dynamic_cast<Torch*>(e->obj))
				{
					Torch* torch = dynamic_cast<Torch*>(e->obj);
					torch->isHitted = true;
					this->isExposed = true;
					sound->Play(SOUND_HIT);

				}
				else if (dynamic_cast<Candle*>(e->obj))
				{
					Candle* candle = dynamic_cast<Candle*>(e->obj);
					candle->isHitted = true;
					this->isExposed = true;
					sound->Play(SOUND_HIT);
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