#pragma once
#include "Textures.h"
#include "Sprites.h"
#include  "Animations.h"
class Effect
{
protected:
	int x, y;
	DWORD timeDelay;
	bool isEnable;
	vector<LPANIMATION> animations;
public:
	virtual void Render() {}
	virtual void Update(DWORD dt) {}
	void SetPosition(float X, float Y) { x = X, y = Y; }
	void SetEnable(bool e) { isEnable = e; }
	virtual void AddAnimation(int aniId);
	Effect();
	~Effect();
};

