#include "Cell.h"

Cell::Cell()
{
	listObj.clear();
}


Cell::~Cell()
{

}

void Cell::Insert(LPGAMEOBJECT object)
{
	listObj.push_back(object);
}

void Cell::Unload()
{
	listObj.clear();
}
