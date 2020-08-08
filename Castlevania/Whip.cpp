#include "Simon.h"
#include "Whip.h"
#include "Knight.h"
#include "Bat.h"
#include "EffectWhip.h"
#include "PhantomBat.h"
#include "HiddenBrick.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float wl, wr, wt, wb;
	GetBoundingBox(wl, wt, wr, wb);
	//DebugOut(L"[WHIP] nx = %d\n", nx);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Enemy*>(coObjects->at(i))) {
			if (dynamic_cast<PhantomBat*>(coObjects->at(i)))
			{
				PhantomBat* phantomBat = dynamic_cast<PhantomBat*>(coObjects->at(i));

				float zl, zr, zt, zb;
				phantomBat->GetBoundingBox(zl, zt, zr, zb);

				if (wl < zr && wr > zl && wt < zb && wb > zt)
				{
					sound->Play(SOUND_HIT);
					if (!phantomBat->isUntouchable)
					{
						CGame::GetInstance()->bossHeath -= WEAPON_DAME;
						phantomBat->isHurt = true;
						phantomBat->isHitted = true;
						phantomBat->hurtTime = GetTickCount();
						phantomBat->StartUntouchable();
					}

					if (CGame::GetInstance()->bossHeath <= 0)
					{
						phantomBat->isDie = true;
						Simon::score += 1000;
						CGame::GetInstance()->startFightBoss = false;
						//CGame::GetInstance()->bossHeath = 16;
					}
				}
			}
			else {
				/*if (dynamic_cast<Fleaman*>(coObjects->at(i)))
				{*/
				Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));

				float zl, zt, zr, zb;
				enemy->GetBoundingBox(zl, zt, zr, zb);

				if (wl < zr && wr > zl && wt < zb && wb > zt)
				{
					sound->Play(SOUND_HIT);

					enemy->isHitted = true;
					enemy->health -= 1;
				}
				//}
			}

		}
		else if (dynamic_cast<Torch*>(coObjects->at(i)))
		{
			Torch* torch = dynamic_cast<Torch*>(coObjects->at(i));
			float zl, zr, zt, zb;
			torch->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zr && wr > zl && wt < zb && wb > zt)
			{
				sound->Play(SOUND_HIT);

				torch->isHitted = true;
			}
		}
		else if (dynamic_cast<Candle*>(coObjects->at(i)))
		{
			Candle* candle = dynamic_cast<Candle*>(coObjects->at(i));
			float zl, zr, zt, zb;
			candle->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zr && wr > zl && wt < zb && wb > zt)
			{
				sound->Play(SOUND_HIT);

				candle->isHitted = true;
			}
		}
		else if (dynamic_cast<HiddenBrick*>(coObjects->at(i)))
		{
			HiddenBrick* brick = dynamic_cast<HiddenBrick*>(coObjects->at(i));
			float zl, zr, zt, zb;
			brick->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zr && wr > zl && wt < zb && wb > zt)
			{
				brick->isHitted = true;
			}
		}
		else if (dynamic_cast<Knight*>(coObjects->at(i)))
		{
			Knight* knight = dynamic_cast<Knight*>(coObjects->at(i));
			float zl, zr, zt, zb;
			knight->GetBoundingBox(zl, zt, zr, zb);
			if (wl < zr && wr > zl && wt < zb && wb > zt)
			{
				sound->Play(SOUND_HIT);

				knight->isHitted = true;
				knight->health -= 1;
				DebugOut(L"Hitted: health = %d \n", knight->health);
			}
		}
		else if (dynamic_cast<Bat*>(coObjects->at(i)))
		{
			Bat* bat = dynamic_cast<Bat*>(coObjects->at(i));
			float zl, zr, zt, zb;
			bat->GetBoundingBox(zl, zt, zr, zb);

			if (wl < zr && wr > zl && wt < zb && wb > zt)
			{
				sound->Play(SOUND_HIT);

				bat->isHitted = true;
			}
		}

	}

	x = 0;
	y = 0;

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
