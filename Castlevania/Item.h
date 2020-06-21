#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Candle.h"
#include "Torch.h"
#include "CheckStair.h"
#include "Enemy.h"


class Item : public CGameObject
{
	int type;
	bool isEaten;
public:
	Item();
	DWORD appearTime;
	void SetType(int _type) { type = _type; }
	int GetType() { return type; }
	void SetEaten() { isEaten = true; }
	bool GetEaten() { return isEaten; }
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

