#pragma once
#include <time.h>  
#include <stdlib.h>

#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Map.h"
#include "Board.h"
#include "Effect.h"
#include "Grid.h"
#include "Simon.h"


#define SCENE_SECTION_UNKNOWN		-1
#define SCENE_SECTION_SETTINGS		1
#define SCENE_SECTION_TEXTURES		2
#define SCENE_SECTION_SPRITES		3
#define SCENE_SECTION_ANIMATIONS	4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_MAP			7
#define SCENE_SECTION_GRID			8

#define OBJECT_TYPE_SIMON			0
#define OBJECT_TYPE_BRICK			1
#define OBJECT_TYPE_CHECKSTAIR		4
#define OBJECT_TYPE_CHECKSTAIRTOP	5
#define OBJECT_TYPE_WHIP			11
#define OBJECT_TYPE_FLEAMAN			14
#define OBJECT_TYPE_SKELETON		18
#define OBJECT_TYPE_TORCH			100
#define OBJECT_TYPE_CANDLE			400
#define OBJECT_TYPE_ELEVATOR		402
#define OBJECT_TYPE_KNIGHT			500
#define OBJECT_TYPE_BAT				600

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 2048

class Simon;
class Board;

class CPlayScene : public CScene
{
protected:
	Simon* player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> listItems;

	vector<int> arrTexturesID;
	vector<int> arrSpritesID;
	vector<int> arrAnimationsID;
	vector<int> arrAnimationSetsID;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_GRID(string line);
	void _Load_OBJECTS(string line);

public:
	Map* map;
	Board* board;
	Grid* grid = NULL;

	DWORD gameTime = 300000;
	int stage;
	int defaultTimeGame;
	int remainTime;

	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void RemoveObjects();

	Simon* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
	~CPlayScene();
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

