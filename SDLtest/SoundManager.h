#pragma once
#ifndef _SOUNDMANAGER_H
#define	_SOUNDMANAGER_H
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <SDL2\SDL.h>
#include <SDL2\SDL_audio.h>
#include <SDL2\SDL_mixer.h>



using namespace std;

class SoundManager
{

public:
	static SoundManager* Instance() {
		if (m_instance == 0) m_instance = new SoundManager();

		return m_instance;
	}
	void playMusic(const string& fileName);
	void stopMusic();
	void pauseMusic();
	void resumeMusic();

	void setVolume(int value);
	void changeVolume(int value);


	bool isStopped() const
	{
		return m_state == STOPPED;
	}
	bool isPaused() const
	{
		return m_state == PAUSED;
	}
	bool isPlaying() const
	{
		return m_state == PLAYING;
	}
	bool inErrorState() const
	{
		return m_state == ERROR;
	}

private:
	SoundManager() {
		initAudioDevice();
		setVolume(40);
	}
	~SoundManager() {
		if (m_music != nullptr)
			Mix_FreeMusic(m_music);

		Mix_CloseAudio();
	}

	void initAudioDevice() {

		if (SDL_Init(SDL_INIT_AUDIO) != -1)
		{
			if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
			{
				cout << "Error initializing audio device...\n" << endl;
				m_state = ERROR;
			}
			else
			{
				m_state = WAITING;
			}
		}
		else
		{
			cout << "Error initializing SDL audio subsystem...\n" << endl;
			m_state = ERROR;
		}
	}

private :

	enum AudioState
	{
		ERROR = 0,
		WAITING,
		STOPPED,
		PAUSED,
		PLAYING
	};

	static SoundManager* m_instance;
	static string m_currentMusicFilename;
	static AudioState m_state;

	Mix_Music* m_music;
	int m_volume;
};

#endif
