#include "Whip.h"
#include "Game.h"
#include "Utils.h"


#include "Torch.h"
#include "Candle.h"
#include "Item.h"

Whip::Whip()
{
	damage = 1;
	level = WHIP_LEVEL_1;
	indexAni = 0;
}

Whip::~Whip()
{
}

int Whip::GetCurrentFrame()
{
	ani = animation_set->at(indexAni);
	return ani->GetCurrentFrame();
}

void Whip::SetPosition(float x, float y, bool isSitting)
{
	if (isSitting)
	{
		y += WHIP_SITTING;
	}
	CGameObject::SetPosition(x, y);
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (indexAni)
	{
	case WHIP_LEFT:
	case WHIP_RIGHT:
		right = left + WHIP_LV0_BBOX_WIDTH;
		bottom = top + WHIP_LV0_BBOX_HEIGHT;
		break;

	case WHIP_LEFT_1:
	case WHIP_RIGHT_1:
		right = left + WHIP_LV1_BBOX_WIDTH;
		bottom = top + WHIP_LV1_BBOX_HEIGHT;
		break;

	case WHIP_LEFT_2:
	case WHIP_RIGHT_2:
		right = left + WHIP_LV2_BBOX_WIDTH;
		bottom = top + WHIP_LV2_BBOX_HEIGHT;
		break;
	default:
		break;
	}
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == WHIP_STATE_HIT && isEnable)
	{
		CGameObject::Update(dt);

		for (UINT i = 0; i < coObjects->size(); i++)
		{
			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			coObjects->at(i)->GetBoundingBox(l2, t2, r2, b2);

			if (CGame::AABBCheck(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				if (dynamic_cast<Enemy*>(coObjects->at(i))) {

					Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));

					if ((coObjects->at(i))->nx != 0)
					{
						if (enemy->isEnable != false) {
							enemy->GetCollisionEffect()->SetEnable(true);
							enemy->SetHP(enemy->GetHP() - this->damage);
							this->isEnable = false;
							ResetAnimation();
						}
					}
				}
				else if (dynamic_cast<Torch*>(coObjects->at(i))) {
					Torch* torch = dynamic_cast<Torch*>(coObjects->at(i));
					if (torch->isEnable) {
						torch->GetCollisionEffect()->SetEnable(true);
						torch->GetDeadEffect()->SetEnable(true);
						torch->isEnable = false;
						torch->isDeadth = true;
					}
				}
				else if (dynamic_cast<Candle*>(coObjects->at(i))) {
					Candle* candle = dynamic_cast<Candle*>(coObjects->at(i));
					if (candle->isEnable) {
						candle->GetCollisionEffect()->SetEnable(true);
						candle->GetDeadEffect()->SetEnable(true);
						candle->isEnable = false;
						candle->isDeadth = true;
					}
				}
			}
		}
	}
}

void Whip::Render(bool isRight)
{
	if (isEnable)
	{
		float posX = x, posY = y;

		switch (level)
		{
		case WHIP_LEVEL_1:
			if (isRight)
			{
				indexAni = WHIP_RIGHT;
			}
			else
			{
				indexAni = WHIP_LEFT;
			}
			break;
		case WHIP_LEVEL_2:
			if (isRight)
			{
				indexAni = WHIP_RIGHT_1;
			}
			else
			{
				indexAni = WHIP_LEFT_1;
			}
			break;
		case WHIP_LEVEL_3:
			if (isRight)
			{
				indexAni = WHIP_RIGHT_2;

			}
			else
			{
				indexAni = WHIP_LEFT_2;
			}
			break;
		}


		if (isRight)
		{
			posX = x - 16;
		}
		else {
			if (level == WHIP_LEVEL_3)
			{
				posX = x - 44;
			}
			else
			{
				posX = x - 28;
			}

		}
		posY = y + 15;

		animation_set->at(indexAni)->Render(posX, posY);
		if (state == WHIP_STATE_HIT)
		{
			//RenderBoundingBox();
		}
	}

}

void Whip::Render()
{

}

void Whip::ResetAnimation()
{
	for (int i = 0; i < 12; i++)
	{
		ResetAni(i);
	}
}


