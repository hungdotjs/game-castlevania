#include "Weapon.h"



Weapon::Weapon()
{
	isFinish = 1;
}


Weapon::~Weapon()
{
}

void Weapon::Attack(float X, float Y, int Direction)
{
	this->x = X;
	this->y = Y;
	this->nx = Direction;
	isFinish = false; // chưa kết thúc


	LastTimeAttack = GetTickCount(); // lưu lại thời điểm lúc vừa tấn công, làm đánh dấu tránh 1 hit đánh nhiều lần cho các object, có health >1.
}

void Weapon::Render()
{
	if (isFinish)
		return;

	RenderBoundingBox();
}


bool Weapon::isCollision(CGameObject* obj)
{
	if (isFinish == true)
		return false;

	// dt, dx, dy đã update 
	if (obj->GetHealth() <= 0) // vật này die rồi thì ko va chạm
		return false;
	return isCollitionObjectWithObject(obj);
}



bool Weapon::GetFinish()
{
	return isFinish;
}

void Weapon::SetFinish(bool b)
{
	isFinish = b;
}

DWORD Weapon::GetLastTimeAttack()
{
	return LastTimeAttack;
}