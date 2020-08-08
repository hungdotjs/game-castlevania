#pragma once

#include "Torch.h"
#include "Candle.h"
#include "CheckStair.h"
#include "Simon.h"
#include "PhantomBat.h"
#include "Sound.h"

class Weapon : public CGameObject
{

public:
	float max_width;
	float max_x;
	Simon* simon;

	bool isExposed;
	bool isActivate;
	DWORD firstCast;
	Sound* sound;

	Weapon() {
		sound = Sound::GetInstance();
	}

	Weapon(Simon* simon, float max_width = SCREEN_WIDTH)
	{
		this->simon = simon;
		this->max_width = max_width;
		sound = Sound::GetInstance();

	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
};

