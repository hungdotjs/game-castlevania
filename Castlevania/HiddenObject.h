#pragma once

#include "GameObject.h"

#define HIDDEN_BBOX_WIDTH	10
#define HIDDEN_BBOX_HEIGHT 10

/*
	Object that triggers scene switching
*/
class HiddenObject : public CGameObject
{
public:
	bool isTouched;
	float item_x;
	float item_y;
	int itemID;


	HiddenObject(float x, float y, int id);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};