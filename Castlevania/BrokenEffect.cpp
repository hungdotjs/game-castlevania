#include "BrokenEffect.h"

void BrokenEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (GetTickCount() - firstRenderTime > EFFECT_BROKEN_TIME)
		isExposed = true;

	vy += SIMON_GRAVITY_JUMP / 2 * dt;

	x += dx;
	y += dy;
}

void BrokenEffect::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
	//DebugOut(L"[EFFECT] render %f %f\n", x, y);
}

void BrokenEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}

