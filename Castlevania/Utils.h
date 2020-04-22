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
#define SCREEN_HEIGHT 240

#define ID_TEX_TILESET 100

#define MAX_FRAME_RATE 120

using namespace std;

void DebugOut(wchar_t* fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);
