#include "Map.h"

#define MAP_SECTION 10
#define MAP_SECTION_PROPS 11
#define MAX_SCENE_LINE 2048

Map::Map()
{
	tileWidth = TILE_WIDTH;
	tileHeight = TILE_HEIGHT;
}

Map::Map(LPDIRECT3DTEXTURE9 sprite, int _tileWidth, int _tileHeight)
{
	tileSet = sprite;
	tileWidth = _tileWidth;
	tileHeight = _tileHeight;
}

Map::~Map()
{
}


void Map::ReadMapTXT(char* filename)
{
	DebugOut(L"[INFO] Start loading map resources from : %s \n", filename);

	ifstream f;
	f.open(filename);

	int section = -1;
	char str[MAX_SCENE_LINE];

	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line == "") continue;	// skip comment lines	

		if (line == "[MAP_PROPERTY]") { section = MAP_SECTION_PROPS; continue; }
		if (line == "[MAP]") {
			section = MAP_SECTION;
			continue;
		}

		switch (section)
		{
		case MAP_SECTION_PROPS: _ParseSection_MAP_PROPS(line); break;
		case MAP_SECTION: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", filename);

}

void Map::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	/*int index = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i++)
	{
		tileMap[index][i - 1] = atoi(tokens[i].c_str()) - 1;
	}*/
	int index = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			tileMap[i][j] = atoi(tokens[index].c_str()) - 1;
			index += 1;
		}
	}
	int sa = 0;
}

void Map::_ParseSection_MAP_PROPS(string line)
{
	vector<string> tokens = split(line);

	this->rows = atoi(tokens[0].c_str());
	this->columns = atoi(tokens[1].c_str());
}


void Map::DrawMap(float x, float y)
{
	RECT tileRect;
	//D3DXVECTOR3 tilePos;

	/*int cameraWidth = 320;
	int cameraHeight = 240;*/

	//int colStart = (int)x / tileWidth;
	//int colEnd = ((int)x + cameraWidth) / tileWidth < columns - 1 ? (x + cameraWidth) / tileWidth : columns - 1;
	//int rowStart = (int)y / tileHeight;
	//int rowEnd = ((int)y + cameraHeight) / tileHeight < rows - 1 ? (y + cameraHeight) / tileHeight : rows - 1;

	int colStart = 0;
	int colEnd = 47;
	int rowStart = 0;
	int rowEnd = 11;
	

	for (int i = rowStart; i <= rowEnd; i++)
	{
		for (int j = colStart; j <= colEnd; j++)
		{
			//tileRect dung de lay ra RECT trong tile set de ve
			tileRect.left = (tileMap[i][j] % 11) * tileWidth; // 11 là số cột trong tileset
			tileRect.top = (tileMap[i][j] / 11) * tileHeight;
			tileRect.right = tileRect.left + tileWidth;
			tileRect.bottom = tileRect.top + tileHeight;

			CGame::GetInstance()->Draw(tileWidth * j, tileHeight * i, tileSet, tileRect.left, tileRect.top, tileRect.right, tileRect.bottom);
		}
	}
}

int Map::GetTileWidth()
{
	return tileWidth;
}

int Map::GetTileHeight()
{
	return tileHeight;
}

int Map::GetRows()
{
	return rows;
}

int Map::GetColumns()
{
	return columns;
}
