#pragma once

#include "Textures.h"
#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240

class Map
{
private:
	int columns, rows;
	int tileWidth, tileHeight;

	int tileMap[500][500];

	LPDIRECT3DTEXTURE9 tileSet;

public:
	int mapWidth;
	int mapHeght;

	Map();
	Map(LPDIRECT3DTEXTURE9 sprite, int tileWidth, int tileHeight);

	void ReadMapTXT(LPCWSTR filename);
	void DrawMap(float x, float y);

	int GetColumns();
	int GetRows();
	int GetTileWidth();
	int GetTileHeight();

	void _ParseSection_MAP(string line);
	void _ParseSection_MAP_PROPS(string line);

	~Map();

};
