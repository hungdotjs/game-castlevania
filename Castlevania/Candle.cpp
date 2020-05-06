#include "Candle.h"

void Candle::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Candle::SetHit()
{
	isHitted = true;
}

void Candle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CANDLE_BBOX_WIDTH;
	b = y + CANDLE_BBOX_HEIGHT;
}
