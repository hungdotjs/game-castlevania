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
			vx = ENEMY_BULLET_SPEED;
		else
			vx = -ENEMY_BULLET_SPEED;
		vy = -0.8f;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

