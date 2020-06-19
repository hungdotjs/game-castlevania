#include "Simon.h"

int Simon::score = 0;
int Simon::heartsAmount = 5;

Simon* Simon::__instance = NULL;

Simon::Simon(float x, float y) : CGameObject()
{
	SetState(SIMON_STATE_IDLE);
	level = SIMON_LEVEL;
	whip = new Whip(0);

	life = 3;
	preHP = 16;
	hp = 16;
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
		float cbl, cbr, cbt, cbb;
		coObjects->at(i)->GetBoundingBox(cbl, cbt, cbr, cbb);

		if (dynamic_cast<Bat*>(coObjects->at(i)))
		{
			Bat* bat = dynamic_cast<Bat*>(coObjects->at(i));

			if (x < bat->start_x + 96 &&
				x > bat->start_x - 96 &&
				y < bat->start_y + 48 &&
				y > bat->start_y - 48)
			{
				bat->SetState(BAT_STATE_FLYING);

				if (x < bat->start_x)
					bat->vx = -BAT_FLY_SPEED;
				else
					bat->vx = BAT_FLY_SPEED;
			}
		}

		else if (dynamic_cast<Fleaman*>(coObjects->at(i)))
		{
			Fleaman* fleaman = dynamic_cast<Fleaman*>(coObjects->at(i));

			if (x < fleaman->x + 160 &&
				x > fleaman->x - 160 &&
				y < fleaman->y + 96 &&
				y > fleaman->y - 96)
			{

				if (x < fleaman->x)
					fleaman->nx = -1;
				else
					fleaman->nx = 1;
				if (fleaman->state == FLEAMAN_STATE_IDLE)
					fleaman->SetState(FLEAMAN_STATE_ATTACK);
			}
		}

		// Xet va cham len xuong cau thang
		if (dynamic_cast<CheckStair*>(coObjects->at(i))) {
			CheckStair* checkstair = dynamic_cast<CheckStair*>(coObjects->at(i));
			float csl, csr, cst, csb;
			checkstair->GetBoundingBox(csl, cst, csr, csb);

			/*	DebugOut(L"[STAIR] checkstair left = %f, top = %f,\n right = %f, bottom = %f \n", csl, cst, csr, csb);
				DebugOut(L"[STAIR] simon x = %f, y = %f \n", x, y);*/


				// Neu Simon dung tai checkstair bottom
			if (x < csr &&
				x + SIMON_BBOX_WIDTH > csl
				&& y + SIMON_BBOX_HEIGHT > cst
				&& y + SIMON_BBOX_HEIGHT < csb)
			{
				isCollideWithCheckBox = true;

				if (ny > 0) {
					isOnStair = false;
					SetState(SIMON_STATE_IDLE);
				}

				//DebugOut(L"[STATE] state = %d\n", state);
				int type = checkstair->GetType();

				// Xet truong hop nhan phim len
				switch (type)
				{
				case CHECKSTAIR_UP_LEFT:
					ny = -1.0f;

					isOnCheckStairUp = true;

					if (state == SIMON_STATE_ONCHECKSTAIR)
					{
						nx = -1.0f;

						isMoving = true;

						vx = 0;
						vy = 0;

						SetPosition(csl + SIMON_BBOX_WIDTH / 2, cst - SIMON_BBOX_HEIGHT / 2);

						SetState(SIMON_STATE_ONSTAIR_IDLE);

						isLeftToRight = false;
						isOnStair = true;
						isOnCheckStairUp = false;

					}
					else
					{
						isOnCheckStairUp = false;
					}
					break;

				case CHECKSTAIR_UP_RIGHT:
					ny = -1.0f;

					isOnCheckStairUp = true;

					if (state == SIMON_STATE_ONCHECKSTAIR)
					{
						nx = 1.0f;

						isMoving = true;


						vx = 0;
						vy = 0;
						SetPosition(csl - SIMON_BBOX_WIDTH / 2, cst - SIMON_BBOX_HEIGHT / 2);

						SetState(SIMON_STATE_ONSTAIR_IDLE);


						isLeftToRight = true;
						isOnStair = true;
						isOnCheckStairUp = false;

					}
					else
					{
						isOnCheckStairUp = false;
					}
					break;
				}
			}

		}
		else if (dynamic_cast<CheckStairTop*>(coObjects->at(i)))
		{
			CheckStairTop* checkstair = dynamic_cast<CheckStairTop*>(coObjects->at(i));
			float csl, csr, cst, csb;
			checkstair->GetBoundingBox(csl, cst, csr, csb);

			/*	DebugOut(L"[STAIR] checkstair left = %f, top = %f,\n right = %f, bottom = %f \n", csl, cst, csr, csb);
				DebugOut(L"[STAIR] simon x = %f, y = %f \n", x, y);*/

				// Neu Simon dung tai checkstair bottom
			if (x < csr &&
				x + SIMON_BBOX_WIDTH > csl &&
				y > cst &&
				y < csb)
			{
				isCollideWithCheckBox = true;

				if (ny < 0) {
					isOnStair = false;
					SetState(SIMON_STATE_IDLE);
				}

				int type = checkstair->GetType();

				// Xet truong hop nhan phim len
				switch (type)
				{
				case CHECKSTAIR_DOWN_LEFT:
					ny = 1.0f;

					isOnCheckStairDown = true;

					if (state == SIMON_STATE_ONCHECKSTAIR)
					{
						nx = -1.0f;

						isMoving = true;

						vx = 0;
						vy = 0;

						SetPosition(csl - SIMON_BBOX_WIDTH / 2, csb);

						SetState(SIMON_STATE_ONSTAIR_IDLE);

						isLeftToRight = true;
						isOnStair = true;
						isOnCheckStairDown = false;

					}
					else
					{
						isOnCheckStairDown = false;
					}
					break;

				case CHECKSTAIR_DOWN_RIGHT:
					ny = 1.0f;

					isOnCheckStairDown = true;

					if (state == SIMON_STATE_ONCHECKSTAIR)
					{
						nx = 1.0f;

						isMoving = true;

						vx = 0;
						vy = 0;
						SetPosition(csl + SIMON_BBOX_WIDTH / 2, csb);

						SetState(SIMON_STATE_ONSTAIR_IDLE);


						isLeftToRight = false;
						isOnStair = true;
						isOnCheckStairDown = false;

					}
					else
					{
						isOnCheckStairDown = false;
					}
					break;
				}
			}
		}
		// Simon se khong va cham voi nhung vat sau:
		else if (!dynamic_cast<Torch*>(coObjects->at(i)) &&
			!dynamic_cast<Candle*>(coObjects->at(i)) &&
			!dynamic_cast<Item*>(coObjects->at(i)) &&
			!dynamic_cast<Enemy*>(coObjects->at(i))
			)
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}

		if (!isDead) {
			if (dynamic_cast<Item*>(coObjects->at(i)))
			{
				Item* item = dynamic_cast<Item*>(coObjects->at(i));

				float sl, sr, st, sb;
				GetBoundingBox(sl, st, sr, sb);

				if (sl < cbr && sr > cbl && st < cbb && sb > cbt)
				{
					item->SetEaten();

					int type = item->GetType();
					switch (type)
					{
					case ITEM_MINIHEART:
						AddHeart(1);
						break;
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
				
			}
			else if (dynamic_cast<Enemy*>(coObjects->at(i)) && !isUntouchable)
			{
				float sl, sr, st, sb;
				GetBoundingBox(sl, st, sr, sb);

				if (sl < cbr && sr > cbl && st < cbb && sb > cbt)
				{
					SetState(SIMON_STATE_HURT);
					willHurt = true;
					preHP -= 1;
					StartUntouchable();
				}

			}
		}

	}

	if (!isCollideWithCheckBox && !isOnStair)
	{
		isOnCheckStairDown = false;
		isOnCheckStairUp = false;
		ny = 0;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (isJump || isAttack || isHurt && !isOnStair)
	{
		vy += SIMON_GRAVITY_JUMP * dt;
	}
	else if (!isOnStair)
	{
		vy += SIMON_GRAVITY * dt;
	}

	if (isOnStair) vy = 0;

	//DebugOut(L"[DIRECTION] nx = %d, ny = %d\n", nx, ny);
	//DebugOut(L"[POSITION] x = %f, y = %f\n", x, y);
	//DebugOut(L"[POSITION] vx = %f, vy = %f\n", vx, vy);

	// Has completed attack animation
	// when simon attack
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
		if (isUseWhip)
			whip->Update(dt, coObjects);

	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// Set idle state
	if (!isDead && !isOnStair && !isSit && !isMoving && !isJump && !isAttack && !isHurt)
	{
		SetState(SIMON_STATE_IDLE);
	}

	// reset untouchable timer if untouchable time has passed
	if (isUntouchable && GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		isUntouchable = false;
	}

	if (isAttack && isUseWhip)
	{
		if (nx > 0)
		{
			// Whip position equal to simon position
			whip->SetPosition(x, y + SIMON_BBOX_HEIGHT / 3);
			whip->nx = 1;
		}
		else
		{
			// Whip position equal to simon position + simon width - whip width
			whip->nx = -1;
			float wl, wr, wt, wb;
			whip->GetBoundingBox(wl, wt, wr, wb);
			whip->SetPosition(x + SIMON_BBOX_WIDTH - (wr - wl), y + SIMON_BBOX_HEIGHT / 3);
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

	if (y < 76) y = 76;

	if (y > 500)
	{
		SetState(SIMON_STATE_DIE);
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

		// block every object first!
		if (!isUntouchable) {
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
		}

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
				if (isOnStair) {
					x += dx;
					y += dy;
					continue;
				}

				// Da cham dat
				if (isJump && e->nx == 0 && e->ny < 0)
				{
					isJump = false;
				}

				if (e->ny < 0) {
					if (isHurt)
					{
						vx = 0;
						isHurt = false;
					}
				}

				// Xét va chạm cứng
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;

			}

			else if (dynamic_cast<Elevator*>(e->obj)) {
				Elevator* elevator = dynamic_cast<Elevator*>(e->obj);

				if (elevator->nx > 0) {
					vx = elevator->vx * 2;
				}
				else {
					vx = -elevator->vx * 2;
				}
				x += dx;
				isJump = false;

				if (nx != 0) vx = 0;
				if (ny != 0) {
					vy = 0;
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
		if (isHurt) {
			if (nx > 0)
				ani = SIMON_ANI_HURT_RIGHT;
			else
				ani = SIMON_ANI_HURT_LEFT;
		}
		else if (isAttack)
		{
			if (nx > 0)
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				}
				else if (isOnStair)
				{
					if (ny < 0)
						ani = SIMON_ANI_STAIRUP_ATTACK_RIGHT;
					else
						ani = SIMON_ANI_STAIRDOWN_ATTACK_RIGHT;
				}
				else {
					ani = SIMON_ANI_ATTACK_RIGHT;
				}

				if (isUseWhip) {
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
			}
			else
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				}
				else if (isOnStair)
				{
					if (ny < 0)
						ani = SIMON_ANI_STAIRUP_ATTACK_LEFT;
					else
						ani = SIMON_ANI_STAIRDOWN_ATTACK_LEFT;
				}
				else {
					ani = SIMON_ANI_ATTACK_LEFT;
				}

				if (isUseWhip)
				{
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
			case SIMON_STATE_ONCHECKSTAIR:
				ani = SIMON_ANI_WALKING_RIGHT;
				break;
			case SIMON_STATE_ONSTAIR:
				if (ny < 0)
				{
					ani = SIMON_ANI_WALKING_UP_RIGHT;
				}
				else if (ny > 0)
				{
					ani = SIMON_ANI_WALKING_DOWN_RIGHT;
				}
				break;
			case SIMON_STATE_ONSTAIR_IDLE:
				if (ny < 0)
				{
					ani = SIMON_ANI_IDLE_UP_RIGHT;
				}
				else if (ny > 0)
				{
					ani = SIMON_ANI_IDLE_DOWN_RIGHT;
				}
				break;
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
			case SIMON_STATE_ONCHECKSTAIR:
				ani = SIMON_ANI_WALKING_LEFT;
				break;
			case SIMON_STATE_ONSTAIR:
				if (ny < 0)
				{
					ani = SIMON_ANI_WALKING_UP_LEFT;
				}
				else if (ny > 0)
				{
					ani = SIMON_ANI_WALKING_DOWN_LEFT;
				}
				break;
			case SIMON_STATE_ONSTAIR_IDLE:
				if (ny < 0)
				{
					ani = SIMON_ANI_IDLE_UP_LEFT;
				}
				else if (ny > 0)
				{
					ani = SIMON_ANI_IDLE_DOWN_LEFT;
				}
				break;
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
	if (isUntouchable) {
		// Nhap nhay
		if (transparent)
			alpha = 125;
		transparent = !transparent;
	}

	if (ani != -1)
	{
		switch (ani)
		{
		case SIMON_ANI_SIT_RIGHT:
		case SIMON_ANI_SIT_LEFT:
		case SIMON_ANI_HURT_LEFT:
		case SIMON_ANI_HURT_RIGHT:
			animation_set->at(ani)->Render(x, y + 7, alpha);
			break;
		case SIMON_ANI_SIT_ATTACK_RIGHT:
		case SIMON_ANI_SIT_ATTACK_LEFT:
			animation_set->at(ani)->Render(x - 6, y + 7, alpha);
			break;
		case SIMON_ANI_ATTACK_LEFT:
		case SIMON_ANI_ATTACK_RIGHT:
		case SIMON_ANI_STAIRUP_ATTACK_LEFT:
		case SIMON_ANI_STAIRUP_ATTACK_RIGHT:
		case SIMON_ANI_STAIRDOWN_ATTACK_LEFT:
		case SIMON_ANI_STAIRDOWN_ATTACK_RIGHT:
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

	/*whip->RenderBoundingBox();*/
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

	case SIMON_STATE_ONCHECKSTAIR:
		isOnStair = true;
		if (nx < 0)
		{
			vx = -SIMON_WALKING_SPEED;
		}
		else
		{
			vx = SIMON_WALKING_SPEED;
		}
		vy = 0;
		break;

	case SIMON_STATE_ONSTAIR:
		isOnStair = true;
		if (!isAttack)
		{
			isMoving = true;
			if (nx < 0)
			{
				if (ny < 0)
				{
					vx = -SIMON_CLIMBING_SPEED_X;
					vy = -SIMON_CLIMBING_SPEED_Y;
				}
				else
				{
					vx = -SIMON_CLIMBING_SPEED_X;
					vy = SIMON_CLIMBING_SPEED_Y;
				}
			}
			else
			{
				if (ny < 0)
				{
					vx = SIMON_CLIMBING_SPEED_X;
					vy = -SIMON_CLIMBING_SPEED_Y;
				}
				else
				{
					vx = SIMON_CLIMBING_SPEED_X;
					vy = SIMON_CLIMBING_SPEED_Y;
				}
			}
		}
		break;

	case SIMON_STATE_ONSTAIR_IDLE:
		isMoving = false;
		vx = 0;
		vy = 0;
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
	case SIMON_STATE_HURT:
		isHurt = true;

		vy = -SIMON_HURT_DEFLECT_SPEED_Y;
		if (nx > 0)
			vx = -SIMON_HURT_DEFLECT_SPEED_X;
		else
			vx = SIMON_HURT_DEFLECT_SPEED_X;

		isAttack = false;
		isJump = false;
		isMoving = false;
		isOnStair = false;
		isOnCheckStairDown = false;
		isOnCheckStairUp = false;
		isSit = false;
		isExitSit = false;

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
		// nhay khong ngat bat ki trang thai nao
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

void Simon::ResetAnimationAttack() {
	int ani = 0;
	int aniWhip = 0;
	if (this->nx > 0)
	{
		if (isSit)
			ani = SIMON_ANI_SIT_ATTACK_RIGHT;
		else
			ani = SIMON_ANI_ATTACK_RIGHT;

		if (whip->level == 0)
			aniWhip = WHIP_RIGHT;
		else if (whip->level == 1)
			aniWhip = WHIP_RIGHT_1;
		else
			aniWhip = WHIP_RIGHT_2;
	}
	else if (this->nx < 0)
	{
		if (isSit)
			ani = SIMON_ANI_SIT_ATTACK_LEFT;

		else
			ani = SIMON_ANI_ATTACK_LEFT;


		if (whip->level == 0)
			aniWhip = WHIP_LEFT;

		else if (whip->level == 1)
			aniWhip = WHIP_LEFT_1;

		else
			aniWhip = WHIP_LEFT_2;

	}
	animation_set->at(ani)->ResetFrame();
	whip->animation_set->at(aniWhip)->ResetFrame();
}

// Load Simon 
void Simon::Load(LPCWSTR simonFile)
{
	DebugOut(L"[INFO] Start loading simon resources from : %s \n", simonFile);

	ifstream f;
	f.open(simonFile);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines
		if (line[0] == '/' && line[1] == '/') continue;	// skip comment lines

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[SETTINGS]") {
			section = SCENE_SECTION_SETTINGS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"Resources\\Textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading simon resources %s\n", simonFile);
}

void Simon::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void Simon::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void Simon::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);

	DebugOut(L"[ANIMATION] Load animation id: %d\n", ani_id);
}

void Simon::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}
	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void Simon::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int id = atoi(tokens[0].c_str());
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(id);

	if (tokens[1] == "SIMON")
	{
		SetAnimationSet(ani_set);
	}
	else if (tokens[1] == "WHIP")
	{

		SetWhipAnimationSet(ani_set);
	}
	else
		DebugOut(L"[ERROR] Unknown scene setting %s\n", ToWSTR(tokens[0]).c_str());
}

void Simon::SetWhipAnimationSet(LPANIMATION_SET ani_set)
{
	whip->SetAnimationSet(ani_set);
}