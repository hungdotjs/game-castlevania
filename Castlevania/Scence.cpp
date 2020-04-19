#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

CScene::~CScene()
{
	if (key_handler != NULL)
	{
		delete key_handler;
	}
}


CScenceKeyHandler::~CScenceKeyHandler()
{
	if (this->scence != NULL)
	{
		delete scence;
	}
}