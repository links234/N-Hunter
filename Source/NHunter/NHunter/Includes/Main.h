#ifndef _MAIN_H_
#define _MAIN_H_

#define CRTDBG_MAP_ALLOC
#include <strstream>
#include <windows.h>
#include <commdlg.h>
#include <tchar.h>
#include <stdio.h>
#include <fstream>
#include <conio.h>
#include <math.h>
#include <string>
#include <vector>

#include "..\\Includes\\CVideo.h"
#include "..\\Includes\\CSound.h"
#include "..\\Includes\\FatalError.h"
#include "..\\Includes\\Sprite.h"
#include "..\\Includes\\CTimer.h"
#include "..\\Includes\\ImageFile.h"
#include "..\\Includes\\Vec2.h"
#include "..\\Includes\\BackBuffer.h"
#include "..\\Includes\\RectCollide.h"
#include "..\\Includes\\CPlayer.h"
#include "..\\Includes\\CProjectile.h"
#include "..\\Includes\\Objects.h"
#include "..\\Includes\\Shake.h"
#include "..\\Includes\\Explosion.h"
#include "..\\Includes\\CheckPoint.h"
#include "..\\Includes\\Enemies.h"

#include "..\\Res\\resource.h"

#define EPS 1e-3 
#define PI 3.14159265358979323846
#define DEG2RAD(deg) (PI * (deg) / 180.0)
#define RAD2DEG(rad) ((rad) * 180.0 / PI)

#define GP_PLAYING 		 	0
#define GP_MAIN_MENU 	 	1
#define GP_STARTGAME_MENU	2
#define GP_OPTIONS_MENU		3
#define GP_CREDITS_MENU		4
#define GP_PLAY_ANIMATION   5
#define GP_GAMEOVER			6
#define GP_GAMEFINISHED		7
#define GP_LEVEL1_INTRO		8
#define GP_LEVEL2_INTRO		9
#define GP_LEVEL3_INTRO		10
#define GP_LEVEL4_INTRO		11
#define GP_LEVEL5_INTRO		12
#define GP_LEVEL2_DIALOG	13
#define GP_LEVEL2_DIALOG2	14
#define GP_FUNMODE			15

#define FADE_IN				0
#define FADE_OUT			1
#define WAIT_STATE			2
#define JUST_QUIT			3

#define BUTTON_DISABLED		0
#define BUTTON_NORMAL		1
#define BUTTON_HOVER		2
#define BUTTON_CLICKED		3

#define MAIN_MENU_STARTGAME	0
#define MAIN_MENU_OPTIONS	1
#define MAIN_MENU_CREDITS	2
#define MAIN_MENU_EXIT		3

#define STARTGAME_LEVEL1	0
#define STARTGAME_LEVEL2	1
#define STARTGAME_LEVEL3	2
#define STARTGAME_LEVEL4	3
#define STARTGAME_LEVEL5	4
#define STARTGAME_CONTINUE  5
#define STARTGAME_BACK		6

#define GAMEOVER_RESTART	0
#define GAMEOVER_MAINMENU	1

#define GAMEFIN_RESTART		0
#define GAMEFIN_MAINMENU	1
#define GAMEFIN_NEXTLEVEL	2

#define OPTION_LEFT			0
#define OPTION_RIGHT		1
#define OPTION_BACK			2

#define g_cGravity			2000.0f
#define	g_cMaxMVSpeed		70.0f
#define g_cMVAcc			20.0f

#define MAXCHEATS			32
#define CHEAT_NEVERHEAT		0
#define CHEAT_1HIT1KILL		1
#define CHEAT_ENERGY		2
#define CHEAT_FULLHP		3
#define CHEAT_GOD			4
#define CHEAT_SPEED			5
#define CHEAT_IMMORTAL		6
#define CHEAT_FULLEP		7
#define CHEAT_NOHEAT		8
#define CHEAT_STRENGTH		9
#define CHEAT_FIREBOY		10
#define CHEAT_PICKME		11
#define CHEAT_WATERGIRL		12
#define CHEAT_SLOWTIME		13
#define CHEAT_UBERBULLET	14
#define CHEAT_WALLSHOT		15
#define CHEAT_GENERIC		16

#define MAX_PARALLAX_PLANES 4

bool FileExists(std::string fileName);
void FileCheck(std::string fileName);

struct MouseButtons
{
	bool Left, Right, Middle;
};

struct Mouse
{
	MouseButtons Button;
	POINT Pos;
	bool IsUsed;
	bool Hide;
	float ElapsedTime;
	float TimeWait;
};

struct Button
{
	POINT Pos;
	int State;
	int Frame;
	RECT ZONE;
};

struct CreditLine
{
	int PosY;
	HFONT *pFont;
	std::string Line;
};
#endif