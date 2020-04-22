#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Torch.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

CPlayScene::~CPlayScene()
{
	if (player != NULL)
	{
		delete player;
		player = nullptr;
	}
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

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_WHIP	11
#define OBJECT_TYPE_TORCH	100

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 2048


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
			DebugOut(L"[ERROR] SIMON object was created before!\n");
			return;
		}
		obj = new Simon();
		player = (Simon*)obj;
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_TORCH: obj = new Torch(); break;
	case OBJECT_TYPE_WHIP:
	{
		obj = new Whip();
		player->whip = (Whip*)obj;
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
		return;
	}

	case OBJECT_TYPE_PORTAL: {
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
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
	objects.push_back(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

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
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 0, 0));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	CTextures* textures = CTextures::GetInstance();

	LPDIRECT3DTEXTURE9 tileset = textures->Get(ID_TEX_TILESET);

	map = new Map(tileset, 16, 16);
	map->ReadMapTXT("textures\\map\\map_1.txt");

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	//skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;


	// Update camera to follow mario
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

	CGame::GetInstance()->SetCamPos(cx, 0 /*cy*/);

	RemoveObjects();
}

void CPlayScene::Render()
{
	float x, y;
	CGame::GetInstance()->GetCamPos(x, y);

	map->DrawMap(x, y);

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}


void CPlayScene::RemoveObjects() 
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Torch*>(objects.at(i)))
		{
			Torch* torch = dynamic_cast<Torch*>(objects.at(i));
			if (torch->isHitted)
			{
				float torch_x, torch_y, torch_right, torch_bottom;
				torch->GetBoundingBox(torch_x, torch_y, torch_right, torch_bottom);

				Item* item = new Item();
				item->SetPosition(torch_x, torch_y);
				item->SetSpeed(0, -0.1);
				objects.push_back(item);

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
				objects.erase(objects.begin() + i);
				delete torch;
			}
		}
		else if (dynamic_cast<Item*>(objects.at(i)))
		{
			Item* item = dynamic_cast<Item*>(objects.at(i));

			if (item->GetEaten())
			{
				objects.erase(objects.begin() + i);
				delete item;
			}
		}
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();

	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	switch (KeyCode)
	{
	case DIK_SPACE:	// Nhay
		if (simon->isJump == false && simon->isSit == false && simon->isAttack == false)
			simon->SetAction(SIMON_ACTION_JUMP);
		break;

	case DIK_A: // Danh
		if (simon->isAttack == false)
			simon->SetAction(SIMON_ACTION_ATTACK);
		break;

	case DIK_R:	// Reset
		simon->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();

	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	// Ngoi
	if (KeyCode == DIK_DOWN)
	{
		if (simon->isSit)
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
	else if (KeyCode == DIK_RIGHT || KeyCode == DIK_LEFT)
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

	// Ngoi
	if (game->IsKeyDown(DIK_DOWN))
	{
		if (!simon->isAttack && !simon->isJump)
			simon->SetState(SIMON_STATE_SIT);
	}

	// Di bo
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (!simon->isSit && !simon->isAttack)
			simon->SetState(SIMON_STATE_WALK);
		if (!simon->isAttack)
			simon->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (!simon->isSit && !simon->isAttack)
			simon->SetState(SIMON_STATE_WALK);
		if (!simon->isAttack)
			simon->nx = -1;
	}

}