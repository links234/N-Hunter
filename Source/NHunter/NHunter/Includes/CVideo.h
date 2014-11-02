#ifndef CVIDEO_H
#define CVIDEO_H

#include "Main.h"
#include <string>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

class CVideo
{
public:
	static CVideo* getInstance();
	void load(char *szFileName);
	void load(std::string szFileName);
	void play();
	void stop();
	void pause();
	void unload();
	std::string getFileName();
	bool isPaused();
	void setPaused(bool bPaused);
	bool isFullscreen();
	void setFullscreen(bool bFS);
	bool isSetToWait();
	void setWait(bool bWait);
	bool isSetToNotify();
	void setNotify(bool bNotify);
	void setNotifyHWND(HWND hWnd);

private:
	CVideo():m_bPaused(false),m_bFS(false),m_bWait(false),m_bNotify(false){}
	~CVideo(){unload();}
	CVideo(const CVideo&);
  	CVideo& operator=(const CVideo&);

	inline void load()
	{
		std::string szCommand = "open \"" + getFileName() + "\" type mpegvideo alias " + getFileName();		
		mciSendString(szCommand.c_str(), NULL, 0, 0);
	}

	HWND m_hWnd;
	std::string m_szFileName;
	bool m_bPaused;
	bool m_bFS;
	bool m_bWait;
	bool m_bNotify;
};

#endif