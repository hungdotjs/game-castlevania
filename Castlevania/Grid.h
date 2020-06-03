#pragma once
#include "GameObject.h"
#include <vector>
#include "Game.h"
#include "Cell.h"
#include "Camera.h"

class Grid
{
	friend class CGameObject;
	int cellSize;
	Cell** cells;
	int numXCells;
	int numYCells;
	vector <LPGAMEOBJECT>* listObject;
	void LoadSetting(string line);
	void LoadObject(string line);
public:
	Grid(string file, vector <LPGAMEOBJECT>* listObject);
	~Grid();
	void GetListOfObjects(vector<LPGAMEOBJECT>* list_object, int screenWidth, int screenHeight);
	void Unload();
};

