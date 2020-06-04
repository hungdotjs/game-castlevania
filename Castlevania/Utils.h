#pragma once

#include <Windows.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <algorithm>
#include <fstream> 
#include <string>
#include <vector>

#define GAME_FONT L"textures\\font\\prstart.ttf"


#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256

#define ID_TEX_TILESET 100
#define ID_TEX_MISC		20

#define MAX_FRAME_RATE 120
#define MAX_SCENE_LINE			 2048

#define MAP_1_WIDTH		768
#define MAP_2_WIDTH		240
#define MAP_3_WIDTH		512
#define MAP_4_WIDTH		768


#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_SETTINGS			1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_MAP				7


using namespace std;

void DebugOut(wchar_t* fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);
