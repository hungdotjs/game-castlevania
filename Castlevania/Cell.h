#pragma once
#include "GameObject.h"
class Cell
{
	int iD;
	vector <LPGAMEOBJECT> listObj;
public:
	Cell();
	~Cell();
	void Insert(LPGAMEOBJECT object);
	vector<LPGAMEOBJECT> GetListObjects() { return listObj; }
	void Unload();
};

class Cell;
typedef Cell* LPCELL;