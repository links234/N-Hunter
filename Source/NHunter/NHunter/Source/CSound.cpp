#include "..\\Includes\\CSound.h"

std::vector<CSound*> CSound::SFX_Sounds;
std::vector<CSound*> CSound::Music_Sounds;
CSound* CSound::MusicFocus;
extern HWND		g_hWnd;

std::string ConvertIntToString(int value);

CSound::CSound(std::string fileName, int soundType, bool loop)
{
	FileCheck(fileName);

	m_SoundType = soundType;
	m_bPaused = false;
	if(soundType==SOUND_SFX)
	{
		SFX_Sounds.push_back(this);
	}
	else if(soundType==SOUND_MUSIC)
	{
		Music_Sounds.push_back(this);
	}
	m_bLoop=loop;
	load(fileName);
}	

CSound::~CSound(){unload();}

void CSound::load(char *szFileName)
{	
	m_szFileName = szFileName;
	load();
}

void CSound::load(std::string szFileName)
{
	m_szFileName = szFileName;
	load();
}

void CSound::play()
{
	std::string szCommand;
	szCommand = "seek " + getFileName() + " to start";
	mciSendString(szCommand.c_str(), NULL, 0, 0);
	if(m_bLoop)
	{
		szCommand = "play " + getFileName() + " repeat";
	}
	else
	{
		szCommand = "play " + getFileName();
	}	
	mciSendString(szCommand.c_str(), NULL, 0, g_hWnd);
}

void CSound::stop()
{
	std::string szCommand = "stop " + getFileName();
	mciSendString(szCommand.c_str(), NULL, 0, 0);
}

void CSound::pause()
{
	std::string szCommand;
	if(m_bPaused)
	{
		szCommand = "resume " + getFileName();
		mciSendString(szCommand.c_str(), NULL, 0, 0);
		m_bPaused=false;
	}
	else
	{
		szCommand = "pause " + getFileName();
		mciSendString(szCommand.c_str(), NULL, 0, 0);		
		m_bPaused=true;
	}
}

void CSound::pause(bool pb)
{
	if(pb!=m_bPaused)
		pause();
}

void CSound::unload()
{
	std::string szCommand = "close" + getFileName();
	stop();
	mciSendString(szCommand.c_str(), NULL, 0, 0);
}

std::string CSound::getFileName()
{
	return m_szFileName;
}

bool CSound::isPaused()
{
	return m_bPaused;
}

bool CSound::isLooped()
{
	return m_bLoop;
}

void CSound::setLoop(bool bLoop)
{
	m_bLoop=bLoop;
}

bool CSound::isLeftMuted()
{
	return m_bMuteLeft;
}

bool CSound::isRightMuted()
{
	return m_bMuteRight;
}

bool CSound::isAllMuted()
{
	return m_bMuteAll;
}

int CSound::getTrebleVolume()
{
	return m_trebleVolume;
}

int CSound::getBassVolume()
{
	return m_bassVolume;
}

int CSound::getLeftVolume()
{
	return m_leftVolume;
}

int CSound::getRightVolume()
{
	return m_rightVolume;
}

int CSound::getAllVolume()
{
	return m_allVolume;
}

void CSound::setMuteRight(bool muteRight)
{
	m_bMuteRight=muteRight;
	std::string szCommand;
	if(muteRight)
	{
		szCommand = "setaudio " + getFileName() + " right off";
		mciSendString(szCommand.c_str(), NULL, 0, 0);	
	}
	else
	{
		szCommand = "setaudio " + getFileName() + " right on";
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}

void CSound::setMuteLeft(bool muteLeft)
{
	m_bMuteLeft=muteLeft;
	std::string szCommand;
	if(muteLeft)
	{
		szCommand = "setaudio " + getFileName() + " left off";
		mciSendString(szCommand.c_str(), NULL, 0, 0);	
	}
	else
	{
		szCommand = "setaudio " + getFileName() + " left on";
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}
	
void CSound::setMuteAll(bool muteAll)
{	
	m_bMuteAll=muteAll;
	m_bMuteRight=muteAll;
	m_bMuteLeft=muteAll;
	std::string szCommand;
	if(muteAll)
	{
		szCommand = "setaudio " + getFileName() + " off";
		mciSendString(szCommand.c_str(), NULL, 0, 0);	
	}
	else
	{
		szCommand = "setaudio " + getFileName() + " on";
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}

void CSound::setLeftVolume(int value)
{
	if(value>=0 && value<=1000)
	{
		m_leftVolume=value;
		std::string szCommand="setaudio " + getFileName() + " left volume to " + ConvertIntToString(value);
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}

void CSound::setRightVolume(int value)
{
	if(value>=0 && value<=1000)
	{
		m_rightVolume=value;
		std::string szCommand="setaudio " + getFileName() + " right volume to " + ConvertIntToString(value);
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}
	
void CSound::setAllVolume(int value)
{
	if(value>=0 && value<=1000)
	{
		m_rightVolume=value;
		m_leftVolume=value;
		m_allVolume=value;
		std::string szCommand="setaudio " + getFileName() + " volume to " + ConvertIntToString(value);
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}
	
void CSound::setTrebleVolume(int value)
{
	if(value>=0 && value<=1000)
	{
		m_trebleVolume=value;
		std::string szCommand="setaudio " + getFileName() + " treble to " + ConvertIntToString(value);
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}
	
void CSound::setBassVolume(int value)
{
	if(value>=0 && value<=1000)
	{
		m_trebleVolume=value;
		std::string szCommand="setaudio " + getFileName() + " bass to " + ConvertIntToString(value);
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}

void CSound::setBalance(int value)
{	
	if(value>=-1000 && value<=1000)
	{
		if(value<0)
		{
			setLeftVolume(1000);
			setRightVolume(1000+value);
		}
		else
		{
			setLeftVolume(1000-value);
			setRightVolume(1000);
		}		
	}
}

void CSound::seek(int miliseconds)
{
	std::string szCommand="seek " + getFileName() + " to " + ConvertIntToString(miliseconds);
	mciSendString(szCommand.c_str(), NULL, 0, 0);
	if(!m_bPaused)
	{
		play();
	}
}

int CSound::getSoundType()
{
	return m_SoundType;
}

void CSound::SFX_SetVolume(int value)
{		
	for(register int i=0;i<(int)SFX_Sounds.size();++i)		
	{
		SFX_Sounds[i]->setAllVolume(value);
	}
}

void CSound::SFX_SetRightVolume(int value)
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		SFX_Sounds[i]->setRightVolume(value);	
	}
}

void CSound::SFX_SetLeftVolume(int value)
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		SFX_Sounds[i]->setLeftVolume(value);
	}
}

void CSound::SFX_SetTrebleVolume(int value)
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		SFX_Sounds[i]->setTrebleVolume(value);
	}
}
	
void CSound::SFX_SetBassVolume(int value)
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		SFX_Sounds[i]->setBassVolume(value);
	}
}
	
void CSound::SFX_SetRightMute(bool value)
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		SFX_Sounds[i]->setMuteRight(value);
	}
}

void CSound::SFX_SetLeftMute(bool value)
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		SFX_Sounds[i]->setMuteLeft(value);
	}
}

void CSound::SFX_SetMute(bool value)
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		SFX_Sounds[i]->setMuteAll(value);
	}
}

void CSound::SFX_SetBalance(int value)
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		SFX_Sounds[i]->setBalance(value);
	}
}

void CSound::SFX_DeleteAllMemory()
{
	for(register int i=0;i<(int)SFX_Sounds.size();++i)
	{
		delete SFX_Sounds[i];
		SFX_Sounds.erase(SFX_Sounds.begin()+i);
		--i;
	}
}

void CSound::Music_SetVolume(int value)
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setAllVolume(value);
	}
	CSound::SFX_SetVolume(value);
}

void CSound::Music_SetRightVolume(int value)
{	
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setRightVolume(value);
	}
}

void CSound::Music_SetLeftVolume(int value)
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setLeftVolume(value);
	}
}

void CSound::Music_SetTrebleVolume(int value)
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setTrebleVolume(value);
	}
}

void CSound::Music_SetBassVolume(int value)
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setBassVolume(value);
	}
}

void CSound::Music_SetRightMute(bool value)
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setMuteRight(value);
	}
}

void CSound::Music_SetLeftMute(bool value)
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setMuteLeft(value);
	}
}

void CSound::Music_SetMute(bool value)
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setMuteAll(value);
	}
}

void CSound::Music_SetBalance(int value)
{	
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setBalance(value);
	}
}

void CSound::Music_DeleteAllMemory()
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		delete Music_Sounds[i];
		Music_Sounds.erase(Music_Sounds.begin()+i);
		--i;
	}
}

void CSound::Music_PlayAll()
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->setMuteAll(true);
		Music_Sounds[i]->play();
	}
}

void CSound::Music_SetFocus(CSound* pObj)
{
	MusicFocus=pObj;
	Music_PauseAll();
	pObj->pause(false);
}
	
void CSound::Music_PauseAll()
{
	for(register int i=0;i<(int)Music_Sounds.size();++i)
	{
		Music_Sounds[i]->pause(true);
	}
}

CSound* CSound::Music_GetFocus()
{
	return MusicFocus;
}

std::string ConvertIntToString(int value)
{
	std::string str="";
	int temp=value;
	while(value)
	{
		str=(std::string("")+(char)(value%10+'0'))+str;
		value/=10;
	}
	if(temp<0)
	{
		str=std::string("-")+str;
	}
	return str;
}