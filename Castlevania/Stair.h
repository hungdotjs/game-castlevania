#pragma once

#include "GameObject.h"

#define STAIR_BBOX_WIDTH  16
#define STAIR_BBOX_HEIGHT 16

class Stair : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

