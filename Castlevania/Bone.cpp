#include "Bone.h"

void Bone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BONE_BBOX_WIDTH;
	bottom = y + BONE_BBOX_HEIGHT;
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
	vy += BONE_GRAVITY * dt;
	x += dx;
	y += dy;
}

void Bone::Render()
{

	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}

