#include "HiddenObject.h"


HiddenObject::HiddenObject(float item_x, float item_y, int itemID)
{
	this->itemID = itemID;
	this->item_x = item_x;
	this->item_y = item_y;
}

void HiddenObject::Render()
{
	RenderBoundingBox();
}

void HiddenObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + HIDDEN_BBOX_WIDTH;
	b = y + HIDDEN_BBOX_HEIGHT;
}