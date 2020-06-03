#include "DeadEffect.h"

DeadEffect::DeadEffect()
{
	AddAnimation(500);
	timeDelay = 0.0f;
	isEnable = false;
}

DeadEffect::~DeadEffect()
{
}

void DeadEffect::Update(DWORD dt)
{
	if (isEnable)
	{
		timeDelay += dt;
		if (timeDelay > 450)
		{
			timeDelay = 0.0f;
			isEnable = false;
		}
	}
}

void DeadEffect::Render()
{
	if (isEnable)
		animations[0]->Render(x, y);
}

void DeadEffect::Render(float posX, float posY)
{
	if (isEnable)
		animations[0]->Render(posX, posY);
}