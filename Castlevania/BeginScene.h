#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Map.h"


class BeginScene : public CScene
{
protected:
	vector<LPGAMEOBJECT> objects;
	//vector<LPGAMEOBJECT> coObjects;

	vector<int> arrTexturesID;
	vector<int> arrSpritesID;
	vector<int> arrAnimationsID;
	vector<int> arrAnimationSetsID;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

public:
	int stage;
	Map* map;

public:
	BeginScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//friend class CPlayScenceKeyHandler;
	~BeginScene();
};

class BeginSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	BeginSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

