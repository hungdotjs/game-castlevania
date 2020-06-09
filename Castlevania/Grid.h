#pragma once
#include "GameObject.h"
#include "Utils.h"
#include <math.h>
#include "Torch.h"
#include "Portal.h"
#include "Candle.h"
#include "Bat.h"
#include "Skeleton.h"
#include "Fleaman.h"
#include "Knight.h"

using namespace std;

class GridObjects
{
	float left, right;

public:
	vector<LPGAMEOBJECT> listObject;

	GridObjects(float left, float right)
	{
		this->left = left;
		this->right = right;
	}

	void GetPoint(float& left, float& right);
	vector<LPGAMEOBJECT> GetListObject();

	void AddObject(LPGAMEOBJECT object);
	void RemoveObject(int i);
};

class ListGrids
{
	static ListGrids* instance;
	vector<GridObjects*> listGrids;

public:
	void InitList(float mapWidth);
	void ReleaseList();

	void RemoveGrid(int i);
	bool isEmpty();                                                    
	void AddObject(LPGAMEOBJECT object);
	void RemoveObject(LPGAMEOBJECT object);
	vector<GridObjects*> GetListGrids() { return listGrids; }
	static ListGrids* GetInstance();
	vector<GridObjects*> GetCurrentGrids(float cam_x);
	void UpdateObjectInGrid(LPGAMEOBJECT object);
};