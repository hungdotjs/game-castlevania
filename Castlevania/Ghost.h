#pragma once
#include "Enemy.h"
class Ghost : public Enemy
{
	float startX, startY;

public:
	Ghost(float startX, float startY);
	~Ghost();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
};