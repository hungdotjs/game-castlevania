#include"Simon.h"
#include "Whip.h"
#include "Knight.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float wl, wr, wt, wb;
	GetBoundingBox(wl, wt, wr, wb);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Torch*>(coObjects->at(i)))
		{
			Torch* torch = dynamic_cast<Torch*>(coObjects->at(i));
			float zl, zr, zt, zb;
			torch->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zl && wr > zr && wt > zt && wb < zb)
			{
				torch->isHitted = true;
			}
		}
		else if (dynamic_cast<Candle*>(coObjects->at(i)))
		{
			Candle* candle = dynamic_cast<Candle*>(coObjects->at(i));
			float zl, zr, zt, zb;
			candle->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zl && wr > zr && wt > zt && wb < zb)
			{
				candle->isHitted = true;
			}
		}
		else if (dynamic_cast<Knight*>(coObjects->at(i)))
		{
			Knight* knight = dynamic_cast<Knight*>(coObjects->at(i));
			float zl, zr, zt, zb;
			knight->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zl && wr > zr && wt > zt && wb < zb)
			{
				knight->isHitted = true;
			}
		}
	}
}

void Whip::Render()
{
	RenderBoundingBox();
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (level)
	{
	case 0:
		right = x + WHIP_LV0_BBOX_WIDTH;
		bottom = y + WHIP_LV0_BBOX_HEIGHT;
		break;
	case 1:
		right = x + WHIP_LV1_BBOX_WIDTH;
		bottom = y + WHIP_LV1_BBOX_HEIGHT;
		break;
	default:
		right = x + WHIP_LV2_BBOX_WIDTH;
		bottom = y + WHIP_LV2_BBOX_HEIGHT;
		break;
	}
}

Whip::~Whip()
{

}
