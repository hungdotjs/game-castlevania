#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Knight.h"
#include "Torch.h"
#include "Candle.h"
#include "Bat.h"
#include "Elevator.h"
#include "Portal.h"
#include "CheckStairTop.h"
#include "Fleaman.h"
#include "Skeleton.h"
#include "Ghost.h"
#include "Raven.h"
#include "PhantomBat.h"
#include "Zombie.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	listGrids = ListGrids::GetInstance();
}

CPlayScene::~CPlayScene()
{
	if (map != NULL)
	{
		delete map;
		map = nullptr;
	}
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define OBJECT_TYPE_SIMON			0
#define OBJECT_TYPE_BRICK			1
#define OBJECT_TYPE_CHECKSTAIR		4
#define OBJECT_TYPE_CHECKSTAIRTOP	5
#define OBJECT_TYPE_WHIP			11
#define OBJECT_TYPE_FLEAMAN			14
#define OBJECT_TYPE_SKELETON		18
#define OBJECT_TYPE_GHOST			28
#define OBJECT_TYPE_RAVEN			32
#define OBJECT_TYPE_ZOMBIE			40
#define OBJECT_TYPE_PHANTOMBAT		44
#define OBJECT_TYPE_TORCH			100
#define OBJECT_TYPE_CANDLE			400
#define OBJECT_TYPE_ELEVATOR		402
#define OBJECT_TYPE_KNIGHT			500
#define OBJECT_TYPE_BAT				600
#define OBJECT_TYPE_HIDDENOBJECT	998
#define OBJECT_TYPE_HIDDENBRICK		999
#define OBJECT_TYPE_PORTAL			50

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
	arrTexturesID.push_back(texID);
}

void CPlayScene::_ParseSection_SPRITES(string line)
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
	arrSpritesID.push_back(ID);

}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
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
	arrAnimationsID.push_back(ani_id);

	DebugOut(L"[ANIMATION] Load animation id: %d\n", ani_id);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
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
	arrAnimationSetsID.push_back(ani_set_id);

	DebugOut(L"[ANIMATION] Load animation  set id: %d\n", ani_set_id);

}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player != NULL)
		{
			player->SetPosition(x, y);
			DebugOut(L"[ERROR] SIMON object was created before!\n");
			return;
		}
		player = Simon::GetInstance();
		player->SetPosition(x, y);

		//objects.push_back(player);
		return;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_TORCH: obj = new Torch(); break;
	case OBJECT_TYPE_CANDLE:
	{
		int itemID = atoi(tokens[4].c_str());
		obj = new Candle(itemID);
		break;
	}
	case OBJECT_TYPE_KNIGHT:
	{
		float minX = atof(tokens[4].c_str());
		float maxX = atof(tokens[5].c_str());
		obj = new Knight(minX, maxX);
		break;
	}
	case OBJECT_TYPE_ELEVATOR:
	{
		float minX = atof(tokens[4].c_str());
		float maxX = atof(tokens[5].c_str());
		obj = new Elevator(minX, maxX);
		break;
	}
	case OBJECT_TYPE_BAT: obj = new Bat(x, y); break;
	case OBJECT_TYPE_FLEAMAN: obj = new Fleaman(x, y); break;
	case OBJECT_TYPE_GHOST: obj = new Ghost(x, y); break;
	case OBJECT_TYPE_ZOMBIE: obj = new Zombie(); break;
	case OBJECT_TYPE_PHANTOMBAT: obj = new PhantomBat(); break;
	case OBJECT_TYPE_RAVEN: obj = new Raven(); break;
	case OBJECT_TYPE_SKELETON:
		obj = new Skeleton();
		break;
	case OBJECT_TYPE_CHECKSTAIR: {
		int type = atof(tokens[4].c_str());
		obj = new CheckStair(type);
		break;
	}
	case OBJECT_TYPE_CHECKSTAIRTOP: {
		int type = atof(tokens[4].c_str());
		obj = new CheckStairTop(type);
		break;
	}

	case OBJECT_TYPE_PORTAL: {
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}

	case OBJECT_TYPE_HIDDENBRICK:
		obj = new HiddenBrick();
		break;	
	case OBJECT_TYPE_HIDDENOBJECT:
	{
		float itemX = atof(tokens[4].c_str());
		float itemY = atof(tokens[5].c_str());
		int itemID = atoi(tokens[6].c_str());
		obj = new HiddenObject(itemX, itemY, itemID);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	//objects.push_back(obj);
	listGrids->AddObject(obj);

	DebugOut(L"[OBJECT] Load object %d\n", object_type);

}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	wstring path = ToWSTR(tokens[0]);
	CTextures* textures = CTextures::GetInstance();
	LPDIRECT3DTEXTURE9 tileset = textures->Get(ID_TEX_TILESET);

	map = new Map(tileset, 16, 16);
	map->ReadMapTXT(path.c_str());
}

void CPlayScene::Load()
{
	isUnload = false;
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	if (listGrids->isEmpty()) {
		int stage = CGame::GetInstance()->GetCurrentStage();

		switch (stage)
		{
		case 1:
			listGrids->InitList(MAP_1_WIDTH);
			break;
		case 2:
		case 7:
			listGrids->InitList(MAP_2_WIDTH);
			break;
		case 3:
			listGrids->InitList(MAP_3_WIDTH);
			break;
		case 4:
		case 8:
			listGrids->InitList(MAP_4_WIDTH);
			break;
		case 5:
			listGrids->InitList(MAP_5_WIDTH);
			break;
		case 6:
			listGrids->InitList(MAP_6_WIDTH);
			break;

		default:
			break;
		}
	}

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

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
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]")
		{
			section = SCENE_SECTION_MAP; continue;
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
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	board = new Board();
	board->Initialize(CGame::GetInstance()->GetDirect3DDevice(), player);
}

void CPlayScene::Update(DWORD dt)
{

	//skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	if (player->isWin) {
		player->SetState(SIMON_STATE_IDLE);
		if (gameTime / 1000 <= 0)
			isFinish = true;
		else {
			gameTime -= 1000;
			player->AddScore(10);
			board->Update(gameTime / 1000, CGame::GetInstance()->GetCurrentStage(), player);

		}
		return;
	}

	if (player->isDead && GetTickCount() > player->resetTime + TIME_RESET) {
		player->isDead = false;
		player->preHP = SIMON_HP;
		player->SetState(SIMON_STATE_IDLE);
		switch (CGame::GetInstance()->GetCurrentStage())
		{
		case 1:
			CGame::GetInstance()->SwitchScene(1);
			break;
		case 2:
		case 3:
			CGame::GetInstance()->SwitchScene(2);
			break;
		case 4:
		case 5:
			CGame::GetInstance()->SwitchScene(4);
			break;
		case 6:
			CGame::GetInstance()->SwitchScene(6);
			break;
		}
		return;
	}
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	if (player->x < 0)
		currentGrids = listGrids->GetCurrentGrids(0);
	else
		currentGrids = listGrids->GetCurrentGrids(player->x);

	for (int i = 0; i < currentGrids.size(); i++)
	{
		vector<LPGAMEOBJECT> listObjects = currentGrids[i]->GetListObject();
		int listObjectSize = listObjects.size();
		for (int j = 0; j < listObjectSize; j++)
		{
			coObjects.push_back(listObjects[j]);
		}
	}

	player->Update(dt, &coObjects);

	if (isUnload)
		return;

	if (isClockWeaponUsed)
	{
		if (GetTickCount() - clockWeaponCast > CLOCK_WEAPON_FREEZE_TIME)
		{
			isClockWeaponUsed = false;
		}
	}

	for (int i = 0; i < coObjects.size(); i++)
	{
		if (!dynamic_cast<Enemy*>(coObjects.at(i)))
			coObjects[i]->Update(dt, &coObjects);

		if (dynamic_cast<Enemy*>(coObjects.at(i))) {
			Enemy* enemy = dynamic_cast<Enemy*>(coObjects.at(i));

			if (isClockWeaponUsed) {
				enemy->isFrozen = true;
			}
			else {
				enemy->isFrozen = false;

				if (dynamic_cast<Skeleton*>(coObjects.at(i)))
				{
					Skeleton* skeleton = dynamic_cast<Skeleton*>(coObjects.at(i));

					if (skeleton->x < player->x) {
						skeleton->nx = 1;
					}
					else
						skeleton->nx = -1;

					int nx = skeleton->nx;
					if (GetTickCount() - skeleton->timer > 2000)
					{
						enemy = new Bone(nx);
						enemy->SetPosition(skeleton->x, skeleton->y - 10);
						listGrids->AddObject(enemy);
						skeleton->timer = GetTickCount();
					}
				}
				enemy->Update(dt, &coObjects);
			}

		}

		listGrids->UpdateObjectInGrid(coObjects[i]);
	}


	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();

	// Dung camera khi toi cuoi map
	int maxCamX = (int)cx + game->GetScreenWidth();
	if (maxCamX - game->GetScreenWidth() / 2 > map->mapWidth)
		cx = map->mapWidth - game->GetScreenWidth();
	else
		cx -= game->GetScreenWidth() / 2;

	cy -= game->GetScreenHeight() / 2;


	if (cx < 0) cx = 0;
	if (cy > 48) cy = 48;

	CGame::GetInstance()->SetCamPos(cx, 48);

	gameTime -= dt;
	if (gameTime / 1000 == 0) {
		player->SetState(SIMON_STATE_DIE);
		ResetGameTime();
	}
	board->Update(gameTime / 1000, CGame::GetInstance()->GetCurrentStage(), player);
	objects = coObjects;
	RemoveObjects(coObjects);
}

void CPlayScene::Render()
{
	if (player->isDead && GetTickCount() - player->resetTime > TIME_BLACK_SCREEN) {
		string lifeLeft = "X " + std::to_string(player->life);
		CSprites::GetInstance()->Get(1001)->Draw(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2);
		board->RenderTextScreen(SCREEN_WIDTH / 3 + 32, SCREEN_HEIGHT / 3, lifeLeft);
		ResetGameTime();
		return;
	}

	if (isFinish) {
		string text = "YOU WIN \n\n";
		text += "YOUR SCORE - " + std::to_string(Simon::score);
		board->RenderTextScreen(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, text);
		return;
	}

	float x, y;
	CGame::GetInstance()->GetCamPos(x, y);

	map->DrawMap(x, y);
	board->Render(x, y, player, CGame::bossHeath);


	//for (int i = 0; i < objects.size(); i++)
	//	objects[i]->Render();

	for (int i = 0; i < listGrids->GetListGrids().size(); i++)
	{
		vector<LPGAMEOBJECT> listObject = listGrids->GetListGrids()[i]->GetListObject();
		int listObjectSize = listObject.size();

		for (int j = 0; j < listObjectSize; j++)
		{
			listObject[j]->Render();
		}
	}

	player->Render();
}


void CPlayScene::RemoveObjects(vector<LPGAMEOBJECT>& objects)
{
	vector<LPGAMEOBJECT> listRemoveObjects;

	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Enemy*>(objects.at(i)))
		{
			Enemy* enemy = dynamic_cast<Enemy*>(objects.at(i));

			float x, y, right, bottom;
			enemy->GetBoundingBox(x, y, right, bottom);

			if (dynamic_cast<PhantomBat*>(objects.at(i))) {
				if (CGame::GetInstance()->bossHeath <= 0) {
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							Effect* effect = new Effect();
							effect->SetPosition(x + EFFECT_WIDTH * j, y + EFFECT_HEIGHT * i);
							objects.push_back(effect);
							listGrids->AddObject(effect);
						}
					}

					Item* item = new Item();
					item->SetPosition(x, y);
					item->SetSpeed(0, -0.1);
					item->SetType(ITEM_CRYSTAL);
					item->SetAnimationSet(CAnimationSets::GetInstance()->Get(ITEM_CRYSTAL));
					objects.push_back(item);
					listGrids->AddObject(item);

					listRemoveObjects.push_back(enemy);
					Simon::score += PHANTOMBAT_SCORE;
				}
			}

			if (enemy->health == 0)
			{
				Effect* effect = new Effect();
				effect->SetPosition(x + (right - x) / 2, y + (bottom - y) / 2);
				objects.push_back(effect);
				listGrids->AddObject(effect);

				listRemoveObjects.push_back(enemy);

				Simon::score += 100;
			}
			else if (enemy->isHitted) {
				// Thêm hiệu ứng tóe lửa
				EffectWhip* whipEffect = new EffectWhip();
				whipEffect->SetPosition(x + (right - x) / 2, y + (bottom - y) / 2);
				objects.push_back(whipEffect);
				listGrids->AddObject(whipEffect);

				enemy->isHitted = false;
			}
		}
		else if (dynamic_cast<HiddenBrick*>(objects.at(i)))
		{
			HiddenBrick* brick = dynamic_cast<HiddenBrick*>(objects.at(i));
			if (brick->isHitted)
			{
				BrokenEffect* effect = new BrokenEffect();
				effect->SetPosition(brick->x, brick->y);
				effect->SetSpeed(-0.08f, -0.08f);
				objects.push_back(effect);
				listGrids->AddObject(effect);

				effect = new BrokenEffect();
				effect->SetPosition(brick->x, brick->y);
				effect->SetSpeed(0.08f, -0.08f);
				objects.push_back(effect);
				listGrids->AddObject(effect);

				listRemoveObjects.push_back(brick);

			}
		}
		else if (dynamic_cast<HiddenObject*>(objects.at(i)))
		{
			HiddenObject* ho = dynamic_cast<HiddenObject*>(objects.at(i));
			if (ho->isTouched)
			{
				Item* item = new Item();
				item->SetPosition(ho->item_x, ho->item_y);
				item->SetSpeed(0, -0.1);

				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set;
				ani_set = animation_sets->Get(ho->itemID);
				item->SetType(ho->itemID);
				item->SetAnimationSet(ani_set);
				objects.push_back(item);
				listGrids->AddObject(item);

				listRemoveObjects.push_back(ho);

			}
		}
		else if (dynamic_cast<Bat*>(objects.at(i)))
		{
			Bat* bat = dynamic_cast<Bat*>(objects.at(i));
			if (bat->isHitted)
			{
				Effect* effect = new Effect();
				effect->SetPosition(bat->x + BAT_BBOX_WIDTH / 2, bat->y + BAT_BBOX_HEIGHT / 2);
				objects.push_back(effect);
				objects.erase(objects.begin() + i);
				player->AddScore(100);
			}
		}
		else if (dynamic_cast<Torch*>(objects.at(i)))
		{
			Torch* torch = dynamic_cast<Torch*>(objects.at(i));
			if (torch->isHitted)
			{
				float torch_x, torch_y, torch_right, torch_bottom;
				torch->GetBoundingBox(torch_x, torch_y, torch_right, torch_bottom);

				Effect* effect = new Effect();
				effect->SetPosition(torch_x + TORCH_BBOX_WIDTH / 2, torch_y + TORCH_BBOX_HEIGHT / 2);
				objects.push_back(effect);
				listGrids->AddObject(effect);

				Item* item = new Item();
				item->SetPosition(torch_x, torch_y);
				item->SetSpeed(0, -0.1);
				objects.push_back(item);
				listGrids->AddObject(item);

				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set;

				// Whip item
				if (player->whip->level < 2)
				{
					ani_set = animation_sets->Get(ITEM_WHIP);
					item->SetAnimationSet(ani_set);
					item->SetType(ITEM_WHIP);
				}
				else
				{
					/**
					 * Random ra item
					 * 70% heart
					 * 20% money
					 * 10% knife
					 */

					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					if (random_portion < 70)
					{
						ani_set = animation_sets->Get(ITEM_HEART);
						item->SetType(ITEM_HEART);
					}
					// Money
					else if (random_portion >= 70 && random_portion < 90)
					{
						ani_set = animation_sets->Get(ITEM_MONEY);
						item->SetType(ITEM_MONEY);
					}
					// Knife
					else
					{
						ani_set = animation_sets->Get(ITEM_KNIFE);
						item->SetType(ITEM_KNIFE);
					}
				}

				item->SetAnimationSet(ani_set);
				listRemoveObjects.push_back(torch);

			}
		}
		else if (dynamic_cast<Candle*>(objects.at(i))) {
			Candle* candle = dynamic_cast<Candle*>(objects.at(i));
			if (candle->isHitted)
			{
				float x, y, r, b;
				candle->GetBoundingBox(x, y, r, b);

				Effect* effect = new Effect();
				effect->SetPosition(x, y);
				objects.push_back(effect);
				listGrids->AddObject(effect);

				Item* item = new Item();
				item->SetPosition(x, y);
				item->SetSpeed(0, -0.1);
				objects.push_back(item);
				listGrids->AddObject(item);

				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set;

				// Whip item
				if (player->whip->level < 2)
				{
					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					if (random_portion < 40) {
						ani_set = animation_sets->Get(ITEM_WHIP);
						item->SetAnimationSet(ani_set);
						item->SetType(ITEM_WHIP);
					}
					else {
						ani_set = animation_sets->Get(candle->itemID);
						item->SetType(candle->itemID);
						item->SetAnimationSet(ani_set);
					}
				}
				else {
					ani_set = animation_sets->Get(candle->itemID);
					item->SetType(candle->itemID);
					item->SetAnimationSet(ani_set);
				}

				listRemoveObjects.push_back(candle);
			}
		}
		else if (dynamic_cast<Weapon*>(objects.at(i)))
		{
			Weapon* weapon = dynamic_cast<Weapon*>(objects.at(i));

			if (weapon->isExposed)
			{
				listRemoveObjects.push_back(weapon);
			}
		}
		else if (dynamic_cast<Item*>(objects.at(i)))
		{
			Item* item = dynamic_cast<Item*>(objects.at(i));
			if (item->GetType() == ITEM_CRYSTAL) {
				if (item->GetEaten()) {
					listRemoveObjects.push_back(item);

				}
			}
			else if (item->GetEaten() || GetTickCount() - item->appearTime > ITEM_LIVE_TIME)
			{
				listRemoveObjects.push_back(item);

			}
		}
		else if (dynamic_cast<Effect*>(objects.at(i)))
		{
			Effect* effect = dynamic_cast<Effect*>(objects.at(i));

			if (effect->GetExposed())
			{
				listRemoveObjects.push_back(effect);
			}
		}
		else if (dynamic_cast<BrokenEffect*>(objects.at(i)))
		{
			BrokenEffect* effect = dynamic_cast<BrokenEffect*>(objects.at(i));

			if (effect->GetExposed())
			{
				listRemoveObjects.push_back(effect);
			}
		}
		else if (dynamic_cast<EffectWhip*>(objects.at(i)))
		{
			EffectWhip* effect = dynamic_cast<EffectWhip*>(objects.at(i));

			if (effect->GetExposed())
			{
				listRemoveObjects.push_back(effect);
			}
		}

	}

	// Remove lần lượt từng object từ listRemoveObjects trong listGrids
	for (int i = 0; i < listRemoveObjects.size(); i++)
	{
		listGrids->RemoveObject(listRemoveObjects[i]);
		delete listRemoveObjects[i];
	}
}


/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	//player = NULL;
	isUnload = true;
	RemoveObjects(objects);
	objects.clear();

	listGrids->ReleaseList();

	CTextures::GetInstance()->Clear(arrTexturesID);
	CSprites::GetInstance()->Clear(arrSpritesID);
	CAnimations::GetInstance()->Clear(arrAnimationsID);
	CAnimationSets::GetInstance()->Clear(arrAnimationSetsID);

	arrTexturesID.clear();
	arrSpritesID.clear();
	arrAnimationsID.clear();
	arrAnimationSetsID.clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();

	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	switch (KeyCode)
	{
	case KEY_JUMP:	// Nhay
		if (simon->isJump == false && simon->isSit == false && simon->isAttack == false && simon->isOnStair == false && !simon->isHurt)
			simon->SetAction(SIMON_ACTION_JUMP);
		break;

	case DIK_1:
		simon->currentWeapon = ITEM_KNIFE;
		((CPlayScene*)scence)->GenerateWeapon();
		break;

	case DIK_2:
		simon->currentWeapon = ITEM_AXE;
		((CPlayScene*)scence)->GenerateWeapon();
		break;

	case DIK_3:
		simon->currentWeapon = ITEM_CROSS;
		((CPlayScene*)scence)->GenerateWeapon();
		break;

	case DIK_4:
		simon->currentWeapon = ITEM_HOLYWATER;
		((CPlayScene*)scence)->GenerateWeapon();
		break;

	case DIK_5:
		simon->currentWeapon = ITEM_CLOCK;
		((CPlayScene*)scence)->GenerateWeapon();
		break;
	case DIK_P:
		simon->SetState(SIMON_STATE_DIE);
		break;
	case KEY_ATTACK: // Danh
		if (game->IsKeyDown(KEY_UP))
		{
			if (simon->isAttack == false && simon->currentWeapon != 0)
			{
				simon->ResetAnimationAttack();

				if (simon->heartsAmount > 0)
				{
					simon->heartsAmount -= 1;
					((CPlayScene*)scence)->GenerateWeapon();
				}
			}
		}
		else if (simon->isAttack == false) {
			simon->ResetAnimationAttack();
			simon->SetAction(SIMON_ACTION_ATTACK);
			simon->isUseWhip = true;
		}
		break;
	case DIK_R:	// Reset
		simon->Reset();
		break;
	case DIK_F1:
	{
		int currScene = game->current_scene;
		switch (currScene) {
		case 1:
			game->SwitchScene(2);
			break;
		case 2:
		case 3:
			game->SwitchScene(4);
			break;
		case 4:
		case 5:
			game->SwitchScene(6);
			break;
		}
		break;
	}
	case DIK_F2:
	{
		int currScene = game->current_scene;
		switch (currScene) {
		case 2:
		case 3:
			game->SwitchScene(1);
			break;
			
		case 4:
		case 5:
			game->SwitchScene(2);
			break;
	
		case 6:
			game->SwitchScene(4);
			break;
		}
		break;
	}
	case DIK_F8:
		simon->preHP -= 2;
		break;
	case DIK_F9:
		simon->preHP += 2;
		break;

	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();

	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (KeyCode == KEY_UP)
	{
		if (simon->isOnStair)
		{
			simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
		}
		else if (simon->isOnCheckStairUp)
		{
			simon->SetState(SIMON_STATE_IDLE);
		}
		else {
			simon->SetState(SIMON_STATE_IDLE);
		}
	}

	// Ngoi
	if (KeyCode == KEY_DOWN)
	{
		if (simon->isOnCheckStairDown)
		{
			simon->SetState(SIMON_STATE_IDLE);
		}
		else if (simon->isOnStair)
		{
			simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
		}
		else if (simon->isSit)
		{
			if (!simon->isAttack)
			{
				simon->isSit = false;
			}
			else
			{
				simon->isExitSit = true;
			}
		}
	}

	// Di bo
	else if (KeyCode == KEY_RIGHT || KeyCode == KEY_LEFT)
	{
		simon->isMoving = false;
		simon->vx = 0;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();

	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (game->IsKeyDown(KEY_UP))
	{
		if (simon->ny == -1 &&
			!simon->isOnStair &&
			!simon->isAttack &&
			!simon->isSit &&
			!simon->isJump &&
			!simon->isHurt)
		{
			simon->SetState(SIMON_STATE_ONCHECKSTAIR);
		}
		else if (simon->isOnStair && !simon->isAttack)
		{
			simon->ny = -1;
			simon->SetState(SIMON_STATE_ONSTAIR);
			if (simon->isLeftToRight)
				simon->nx = 1;
			else
				simon->nx = -1;
		}
		else if (simon->isOnStair && simon->isAttack)
		{
			simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
		}
		else if (!simon->isOnCheckStairUp && !simon->isOnStair && !simon->isAttack && !simon->isJump)
			simon->SetState(SIMON_STATE_IDLE);

	}

	// Ngoi
	if (game->IsKeyDown(KEY_DOWN))
	{
		if (simon->ny == 1 && !simon->isOnStair)
		{
			if (!simon->isAttack && !simon->isSit && !simon->isJump)
			{
				simon->SetState(SIMON_STATE_ONCHECKSTAIR);
			}
		}
		else if (simon->isOnStair && !simon->isAttack)
		{
			simon->ny = 1;
			simon->SetState(SIMON_STATE_ONSTAIR);
			if (simon->isLeftToRight)
				simon->nx = -1;
			else
				simon->nx = 1;
		}
		else if (simon->isOnStair && simon->isAttack)
		{
			simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
		}
		else if (!simon->isOnCheckStairDown && !simon->isOnStair && !simon->isAttack && !simon->isJump)
			simon->SetState(SIMON_STATE_SIT);
	}

	// Di bo
	if (game->IsKeyDown(KEY_RIGHT))
	{
		if (simon->isOnStair || simon->isHurt || simon->isAttack) return;

		if (!simon->isSit && !simon->isAttack && !simon->isOnStair)
			simon->SetState(SIMON_STATE_WALK);
		if (simon->isJump) {
			simon->vx = SIMON_JUMP_SPEED_X;
		}
		if (!simon->isOnStair && !simon->isAttack || !simon->isJump)
			simon->nx = 1;
	}
	else if (game->IsKeyDown(KEY_LEFT))
	{
		if (simon->isOnStair || simon->isHurt || simon->isAttack) return;

		if (!simon->isSit && !simon->isAttack && !simon->isOnStair)
			simon->SetState(SIMON_STATE_WALK);
		if (simon->isJump) {
			simon->vx = -SIMON_JUMP_SPEED_X;
		}
		if (!simon->isOnStair && !simon->isAttack || !simon->isJump)
			simon->nx = -1;
	}

}

void CPlayScene::GenerateWeapon() {
	int nx = player->nx;

	player->SetAction(SIMON_ACTION_ATTACK);
	player->isUseWhip = false;

	switch (player->currentWeapon)
	{
	case ITEM_KNIFE:
		weapon = new Knife(player, SCREEN_WIDTH);
		weapon->nx = nx;
		if (nx > 0)
		{
			weapon->SetSpeed(KNIFE_SPEED, 0);
		}
		else if (nx < 0)
		{
			weapon->SetSpeed(-KNIFE_SPEED, 0);
		}

		weapon->SetPosition(player->x, player->y);
		weapon->firstCast = GetTickCount();
		listGrids->AddObject(weapon);
		break;

	case ITEM_AXE:
		weapon = new Axe(player);
		weapon->nx = nx;

		if (nx > 0)
		{
			weapon->SetSpeed(AXE_SPEED_X, -AXE_SPEED_Y);
		}
		else if (nx < 0)
		{
			weapon->SetSpeed(-AXE_SPEED_X, -AXE_SPEED_Y);
		}

		weapon->SetPosition(player->x, player->y);
		weapon->firstCast = GetTickCount();
		listGrids->AddObject(weapon);
		break;

	case ITEM_HOLYWATER:
		weapon = new HolyWater(player);

		if (nx > 0)
		{
			weapon->SetSpeed(HOLYWATER_SPEED_X, -HOLYWATER_SPEED_Y);
		}
		else if (nx < 0)
		{
			weapon->SetSpeed(-HOLYWATER_SPEED_X, -HOLYWATER_SPEED_Y);
		}

		weapon->SetPosition(player->x, player->y);
		weapon->firstCast = GetTickCount();
		listGrids->AddObject(weapon);
		break;

	case ITEM_CROSS:
		weapon = new Cross(player, SCREEN_WIDTH / 2);
		weapon->nx = nx;

		if (nx > 0)
		{
			weapon->SetSpeed(CROSS_SPEED, 0);
		}
		else if (nx < 0)
		{
			weapon->SetSpeed(-CROSS_SPEED, 0);
		}

		weapon->SetPosition(player->x, player->y);
		weapon->firstCast = GetTickCount();
		listGrids->AddObject(weapon);
		break;

	case ITEM_CLOCK:
		isClockWeaponUsed = true;
		clockWeaponCast = GetTickCount();
		break;

	}
}