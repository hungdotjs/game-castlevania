#include "EffectWhip.h"

void EffectWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount() - firstRenderTime > EFFECT_WHIP_TIME)
		isExposed = true;
}

void EffectWhip::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
	//DebugOut(L"[EFFECT] render %f %f\n", x, y);
}

void EffectWhip::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}

