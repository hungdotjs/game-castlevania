#include "Cross.h"
#include "Effect.h"
#include "Portal.h"

void Cross::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	float ml, mr, mt, mb;

	this->GetBoundingBox(ml, mt, mr, mb);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (!dynamic_cast<CheckStair*>(coObjects->at(i)) &&
			!dynamic_cast<CheckStairTop*>(coObjects->at(i)) &&
			!dynamic_cast<Effect*>(coObjects->at(i)) &&
			!dynamic_cast<CPortal*>(coObjects->at(i)) &&
			!dynamic_cast<Item*>(coObjects->at(i)) &&
			!dynamic_cast<CBrick*>(coObjects->at(i)) &&
			!dynamic_cast<Weapon*>(coObjects->at(i)))
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			float sl, sr, st, sb;

			coObjects->at(i)->GetBoundingBox(sl, st, sr, sb);

			if (ml < sr && mr > sl && mt < sb && mb > st)
			{
				if (dynamic_cast<Enemy*>(e->obj))
				{
					Enemy* enemy = dynamic_cast<Enemy*>(e->obj);


					Simon::score += 100;
					enemy->isHitted = true;
					enemy->health -= 1;


				}
				else if (dynamic_cast<Simon*>(e->obj) && isReturn)
				{
					isExposed = true;
				}
			}

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Cross::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Weapon::Update(dt);

	if (isActivate)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

		float camX, camY;
		CGame::GetInstance()->GetCamPos(camX, camY);

		// Outrange
		if (nx > 0)
		{
			if (x < camX) {
				isExposed = true;
				return;
			}
			if (x > max_x)
			{
				if (!isReturn)
				{
					x = max_x;
					vx = -vx;
					dx = 0;
					isReturn = true;
				}
				else
					isExposed = true;
			}
		}
		else if (nx < 0)
		{

			if (x > camX + SCREEN_WIDTH ) {
				isExposed = true;
				return;
			}
			if (x < 0) {
				x = 0;
				vx = -vx;
				dx = 0;
				isReturn = true;

			}
			else if (x < max_x )
			{
				if (!isReturn)
				{
					x = max_x;
					vx = -vx;
					dx = 0;
					isReturn = true;
				}
				else
					isExposed = true;
			}
		}

		vector<LPGAMEOBJECT>* simonObject = new vector<LPGAMEOBJECT>;
		simonObject->push_back(simon);

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		CalcPotentialCollisions(coObjects, coEvents);
		CalcPotentialCollisions(simonObject, coEvents);

		delete simonObject;

		// No collision occured, proceed normally
		x += dx;
		y += dy;

		float min_tx, min_ty, nx = 0, ny, rdx = 0, rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Torch*>(e->obj))
			{
				Torch* torch = dynamic_cast<Torch*>(e->obj);
				torch->isHitted = true;
			}
			else if (dynamic_cast<Candle*>(e->obj))
			{
				Candle* candle = dynamic_cast<Candle*>(e->obj);
				candle->isHitted = true;
			}
			else if (dynamic_cast<Simon*>(e->obj))
			{
				isExposed = true;
			}
		}
	}
}

void Cross::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CROSS_BBOX_WIDTH;
	b = y + CROSS_BBOX_HEIGHT;
}
