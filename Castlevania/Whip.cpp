#include"Simon.h"
#include "Whip.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float wl, wr, wt, wb;
	GetBoundingBox(wl, wt, wr, wb);

	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	if (dynamic_cast<Zombie*>(coObjects->at(i)))
	//	{
	//		Zombie* zombie = dynamic_cast<Zombie*>(coObjects->at(i));

	//		float zl, zr, zt, zb;
	//		zombie->GetBoundingBox(zl, zt, zr, zb);
	//		if (wl < zl && wr > zr && wt > zt && wb < zb)
	//		{
	//			if (zombie->GetState() != ZOMBIE_STATE_DIE) {
	//				zombie->SetState(ZOMBIE_STATE_DIE);
	//			}
	//			Simon::score += 100;
	//		}
	//	}
	//	else if (dynamic_cast<BigFire*>(coObjects->at(i)))
	//	{
	//		BigFire* bigfire = dynamic_cast<BigFire*>(coObjects->at(i));
	//		float zl, zr, zt, zb;
	//		bigfire->GetBoundingBox(zl, zt, zr, zb);
	//		if (wl < zl && wr > zr && wt > zt && wb < zb)
	//		{
	//			bigfire->isHitted = true;
	//		}
	//	}
	//}
}

void Whip::Render()
{
	RenderBoundingBox();
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (level)
	{
	case 0:
		right = x + WHIP_LV0_BBOX_WIDTH;
		bottom = y + WHIP_LV0_BBOX_HEIGHT;
		break;
	case 1:
		right = x + WHIP_LV1_BBOX_WIDTH;
		bottom = y + WHIP_LV1_BBOX_HEIGHT;
		break;
	default:
		right = x + WHIP_LV2_BBOX_WIDTH;
		bottom = y + WHIP_LV2_BBOX_HEIGHT;
		break;
	}
}

Whip::~Whip()
{

}
