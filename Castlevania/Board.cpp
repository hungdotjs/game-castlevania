#include "Board.h"

Board::Board()
{
	simonHP = NULL;
	simonHP = NULL;
}


Board::~Board()
{

}

void Board::Initialize(LPDIRECT3DDEVICE9 d3ddv, Simon* simon)
{
	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(ID_TEX_MISC);

	this->heart = new CSprite(100000, 29, 6, 41, 16, tex);
	this->redRect = new CSprite(100001, 153, 74, 185, 96, tex);
	this->axe = new CSprite(100002, 18, 18, 33, 32, tex);
	this->holyWater = new CSprite(100003, 35, 18, 51, 34, tex);
	this->cross = new CSprite(100004, 69, 19, 84, 33, tex);
	this->knife = new CSprite(100005, 1, 22, 17, 31, tex);
	this->stopWatch = new CSprite(100006, 104, 18, 119, 34, tex);

	for (int i = 0; i < 16; i++)
	{
		CSprite* sprite = new CSprite(100007, 8, 84, 12, 92, tex);

		simonHPList.push_back(sprite);
	}

	for (int i = 0; i < 16; i++)
	{
		CSprite* sprite = new CSprite(100008, 18, 84, 22, 92, tex);
		enemyHPList.push_back(sprite);
	}

	font = NULL;
	AddFontResourceEx(GAME_FONT, FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		d3ddv,
		8,
		0,
		FW_NORMAL,
		1,
		false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		L"Press Start",
		&font);

	if (!SUCCEEDED(result))
		return;

	SetRect(&rect, 0, 8, SCREEN_WIDTH, SCREEN_HEIGHT);
	information = "SCORE 000000		TIME 0000		STAGE 00\n";
	information += "PLAYER							 -62\n";
	information += "ENEMY							P-3\n";
}

void Board::Update(int time, int stage, Simon* simon)
{
	this->time = time;
	this->stage = stage;


	std::string scoreString = std::to_string(simon->GetScore());
	while (scoreString.length() < 6)
		scoreString = "0" + scoreString;


	std::string timeString = std::to_string(this->time);
	while (timeString.length() < 4)
		timeString = "0" + timeString;


	std::string stageString = std::to_string(this->stage);
	while (stageString.length() < 2)
		stageString = "0" + stageString;

	information = "SCORE-" + scoreString + " TIME " + timeString + " STAGE " + stageString + "\n";
	information += "PLAYER                  -" + std::to_string(simon->GetHeart()) + "\n";
	information += "ENEMY                  P-" + std::to_string(simon->GetLife()) + "\n";
}

void Board::Render(float x, float y, Simon* simon)
{
	if (font)
		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	for (int i = 0; i < simonHPList.size(); i++)
	{
		simonHPList[i]->Draw(x + 50 + 5 * i, 46);
	}
	for (int i = 0; i < enemyHPList.size(); i++)
	{
		enemyHPList[i]->Draw(x + 50 + 5 * i, 56);
	}

	heart->Draw(x + SCREEN_WIDTH / 2 + 20, 46);
	redRect->Draw(x + SCREEN_WIDTH / 2 - 16, 46);
	if (simon->currentWeapon != NULL)
	{
		int weapon = simon->currentWeapon;
		{
			switch (weapon)
			{
			case ITEM_KNIFE:
				knife->Draw(x + SCREEN_WIDTH / 2 - 16, 50);
				break;
			}
		}
	}
}


