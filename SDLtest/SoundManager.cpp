#include "SoundManager.h"

/** Raw values init **/
SoundManager* SoundManager::m_instance			= 0;
SoundManager::AudioState SoundManager::m_state	= ERROR;
std::string SoundManager::m_currentMusicFilename= "";

void SoundManager::playMusic(const string& fileName) {

	if (m_state != ERROR)
	{
		// If no music is playing, play it
		if (Mix_PlayingMusic() == 0)
		{
			// Load music
			try
			{
				m_music =
					Mix_LoadMUS(fileName.c_str());
				//Play music
				Mix_PlayMusic(m_music, -1);
				m_state = PLAYING;
				m_currentMusicFilename = fileName.c_str();
			}
			catch (const exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			// If music is playing, pause it
			this->pauseMusic();
		}
	}
}

void SoundManager::stopMusic() {
	if (m_state != ERROR)
	{
		Mix_HaltMusic();
		m_state = STOPPED;
		m_currentMusicFilename = "";
	}
}

void SoundManager::pauseMusic()
{
	if (m_state != ERROR)
	{
		// If music is playing, handle the pause request
		if (Mix_PlayingMusic() == 1)
		{
			if (Mix_PausedMusic() == 1)
			{
				// If the music is already paused, resume it with a rewind
				Mix_RewindMusic();
				Mix_PlayMusic(m_music, -1);
				m_state = PLAYING;
			}
			else
			{
				// Otherwise, pause the music
				Mix_PauseMusic();
				m_state = PAUSED;
			}
		}
	}
}

void SoundManager::resumeMusic() {
	if (m_state == PAUSED)
		Mix_ResumeMusic();
}
