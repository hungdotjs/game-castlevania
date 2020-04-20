#pragma once
#include "GameObject.h"

#define TORCH_BBOX_WIDTH				16
#define TORCH_BBOX_HEIGHT				32

class Torch : public CGameObject
{

public:
	bool isHitted;
	Torch() :CGameObject() {
		isHitted = false;
	}
	void SetHit();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
