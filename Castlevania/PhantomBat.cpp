#include "PhantomBat.h"
#include "Utils.h"
#include <math.h>

void PhantomBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PHANTOMBAT_BBOX_WIDTH;
	bottom = y + PHANTOMBAT_BBOX_HEIGHT;
}

void PhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	srand(time(NULL));

	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	// Đổi trạng thái
	if (simon->x > cam_x + 3 * SCREEN_WIDTH / 5 && state == PHANTOMBAT_STATE_WAIT)
	{
		isWait = true;
		isAttack = true;

		destination_x = simon->x;
		destination_y = simon->y;

		waitTime = GetTickCount();
		SetState(PHANTOMBAT_STATE_ATTACK);
	}

	// Đổi từ đang chờ sang bay
	if (isWait)
	{
		// Nếu sắp tấn công
		if (isAttack && GetTickCount() - waitTime > PHANTOMBAT_WAITING_TIME)
		{
			vy = (destination_y - y) / PHANTOMBAT_TIME_DOWN;

			// Tính vận tốc bay ngang theo góc hợp bởi vị trí hiện tại của boss dơi và destination
			float angle = atan2(destination_x - x, destination_y - y);

			DebugOut(L"[GOC] angle = %f\n", angle);

			// Tính vx theo vy
			vx = vy * tan(angle);

			isTop = false;
			isWait = false;
		}

		// Nếu sắp quay về vị trí khác
		if (!isAttack && GetTickCount() - attackTime > PHANTOMBAT_ATTACK_TIME)
		{
			vy = (destination_y - y) / PHANTOMBAT_TIME_UP;

			// Tính vận tốc bay ngang theo góc hợp bởi vị trí hiện tại của boss dơi và destination
			float angle = atan2(destination_x - x, destination_y - y);
			DebugOut(L"[GOC] angle = %f\n", angle);

			// Tính vx theo vy
			vx = vy * tan(angle);

			isBottom = false;
			isWait = false;
		}
	}
	else
	{
		if (isAttack && !isBottom)
		{
			// Đã bay đến vị trí bottom
			if (x - 10 <= destination_x && x + PHANTOMBAT_BBOX_WIDTH  >= destination_x &&
				y - 10 <= destination_y && y + PHANTOMBAT_BBOX_HEIGHT >= destination_y)
			{
				vx = 0;
				vy = 0;

				// Đặt thời gian dừng
				attackTime = GetTickCount();

				// Đổi vị trí destination x y
				destination_x = rand() % (SCREEN_WIDTH - 3 * PHANTOMBAT_BBOX_WIDTH) + cam_x;
				destination_y = rand() % (SCREEN_HEIGHT / 4) + SCREEN_HEIGHT / 4 + cam_y;

				if (destination_x < cam_x)
					destination_x = cam_x;
				if (destination_x > cam_x + SCREEN_WIDTH - 3 * PHANTOMBAT_BBOX_WIDTH)
					destination_x = cam_x + SCREEN_WIDTH - 3 * PHANTOMBAT_BBOX_WIDTH;

				if (destination_y < cam_y)
					destination_y = cam_y;
				if (destination_y > cam_y + SCREEN_HEIGHT / 2)
					destination_y = cam_y + SCREEN_HEIGHT / 2;

				isAttack = false;
				isBottom = true;
				isWait = true;
			}
			// Đang trên đường bay xuống
			else
			{
				if (isHurt)
				{
					if (GetTickCount() - hurtTime > PHANTOMBAT_HURTING_TIME)
					{
						isHurt = false;
						isBleeding = false;
					}
				}
				else
				{
					Enemy::Update(dt, coObjects);

					this->x += dx;
					this->y += dy;
				}
			}
		}

		if (!isAttack && !isTop)
		{
			// Đã bay đến vị trí top
			if (x - 10 <= destination_x && x + PHANTOMBAT_BBOX_WIDTH + 10 >= destination_x &&
				y - 10 <= destination_y && y + PHANTOMBAT_BBOX_HEIGHT + 10 >= destination_y)
			{
				vx = 0;
				vy = 0;

				// Đặt thời gian dừng
				waitTime = GetTickCount();

				// Đổi vị trí destination x y
				destination_x = simon->x;
				destination_y = simon->y;

				isAttack = true;
				isTop = true;
				isWait = true;
			}
			// Đang trên đường bay lên
			else
			{
				if (isHurt)
				{
					if (GetTickCount() - hurtTime > PHANTOMBAT_HURTING_TIME)
					{
						isHurt = false;
						isBleeding = false;
					}
				}
				else
				{
					Enemy::Update(dt, coObjects);

					this->x += dx;
					this->y += dy;
				}
			}
		}
	}
}

void PhantomBat::Render()
{
	int ani = PHANTOMBAT_ANI_WAIT;

	switch (state)
	{
	case PHANTOMBAT_STATE_WAIT:
		ani = PHANTOMBAT_ANI_WAIT;
		break;
	case PHANTOMBAT_STATE_ATTACK:
		ani = PHANTOMBAT_ANI_ATTACK;
		break;
	}

	int alpha = 255;
	if (isUntouchable) {
		// Nhap nhay
		if (transparent)
			alpha = 125;
		transparent = !transparent;
	}

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void PhantomBat::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PHANTOMBAT_STATE_WAIT:
		vx = vy = 0;
		break;
	case PHANTOMBAT_STATE_ATTACK:
		break;
	}
}