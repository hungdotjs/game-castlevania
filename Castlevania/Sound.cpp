
#include "Sound.h"
#include "Utils.h"

#define SECTION_UNKNOWN -1
#define SECTION_SOUND 0

#define MAX_LINE 1024

Sound* Sound::__Instance = NULL;

Sound::Sound()
{
}



void Sound::Load(LPCWSTR file)
{
	Init();

	ifstream f;
	f.open(file);

	int section = SECTION_UNKNOWN;
	char str[MAX_LINE];

	while (f.getline(str, MAX_LINE))
	{
		string line(str);

		if (line[0] == '#')
			continue; // skip comment lines
		if (line[0] == '/' && line[1] == '/')
			continue; // skip comment lines

		if (line == "[SOUND]")
		{
			section = SECTION_SOUND;
			continue;
		}

		switch (section)
		{
		case SECTION_SOUND:
			_ParseSection_SOUND(line);
			break;
		}
	}

}

void Sound::_ParseSection_SOUND(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2)
		return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	LPTSTR lpt = new TCHAR[1024];
	lpt = (LPTSTR)path.c_str();

	this->AddSound(id, lpt);
}

bool Sound::isPlaying(int type)
{
	return _ArrSound[type]->IsSoundPlaying();
}

void Sound::StopAll()
{
	for (auto& x : _ArrSound)
	{
		if (x.second->IsSoundPlaying())
			x.second->Stop();
	}
}


void Sound::AddSound(int type, LPTSTR filename)
{
	CSound* wave;
	HRESULT result = dsound->Create(&wave, filename);
	if (result != DS_OK)
	{
		return;
	}
	_ArrSound[type] = wave;
}

void Sound::Play(int type, bool isLoop, LONG lVolume)
{
	_ArrSound[type]->Play(0, isLoop, lVolume);
}

void Sound::Stop(int type)
{
	_ArrSound[type]->Stop();
}



void Sound::Init()
{
	dsound = new CSoundManager();
	dsound->Initialize(CGame::GetInstance()->GetHWND(), DSSCL_PRIORITY);
	dsound->SetPrimaryBufferFormat(2, 22050, 16);
}

Sound* Sound::GetInstance()
{
	if (__Instance == NULL)
		__Instance = new Sound();
	return __Instance;
}