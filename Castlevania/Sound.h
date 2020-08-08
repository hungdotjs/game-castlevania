#pragma once

#include <iostream>
#include <fstream>
#include "Game.h"
#include "DSutil.h"
#include <unordered_map>

class Sound
{
private:
	unordered_map<int, CSound* > _ArrSound;
	void _ParseSection_SOUND(string line);
public:
	static Sound* __Instance;
	static Sound* GetInstance();
	CSoundManager* dsound;

	void Init();
	Sound();
	void AddSound(int type, LPTSTR filename);
	void Play(int type, bool isLoop = false, LONG lVolume = 0L);
	void Stop(int type);
	void Load(LPCWSTR simonFile);
	bool isPlaying(int type);
	void StopAll();

};