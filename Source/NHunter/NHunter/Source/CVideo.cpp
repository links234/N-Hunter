#include "..\\Includes\\CVideo.h"

CVideo* CVideo::getInstance()
{
    static CVideo singleton;
    return &singleton;
}

void CVideo::load(char *szFileName)
{
	m_szFileName = szFileName;
	load();
}

void CVideo::load(std::string szFileName)
{
	m_szFileName = szFileName;
	load();
}

void CVideo::play()
{
	FileCheck(getFileName());		

	std::string szCommand="play " + getFileName();
	if(isFullscreen())
		szCommand +=" fullscreen";
	szCommand += " from 0";
	if(m_bWait)
		szCommand += " wait";
		
	if(m_bNotify)
	{
		szCommand += " notify";
		mciSendString(szCommand.c_str(), NULL, 0, m_hWnd);
	}
	else
	{
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}
}

void CVideo::stop()
{		
	std::string szCommand = "stop " + getFileName();
	mciSendString(szCommand.c_str(), NULL, 0, 0);
}

void CVideo::pause()
{
	std::string szCommand;
	if(isPaused())
	{
		szCommand = "resume " + getFileName();
		mciSendString(szCommand.c_str(), NULL, 0, 0);
		setPaused(false);
	}
	else
	{
		szCommand = "pause " + getFileName();
		mciSendString(szCommand.c_str(), NULL, 0, 0);		
		setPaused(true);
	}
}

void CVideo::unload()
{
	std::string szCommand = "close" + getFileName();
	stop();
	mciSendString(szCommand.c_str(), NULL, 0, 0);
}

std::string CVideo::getFileName()
{
	return m_szFileName;
}

bool CVideo::isPaused()
{
	return m_bPaused;
}

void CVideo::setPaused(bool bPaused)
{
	m_bPaused = bPaused;
}

bool CVideo::isFullscreen()
{
	return m_bFS;
}

void CVideo::setFullscreen(bool bFS)
{
	m_bFS = bFS;
}

bool CVideo::isSetToWait()
{
	return m_bWait;
}

void CVideo::setWait(bool bWait)
{
	m_bWait = bWait; 
	if(bWait)m_bNotify=false;
}

bool CVideo::isSetToNotify()
{
	return m_bNotify;
}

void CVideo::setNotify(bool bNotify)
{
	m_bNotify = bNotify;
	if(bNotify)m_bWait=false;
}

void CVideo::setNotifyHWND(HWND hWnd)
{
	m_hWnd=hWnd;
}