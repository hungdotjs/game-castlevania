#pragma once
#include "GameObject.h"

#define CANDLE_BBOX_WIDTH  8
#define CANDLE_BBOX_HEIGHT 20

class Candle : public CGameObject
{
public:
	bool isHitted;
	int itemID;
	Candle();
	Candle(int itemID) :CGameObject() {
		isHitted = false;
		this->itemID = itemID;
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

