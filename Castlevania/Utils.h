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
#include <iostream>
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

// Simon

#define SIMON_WALKING_SPEED			0.05f 
#define SIMON_JUMP_SPEED_Y			0.28f
#define SIMON_CLIMBING_SPEED_X		0.05f
#define SIMON_CLIMBING_SPEED_Y		0.05f
#define SIMON_JUMP_DEFLECT_SPEED	0.2f
#define SIMON_JUMP_SPEED_X			0.08f
#define SIMON_HURT_DEFLECT_SPEED_Y	0.25f
#define SIMON_HURT_DEFLECT_SPEED_X	0.1f

#define SIMON_GRAVITY				0.002f
#define SIMON_GRAVITY_JUMP			0.001f
#define SIMON_DIE_DEFLECT_SPEED		0.5f
#define SIMON_SIT_TO_STAND			15.0f

#define SIMON_STATE_IDLE			10
#define SIMON_STATE_WALK			20
#define SIMON_STATE_JUMP			30
#define SIMON_STATE_SIT				40
#define SIMON_STATE_ONCHECKSTAIR	50
#define SIMON_STATE_ONSTAIR			60
#define SIMON_STATE_ONSTAIR_IDLE	70
#define SIMON_STATE_DIE				90
#define SIMON_STATE_HURT			100


// action
#define SIMON_ACTION_JUMP			3
#define SIMON_ACTION_ATTACK			6

// animation
#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1
#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3
#define SIMON_ANI_JUMP_RIGHT		4
#define SIMON_ANI_JUMP_LEFT			5
#define SIMON_ANI_SIT_RIGHT			6
#define SIMON_ANI_SIT_LEFT			7
#define SIMON_ANI_DIE_RIGHT			8
#define SIMON_ANI_DIE_LEFT			9
#define SIMON_ANI_SIT_ATTACK_RIGHT	10
#define SIMON_ANI_SIT_ATTACK_LEFT	11
#define SIMON_ANI_ATTACK_RIGHT		12
#define SIMON_ANI_ATTACK_LEFT		13
#define SIMON_ANI_STAIRUP_ATTACK_RIGHT		14
#define SIMON_ANI_STAIRUP_ATTACK_LEFT		15
#define SIMON_ANI_STAIRDOWN_ATTACK_RIGHT	16
#define SIMON_ANI_STAIRDOWN_ATTACK_LEFT		17
#define SIMON_ANI_WALKING_UP_RIGHT			18
#define SIMON_ANI_WALKING_UP_LEFT			19
#define SIMON_ANI_WALKING_DOWN_RIGHT		20
#define SIMON_ANI_WALKING_DOWN_LEFT			21
#define SIMON_ANI_IDLE_UP_RIGHT				22
#define SIMON_ANI_IDLE_UP_LEFT				23
#define SIMON_ANI_IDLE_DOWN_RIGHT			24
#define SIMON_ANI_IDLE_DOWN_LEFT			25
#define SIMON_ANI_HURT_RIGHT				26
#define SIMON_ANI_HURT_LEFT					27


#define SIMON_ON_LEFT_CHECKSTAIR		1
#define SIMON_ON_RIGHT_CHECKSTAIR		2

#define SIMON_LEVEL			1

#define SIMON_BBOX_WIDTH  16
#define SIMON_BBOX_HEIGHT 30
#define SIMON_SIT_BBOX_WIDTH			16
#define SIMON_SIT_BBOX_HEIGHT			23

#define SIMON_UNTOUCHABLE_TIME		2000
#define SIMON_TIMER_ATTACK			300


// Enemy
#define ENEMY_UNTOUCHABLE_TIME			300

// Enemy bullet
#define ENEMY_BULLET_BBOX_WIDTH			10
#define ENEMY_BULLET_BBOX_HEIGHT		10

#define ENEMY_BULLET_SPEED				0.2f

//Skeleton
#define SKELETON_BBOX_WIDTH				16
#define SKELETON_BBOX_HEIGHT			32
#define SKELETON_WALKING_SPEED			0.1
#define SKELETON_JUMPING_SPEED			0.2
#define SKELETON_ATTACK_ZONE			48
#define SKELETON_STATE_WALKING			0

#define SKELETON_ANI_WALKING_LEFT		0
#define SKELETON_ANI_WALKING_RIGHT		1

//bone
#define BONE_BBOX_WIDTH			15
#define BONE_BBOX_HEIGHT		15
#define BONE_GRAVITY 0.0007f
#define BONE_DAMAGE 1
#define BONE_SPEED_Y	0.25f
#define BONE_SPEED_X	0.1f
#define BONE_STATE_DIE 0
#define BONE_STATE_FLY 1

using namespace std;

void DebugOut(wchar_t* fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);
