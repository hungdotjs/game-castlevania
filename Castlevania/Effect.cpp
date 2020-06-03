#include "Effect.h"

void Effect::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);

	//LPANIMATION temp = new CAnimation(ani->GetDefaultTime(), ani->IsLoop());
	//temp->SetFrames(ani->getFrames());
	animations.push_back(ani);
}

Effect::Effect()
{
}

Effect::~Effect()
{
}
