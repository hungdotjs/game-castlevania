#include "Torch.h"

void Torch::Render()
{
	if (!isHitted)
		animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void Torch::SetHit()
{
	isHitted = true;
}

void Torch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TORCH_BBOX_WIDTH;
	b = y + TORCH_BBOX_HEIGHT;
}
