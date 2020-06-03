#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

/*
Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	vector<LPANIMATION_FRAME> frames;
	bool isLoop;
public:
	CAnimation(int defaultTime = 100, bool isLoop = true)
	{
		this->defaultTime = defaultTime;
		lastFrameTime = -1;
		currentFrame = -1;
		this->isLoop = isLoop;
	}
	void Add(int spriteId, DWORD time = 0);
	int GetCurrentFrame() { return currentFrame; }
	vector<LPANIMATION_FRAME> getFrames() { return frames; }
	void SetFrames(vector<LPANIMATION_FRAME> frames) { this->frames = frames; }
	int GetDefaultTime() { return defaultTime; }
	bool IsLoop() { return isLoop; }
	void Reset() { currentFrame = -1; lastFrameTime = -1; }
	void Render(float x, float y, int alpha = 255);
};

typedef CAnimation* LPANIMATION;

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();
	void Clear(vector<int> listId);
	static CAnimations* GetInstance();
};

typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets* __instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);
	void Clear();
	void Clear(vector<int> listId);

	static CAnimationSets* GetInstance();
};