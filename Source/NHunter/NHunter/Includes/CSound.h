#ifndef CSOUND_H
#define CSOUND_H

#include "Main.h"
#include <vector>
#include <string>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

const int SOUND_DEFAULT	= 1;
const int SOUND_MUSIC	= 2;
const int SOUND_SFX		= 3;

class CSound
{
public:
	CSound(std::string fileName, int soundType=SOUND_DEFAULT, bool loop=false);
	~CSound();

	void load(char *szFileName);
	void load(std::string szFileName);
	void play();
	void stop();
	void pause();
	void pause(bool pb);
	void unload();
	std::string getFileName();
	bool isPaused();
	bool isLooped();
	void setLoop(bool bLoop);
	bool isLeftMuted();
	bool isRightMuted();
	bool isAllMuted();
	int getTrebleVolume();
	int getBassVolume();
	int getLeftVolume();
	int getRightVolume();
	int getAllVolume();

	void setMuteRight(bool muteRight);
	void setMuteLeft(bool muteLeft);
	void setMuteAll(bool muteAll);
	void setLeftVolume(int value);
	void setRightVolume(int value);
	void setAllVolume(int value);
	void setTrebleVolume(int value);
	void setBassVolume(int value);
	void setBalance(int value);
	void seek(int miliseconds);
	int getSoundType();

	static void SFX_SetVolume(int value);
	static void SFX_SetRightVolume(int value);
	static void SFX_SetLeftVolume(int value);
	static void SFX_SetTrebleVolume(int value);
	static void SFX_SetBassVolume(int value);
	static void SFX_SetRightMute(bool value);
	static void SFX_SetLeftMute(bool value);
	static void SFX_SetMute(bool value);
	static void SFX_SetBalance(int value);
	static void SFX_DeleteAllMemory();

	static void Music_SetVolume(int value);
	static void Music_SetRightVolume(int value);
	static void Music_SetLeftVolume(int value);
	static void Music_SetTrebleVolume(int value);
	static void Music_SetBassVolume(int value);
	static void Music_SetRightMute(bool value);
	static void Music_SetLeftMute(bool value);
	static void Music_SetMute(bool value);
	static void Music_SetBalance(int value);
	static void Music_DeleteAllMemory();
	static void Music_PlayAll();
	static void Music_SetFocus(CSound* pObj);
	static void Music_PauseAll();
	static CSound* Music_GetFocus();
private:
	inline void load()
	{
		m_leftVolume=m_rightVolume=m_allVolume=m_trebleVolume=m_bassVolume=1000;
		std::string szCommand = "open " + getFileName() + " type mpegvideo ";		
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}

	static CSound* MusicFocus; 
	std::string m_szFileName;
	bool m_bPaused;
	bool m_bLoop;
	int m_leftVolume, m_rightVolume, m_allVolume, m_trebleVolume, m_bassVolume;
	bool m_bMuteLeft, m_bMuteRight, m_bMuteAll;
	int m_SoundType;

	static std::vector<CSound*> SFX_Sounds;
	static std::vector<CSound*> Music_Sounds;
};

#endif