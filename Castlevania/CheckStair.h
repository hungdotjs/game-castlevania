#pragma once

#include "GameObject.h"

#define CHECKSTAIR_BBOX_WIDTH			16
#define CHECKSTAIR_BBOX_HEIGHT			16

// Check stair
#define CHECKSTAIR_UP_LEFT				0
#define CHECKSTAIR_UP_RIGHT				1
#define CHECKSTAIR_DOWN_LEFT			2
#define CHECKSTAIR_DOWN_RIGHT			3

class CheckStair : public CGameObject
{
	int type;

public:
	CheckStair(int type) {
		this->type = type;
	}

	void SetType(int type) { this->type = type; }
	int GetType() { return type; }

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
