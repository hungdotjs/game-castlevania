#include "Ghost.h"
#include "Utils.h"
#include "Simon.h"

Ghost::Ghost(float startX, float startY)
{
	this->startX = startX;
	this->startY = startY;

	this->health = GHOST_HP;
	isEnable = true;
	damage = GHOST_DAMAGE;
	SetState(GHOST_STATE_HIDE);
}

Ghost::~Ghost()
{
}

void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	Enemy::Update(dt, coObject);
	if (isEnable)
	{
		float simonX, simonY;

		Simon::GetInstance()->GetPosition(simonX, simonY);

		nx = this->x >= simonX ? -1 : 1;

		if (state == GHOST_STATE_FLYING)
		{
			x += dx;
			y += dy;

			if (nx > 0)
			{
				vx = GHOST_FLYING_SPEED_X;
			}
			else
			{
				vx = -GHOST_FLYING_SPEED_X;
			}
		}
		else if (state == GHOST_STATE_HIDE)
		{
			if (abs(this->x - simonX) >= GHOST_DISTANCE_ATTACK_X)
			{
				SetState(GHOST_STATE_FLYING);
			}
		}
	}
}

void Ghost::Render()
{
	if (state != GHOST_STATE_HIDE)
	{
		int ani = 0;
		switch (state)
		{
		case GHOST_STATE_WAITTING:
		case GHOST_STATE_FLYING:
		{
			if (nx > 0)
			{
				ani = GHOST_ANI_FLYING_RIGHT;
			}
			else
			{
				ani = GHOST_ANI_FLYING_LEFT;
			}
		}
		break;
		default:
			break;
		}

		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();

	}

}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GHOST_STATE_HIDE)
	{
		left = 0;
		top = 0;
		right = left + 0;
		bottom = top + 0;
	}
	else
	{
		left = x;
		right = left + GHOST_BBOX_WIDTH;
		top = y;
		bottom = top + GHOST_BBOX_HEIGHT;
	}
}

void Ghost::SetState(int state)
{
	Enemy::SetState(state);
	switch (state)
	{
	case GHOST_STATE_FLYING:
		if (nx > 0)
		{
			vx = GHOST_FLYING_SPEED_X;
		}
		else
		{
			vx = -GHOST_FLYING_SPEED_X;
		}
		break;
	case GHOST_STATE_WAITTING:
		vx = 0;
		break;
	}
}