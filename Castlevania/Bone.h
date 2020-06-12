#pragma once
#include "Enemy.h"
#include "Brick.h"

#define BONE_ANIMATION_ID	22

class Bone : public Enemy
{
public:

	Bone(int nx)
	{
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(BONE_ANIMATION_ID);
		SetAnimationSet(ani_set);

		if (nx > 0)
			vx = BONE_SPEED_X;
		else
			vx = -BONE_SPEED_X;
		vy = -BONE_SPEED_Y;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};

