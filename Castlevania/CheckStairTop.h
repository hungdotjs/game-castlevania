#pragma once

#include "GameObject.h"

#define CHECKSTAIR_BBOX_WIDTH			16
#define CHECKSTAIR_BBOX_HEIGHT			16

// Check stair
#define CHECKSTAIR_DOWN_LEFT			0
#define CHECKSTAIR_DOWN_RIGHT			1

class CheckStairTop : public CGameObject
{
	int type;

public:
	CheckStairTop(int type) {
		this->type = type;
	}

	void SetType(int type) { this->type = type; }
	int GetType() { return type; }

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
