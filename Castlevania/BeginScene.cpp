#include <iostream>
#include <fstream>

#include "BeginScene.h"
#include "Map.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"

using namespace std;

BeginScene::BeginScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new BeginSceneKeyHandler(this);
	map = new Map();
}

BeginScene::~BeginScene()
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

void BeginScene::_ParseSection_TEXTURES(string line)
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

void BeginScene::_ParseSection_SPRITES(string line)
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

void BeginScene::_ParseSection_ANIMATIONS(string line)
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

void BeginScene::_ParseSection_ANIMATION_SETS(string line)
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
void BeginScene::_ParseSection_OBJECTS(string line)
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

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	//objects.push_back(obj)

	DebugOut(L"[OBJECT] Load object %d\n", object_type);

}

void BeginScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	wstring path = ToWSTR(tokens[0]);
	CTextures* textures = CTextures::GetInstance();
	LPDIRECT3DTEXTURE9 tileset = textures->Get(ID_TEX_TILESET);

	map = new Map(tileset, 16, 16);
	map->ReadMapTXT(path.c_str());
}

void BeginScene::Load()
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

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 0));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

}

void BeginScene::Update(DWORD dt)
{



}

void BeginScene::Render()
{
	CSprites::GetInstance()->Get(101)->Draw(16, 16);
	CAnimationSets::GetInstance()->Get(102)->at(0)->Render(206, 106);
}


/*
	Unload current scene
*/
void BeginScene::Unload()
{
	//player = NULL;

	objects.clear();

	CTextures::GetInstance()->Clear(arrTexturesID);
	CSprites::GetInstance()->Clear(arrSpritesID);
	CAnimations::GetInstance()->Clear(arrAnimationsID);
	CAnimationSets::GetInstance()->Clear(arrAnimationSetsID);

	arrTexturesID.clear();
	arrSpritesID.clear();
	arrAnimationsID.clear();
	arrAnimationSetsID.clear();

	DebugOut(L"[INFO 2] Scene %s unloaded! \n", sceneFilePath);
}

void BeginSceneKeyHandler::KeyState(BYTE* states)
{
}

void BeginSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();

	switch (KeyCode)
	{
	case DIK_RETURN:
		game->SwitchScene(1);
		break;
	}

}

void BeginSceneKeyHandler::OnKeyUp(int KeyCode)
{
}
