﻿#include "Item.h"
#include "Weapon.h"
#include "CheckStairTop.h"

Item::Item()
{
	this->appearTime = GetTickCount();
}

void Item::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (!dynamic_cast<Candle*>(coObjects->at(i)) &&
			!dynamic_cast<Torch*>(coObjects->at(i)) &&
			!dynamic_cast<CheckStair*>(coObjects->at(i)) &&
			!dynamic_cast<CheckStairTop*>(coObjects->at(i)) &&
			!dynamic_cast<Enemy*>(coObjects->at(i)) &&
			!dynamic_cast<Item*>(coObjects->at(i)) &&
			!dynamic_cast<Weapon*>(coObjects->at(i))
			)
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += ITEM_GRAVITY * dt;

	// No collision occured, proceed normally
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

		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}
}

void Item::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;

	switch (type)
	{
	case ITEM_MINIHEART:
		r = x + MINIHEART_BBOX_WIDTH;
		b = y + MINIHEART_BBOX_HEIGHT;
		break;
	case ITEM_HEART:
		r = x + HEART_BBOX_WIDTH;
		b = y + HEART_BBOX_HEIGHT;
		break;
	case ITEM_WHIP:
		r = x + WHIPITEM_BBOX_WIDTH;
		b = y + WHIPITEM_BBOX_HEIGHT;
		break;
	case ITEM_MONEY:
		r = x + MONEY_BBOX_WIDTH;
		b = y + MONEY_BBOX_HEIGHT;
		break;
	case ITEM_KNIFE:
		r = x + KNIFE_BBOX_WIDTH;
		b = y + KNIFE_BBOX_HEIGHT;
		break;
	case ITEM_AXE:
		r = x + AXE_BBOX_WIDTH;
		b = y + AXE_BBOX_HEIGHT;
		break;
	case ITEM_CROSS:
		r = x + CROSS_BBOX_WIDTH;
		b = y + CROSS_BBOX_HEIGHT;
		break;
	case ITEM_HOLYWATER:
		r = x + HOLYWATER_BBOX_WIDTH;
		b = y + HOLYWATER_BBOX_HEIGHT;
		break;
	case ITEM_CLOCK:
		r = x + CLOCK_BBOX_WIDTH;
		b = y + CLOCK_BBOX_HEIGHT;
		break;
	}
}

