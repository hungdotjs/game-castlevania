#include "Grid.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Utils.h"

using namespace std;

#define GRID_SECTION_UNKNOWN -1
#define GRID_SECTION_SETTINGS 0
#define GRID_SECTION_OBJECTS 2

void Grid::LoadSetting(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2)
	{
		return;
	}

	cellSize = atof(tokens[0].c_str());
	numXCells = atof(tokens[1].c_str());
	numYCells = atof(tokens[2].c_str());

	cells = new LPCELL[numXCells];

	for (int i = 0; i < numXCells; i++)
		cells[i] = new Cell[numYCells];
}

void Grid::LoadObject(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2)
	{
		return;
	}

	int indexX = atoi(tokens[0].c_str());
	int indexY = atoi(tokens[1].c_str());
	int idObject;

	for (int j = 2; j < tokens.size(); j++)
	{
		idObject = atoi(tokens[j].c_str());
		cells[indexX][indexY].Insert(listObject->at(idObject));
	}
}

Grid::Grid(string pathFile, vector <LPGAMEOBJECT>* listObject)
{
	this->listObject = listObject;

	fstream pFile;
	pFile.open(pathFile, fstream::in);
	string line;
	int i = 0;
	int idObject;

	int section = GRID_SECTION_UNKNOWN;
	while (pFile.good())
	{
		getline(pFile, line);
		if (line.find("END") != string::npos)
			break;

		if (line[0] == '#') continue;	// skip comment lines

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}

		switch (section)
		{
		case GRID_SECTION_SETTINGS: LoadSetting(line); break;
		case GRID_SECTION_OBJECTS: LoadObject(line); break;
		}
	}

	pFile.close();
}

Grid::~Grid()
{

}


bool checkContainId(vector<LPGAMEOBJECT>* list_object, LPGAMEOBJECT e)
{
	LPGAMEOBJECT obj;
	for (int i = 0; i < list_object->size(); i++)
	{
		obj = list_object->at(i);
		if (obj->GetID() == e->GetID()) {
			return true;
		}
	}
	return false;
}

void Grid::GetListOfObjects(vector<LPGAMEOBJECT>* list_object, int screenWidth, int screenHeight)
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	list_object->clear();
	int left, top, right, bottom;
	int i, j, k;

	left = (int)cam_x / cellSize;
	top = (int)cam_y / cellSize;

	right = (int)(cam_x + screenWidth) / cellSize
		+ ((int)(cam_x + screenWidth) % cellSize ? 1 : 0);

	bottom = (int)(cam_y + screenHeight) / cellSize
		+ ((int)(cam_y + screenHeight) % cellSize ? 1 : 0);

	LPGAMEOBJECT e;

	if (right < 0 || left > numXCells || bottom < 0 && top > numYCells)
	{
		return;
	}

	if (right > numXCells)
	{
		right = numXCells;
	}
	if (bottom > numYCells)
	{
		bottom = numYCells;
	}

	if (left < 0)
	{
		left = 0;
	}
	if (top > 0)
	{
		top = 0;
	}

	LPCELL cell;
	for (i = left; i < right; i++)
	{
		for (j = top; j < bottom; j++)
		{
			LPCELL cell = &cells[i][j];
			if (cell)
			{
				if (cells[i][j].GetListObjects().size() != 0)
				{
					for (k = 0; k < cells[i][j].GetListObjects().size(); k++)
					{
						e = cells[i][j].GetListObjects().at(k);
						if (!checkContainId(list_object, e)) {
							list_object->push_back(e);
						}

						//list_object->push_back(e);
					}
				}
			}

		}
	}

}

void Grid::Unload()
{
	if (cells)
	{
		for (int i = 0; i < numXCells; i++)
		{
			for (int j = 0; j < numYCells; j++)
			{
				cells[i][j].Unload();
			}
		}
		delete cells;
		cells = NULL;
	}
}

