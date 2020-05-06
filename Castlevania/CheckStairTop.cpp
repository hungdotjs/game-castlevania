#include "CheckStairTop.h"

void CheckStairTop::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CheckStairTop::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CHECKSTAIR_BBOX_WIDTH;
	b = y + CHECKSTAIR_BBOX_HEIGHT;
}
