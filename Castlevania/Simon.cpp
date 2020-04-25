#include "Simon.h"

int Simon::score = 0;
int Simon::heartsAmount = 5;

Simon::Simon(float x, float y) : CGameObject()
{
	SetState(SIMON_STATE_IDLE);
	level = SIMON_LEVEL;
	untouchable = 0;

	whip = new Whip();

	life = 3;
	preHP = 16;
	currentWeapon = 0;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void Simon::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	bool isCollideWithCheckBox = false;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// Simon se khong va cham voi nhung vat sau:
		if (!dynamic_cast<Torch*>(coObjects->at(i)) &&
			!dynamic_cast<Whip*>(coObjects->at(i)))
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Has completed attack animation
	if (isAttack == true && GetTickCount() - attackTime >= SIMON_TIMER_ATTACK)
	{
		isAttack = false;
		if (isExitSit)
		{
			isSit = false;
			y -= SIMON_SIT_TO_STAND;
			isExitSit = false;
		}
		// Check collision between whip and game objects 
		whip->Update(dt, coObjects);

	}

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// Set idle state
	if (!isSit && !isMoving && !isJump && !isAttack)
	{
		SetState(SIMON_STATE_IDLE);
	}

	// reset untouchable timer if untouchable time has passed
	if (untouchable != 0 && GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// when simon attack
	if (isAttack == true && GetTickCount() - attackTime >= SIMON_TIMER_ATTACK - 50)
	{
		if (nx > 0)
		{
			// Whip position equal to simon position
			whip->SetPosition(x, y);
		}
		else
		{
			// Whip position equal to simon position + simon width - whip width
			float wl, wr, wt, wb;
			whip->GetBoundingBox(wl, wt, wr, wb);
			whip->SetPosition(x + SIMON_BBOX_WIDTH - (wr - wl), y);
		}
	}

	// Handle Simon go over screen camera
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	float leftCorner = cam_x;
	float rightCorner = leftCorner + SCREEN_WIDTH - SIMON_BBOX_WIDTH;
	// Left corner
	if (x < leftCorner)
	{
		x = 0;
	}
	// Right corner
	else if (x > rightCorner)
	{
		x = rightCorner;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		//DebugOut(L"[DY] dy = %f\n", dy);
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// Da cham dat
				if (isJump && e->nx == 0 && e->ny < 0)
					isJump = false;

				// Xét va chạm cứng
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<Item*>(e->obj))
			{
				Item* item = dynamic_cast<Item*>(e->obj);
				item->SetEaten();

				int type = item->GetType();
				switch (type)
				{
				case ITEM_HEART:
					AddHeart(5);
					break;
				case ITEM_WHIP:
					whip->UpLevel();
					break;
				case ITEM_KNIFE:
					SetCurrentWeapon(ITEM_KNIFE);
					break;
				case ITEM_MONEY:
					AddScore(1000);
					break;
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	int ani = -1, aniWhip = -1;

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE_RIGHT;
	else
	{
		if (isAttack)
		{
			if (nx > 0)
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				}
				else {
					ani = SIMON_ANI_ATTACK_RIGHT;
				}

				switch (whip->level)
				{
				case 0:
					aniWhip = WHIP_RIGHT;
					break;
				case 1:
					aniWhip = WHIP_RIGHT_1;
					break;
				case 2:
					aniWhip = WHIP_RIGHT_2;
					break;
				}
			}
			else
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				}
				else {
					ani = SIMON_ANI_ATTACK_LEFT;
				}


				switch (whip->level)
				{
				case 0:
					aniWhip = WHIP_LEFT;
					break;
				case 1:
					aniWhip = WHIP_LEFT_1;
					break;
				case 2:
					aniWhip = WHIP_LEFT_2;
					break;
				}
			}
		}
		else if (isJump)
		{
			if (nx > 0)
				ani = SIMON_ANI_JUMP_RIGHT;
			else
				ani = SIMON_ANI_JUMP_LEFT;
		}
		else if (nx > 0)
		{
			switch (state)
			{
			case SIMON_STATE_SIT:
				if (isAttack)
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				else
					ani = SIMON_ANI_SIT_RIGHT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_RIGHT;
				break;

			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_RIGHT;
				break;
			}
		}
		else if (nx < 0)
		{
			switch (state)
			{
			case SIMON_STATE_SIT:
				if (isAttack)
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				else
					ani = SIMON_ANI_SIT_LEFT;
				break;

			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_LEFT;
				break;

			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_LEFT;
				break;

			}
		}
	}


	int alpha = 255;
	if (untouchable) alpha = 128;

	if (ani != -1)
	{
		switch (ani)
		{
		case SIMON_ANI_SIT_RIGHT:
		case SIMON_ANI_SIT_LEFT:
			animation_set->at(ani)->Render(x, y + 7, alpha);
			break;
		case SIMON_ANI_SIT_ATTACK_RIGHT:
		case SIMON_ANI_SIT_ATTACK_LEFT:
			animation_set->at(ani)->Render(x - 6, y + 7, alpha);
			break;
		case SIMON_ANI_ATTACK_LEFT:
		case SIMON_ANI_ATTACK_RIGHT:
			animation_set->at(ani)->Render(x - 6, y, alpha);
			break;
		default:
			animation_set->at(ani)->Render(x, y, alpha);
			break;
		}
	}

	if (aniWhip != -1)
	{
		float fix_x = x;
		switch (aniWhip)
		{
		case WHIP_LEFT:
		case WHIP_LEFT_1:
			fix_x -= 28;
			break;

		case WHIP_LEFT_2:
			fix_x -= 44;
			break;
		case WHIP_RIGHT:
		case WHIP_RIGHT_1:
		case WHIP_RIGHT_2:
			fix_x -= 16;
			break;
		}
		if (!isSit)
			whip->animation_set->at(aniWhip)->Render(fix_x, y, alpha);
		else
			whip->animation_set->at(aniWhip)->Render(fix_x, y + (SIMON_BBOX_HEIGHT - SIMON_SIT_BBOX_HEIGHT), alpha);
	}

	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_DIE:
		isDead = true;
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;

	case SIMON_STATE_SIT:
		//isOnCheckStair = false;
		isSit = true;
		vx = 0;
		break;

	case SIMON_STATE_WALK:
		isMoving = true;
		if (nx > 0)
		{
			vx = SIMON_WALKING_SPEED;
		}
		else if (nx < 0)
		{
			vx = -SIMON_WALKING_SPEED;
		}
		break;

	case SIMON_STATE_IDLE:
		vx = 0;
		isMoving = false;
		break;

	}
}

void Simon::SetAction(int action)
{
	switch (action)
	{
	case SIMON_ACTION_ATTACK:
		// Ngat tat ca trang thai (tru ngoi)
		// Neu dang nhay toi thi de nguyen van toc, neu khong thi dung lai va danh
		isAttack = true;
		if (!isJump)
			vx = 0;
		isJump = false;
		isMoving = false;
		attackTime = GetTickCount();
		break;
		// Don gian la cho nhay, khong ngat bat ki trang thai nao
		// Them van toc nhay
	case SIMON_ACTION_JUMP:
		isJump = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	}
}

void Simon::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	whip->SetPosition(x, y);
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Simon::Reset()
{
	SetState(SIMON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

