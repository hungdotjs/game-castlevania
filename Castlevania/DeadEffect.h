#pragma once
#include "Effect.h"

class DeadEffect : public Effect
{
public:
	void Render();
	void Render(float posX, float posY);
	void Update(DWORD dt);
	DeadEffect();
	~DeadEffect();
};

