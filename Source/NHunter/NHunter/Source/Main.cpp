#include "..\\Includes\\Main.h"

extern const int FOOTSTATE_LEFTSTOP 			= 1;
extern const int FOOTSTATE_LEFTWALK 			= 2;
extern const int FOOTSTATE_RIGHTSTOP 			= 3;
extern const int FOOTSTATE_RIGHTWALK 			= 4;
extern const int FOOTSTATE_JUMPINGRIGHT	   		= 5;
extern const int FOOTSTATE_JUMPINGLEFT			= 6;
extern const int FOOTSTATE_REVERSELEFT			= 7;
extern const int FOOTSTATE_REVERSERIGHT			= 8;
extern const int FOOTSTATE_REVERSELEFTJUMPING 	= 9;
extern const int FOOTSTATE_REVERSERIGHTJUMPING	= 10;
extern float g_cSpeedModifier;
extern float SPEED_MOVE_DELAY;
extern float NORMAL_MOVE_DELAY;

double		g_ParallaxPlanesDist[MAX_PARALLAX_PLANES];
double		g_ParallaxBackgroundDist;
float 		g_fMusicVolumeSpeed;
float		g_fMusicVolume;
float 		g_fAnimDelay;
float		introFrameDelay;
float		g_TTimer;
float		g_TTimerLimit;
float		g_TTimerLimit2;
float		g_TTimerLimit3;
int 		g_iMusicVolume;
int 		gamePhase = GP_MAIN_MENU;
int			introPhase;
int			introFrame;
int			currLevel=0;
int			g_iNumberOfPlanes;
HINSTANCE	g_hInst;
Mouse		g_Mouse;
bool 		g_bMinimized = false;
bool		g_bMovieDone = false;
bool 		g_bIsFirstMenuMusic = true;
bool 		g_bGameFinished = false;
bool		g_bUnlockedLevels[5];
bool		g_bLevelIsParallax = false;
bool		g_bCheats[MAXCHEATS];
bool		g_bCheaterFirstColor = true;
bool		g_bSoundMute=false;
bool		g_bFullScreen=true;
bool		g_bCanCheat=true;
bool		madeShake=false;
bool		firstSave=false;
bool		secondSave=false;
bool		bFunModeGotAnim=false;
bool		bFunModeStartAnim=true;
bool		bFunModePlayer[8];
UINT		g_nViewWidth, g_nViewHeight, g_nViewX, g_nViewY;
CTimer		g_Timer;
HWND		g_hWnd;
HDC			g_hdcSprite;
DEVMODEA	g_DefaultDisplay;
HFONT		g_hFontLevel2Dialog, g_hFontTimer, g_hFontCheater, g_hFont, g_hFontTitle, g_hFontNormal, g_hFontHP;
Vec2 		g_PointOfView;
std::string g_cheatKeyBuffer;
CPlayer		*g_Player,*g_Player2;
CVideo		*g_VideoPlayer = CVideo::getInstance();
BackBuffer	*g_BBuffer;
std::string	INTRO1TEXT;
int			INTRO1COLOR;
int			INTROTEXTOFFSET=0;
int			INTROY;
int			INTROX;

int			soundCount=0;
int			funmode_Player1Score;
int			funmode_Player2Score;
int			deathPlayer=0;
int			funmode_StartAnimFrame=0;

std::vector<CImageFile*>		g_pBackground;
std::vector<RECT> 				g_CRect;
std::vector<Stuff*>         	g_Stuff;
std::vector<MovableObjects*> 	g_Box;
std::vector<CreditLine*>		g_CreditsData;
std::vector<CImageFile*>		g_ParallaxBackground;
std::vector<Sprite*>			g_ParallaxPlanes[MAX_PARALLAX_PLANES];

CImageFile 		RAW_BACKGROUND_LV1BK1, RAW_BACKGROUND_LV1BK2, RAW_BACKGROUND_LV1BK3, RAW_BACKGROUND_LV1BK4, RAW_BACKGROUND_LV1BK5, RAW_BACKGROUND_LV1BK6, RAW_BACKGROUND_LV1BK7, RAW_BACKGROUND_LV1BK8, RAW_BACKGROUND_LV1BK9, RAW_BACKGROUND_LV1BK10, RAW_BACKGROUND_LV1BK11, RAW_BACKGROUND_LV1BK12, RAW_BACKGROUND_LV1BK13, RAW_BACKGROUND_LV1BK14, RAW_MAINMENUBK, RAW_MAINMENUNLBK, RAW_LOADING_SCREEN, RAW_OPTMENUBK;
CImageFile		RAW_BACKGROUND_LV3BK1, RAW_BACKGROUND_LV3BK2, RAW_BACKGROUND_LV3BK3, RAW_BACKGROUND_LV3BK4, RAW_BACKGROUND_LV3BK5, RAW_BACKGROUND_LV3BK6, RAW_BACKGROUND_LV3BK7, RAW_BACKGROUND_LV3BK8, RAW_BACKGROUND_LV3BK9, RAW_BACKGROUND_LV3BK10, RAW_BACKGROUND_LV3BK11, RAW_BACKGROUND_LV3BK12, RAW_BACKGROUND_LV3BK13, RAW_BACKGROUND_LV3BK14, RAW_BACKGROUND_LV3BK15, RAW_BACKGROUND_LV3BK16, RAW_BACKGROUND_LV3BK17, RAW_BACKGROUND_LV3BK18;    
CImageFile		RAW_FUNMODEBK, RAW_GFBK, RAW_GOBK, RAW_BACKGROUND_LV2BK1, RAW_BACKGROUND_LV2BK2, RAW_BACKGROUND_LV5BK1, RAW_BACKGROUND_LV5BK2;
AnimatedSprite  *RAW_PLAYER2_MOVEMENT_ANIMATION_RIFLE, *RAW_PLAYER_MOVEMENT_ANIMATION_RIFLE, *RAW_BOX1_ANIMATION, *RAW_BOX2_ANIMATION, *RAW_BOX3_ANIMATION, *RAW_FLOOR1_ANIMATION, *RAW_SWITCH1_ANIMATION, *RAW_DOOR1_ANIMATION, *RAW_SPIKE1_ANIMATION;
AnimatedSprite 	*RAW_TURRET1_ANIMATION, *RAW_DUMMY1L_ANIMATION, *RAW_DUMMY1R_ANIMATION, *RAW_PLATFORM1_ANIMATION, *RAW_LSTRENGTH_PWUP_ANIMATION, *RAW_RSTRENGTH_PWUP_ANIMATION, *RAW_LSPEED_PWUP_ANIMATION, *RAW_RSPEED_PWUP_ANIMATION;
AnimatedSprite  *RAW_BOSSHP, *RAW_LARMOR_PWUP_ANIMATION, *RAW_RARMOR_PWUP_ANIMATION, *RAW_MAIN_MENU_BTNS, *RAW_BCBTNS, *RAW_LVLBTNS, *RAW_OPTBTNS;
AnimatedSprite	*RAW_PLAYER_MOVEMENT_ANIMATION_BAZOOKA, *RAW_PLAYER_MOVEMENT_ANIMATION_PISTOL, *RAW_PLAYER_MOVEMENT_ANIMATION_SHOTGUN, *RAW_PHPOH2, *RAW_PHPROB2, *RAW_EXP1, *RAW_EXP2, *RAW_TURRET1, *RAW_GOFBTNS, *RAW_PHPEB, *RAW_PHPOH, *RAW_PHPPW, *RAW_PHPROB, *RAW_PHPWEP, *anim, *RAW_PDAL_ANIMATION, *RAW_PDAR_ANIMATION, *RAW_PDAL2_ANIMATION, *RAW_PDAR2_ANIMATION;
Sprite			*RAW_REPAIRKIT, *RAW_PHPBK, *RAW_PHPBK2, *RAW_W1_PRJ, *RAW_W2_PRJ, *RAW_CURSOR, *RAW_SBK;
Sprite			*RAW_PARALLAX_L2P2BK1, *RAW_PARALLAX_L2P2BK2, *RAW_PARALLAX_L2P2BK3, *RAW_PARALLAX_L2P2BK4;
Sprite			*RAW_PARALLAX_L2P3BK1, *RAW_PARALLAX_L2P3BK2, *RAW_W3_PRJ;
CSound			*RAW_FUNMODEMUSIC, *RAW_PLAYER_DEATH, *RAW_CHANGE_POWER, *RAW_MENUMUSIC, *RAW_MENU2MUSIC, *RAW_LEVEL1MUSIC, *RAW_LEVEL2MUSIC, *RAW_LEVEL3MUSIC, *RAW_LEVEL4MUSIC, *RAW_LEVEL5MUSIC, *RAW_CREDITSMUSIC, *RAW_ACTION1MUSIC, *RAW_ACTION2MUSIC;
CSound			*RAW_DIGITAL_TYPING, *RAW_BALLSOFSTEEL, *RAW_GAMEFINISHEDMUSIC, *RAW_GAMEOVERMUSIC, *Playing_Paused_Music;
CSound			*RAW_FUNMODE1, *RAW_FUNMODE2, *RAW_FUNMODE3, *RAW_FUNMODE4, *RAW_FUNMODE5, *RAW_FUNMODE6, *RAW_FUNMODE7, *RAW_FUNMODE8, *RAW_FUNMODE9, *RAW_FUNMODE10, *RAW_FUNMODE11, *RAW_FUNMODE12; 

Button			MainMenuButtons[4], StartGameMenuButtons[7], OptionsMenuButtons[3], GameOverButtons[3], GameFinishedButtons[3];

LRESULT CALLBACK WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
void InitDisplay();
void LoadResources();
void NextFrame();
void GameLogic(float delta);
void GameRender();
void ReleaseMemory();
void SetupGame();
void CheckBorderCollision(float delta);
void ProcessInput(float delta);
void PlayIntro();
void LoadRectangleData(std::string filename);
void UpdateLevelButtonState();
void SaveGame();
void checkForCheat();
void LoadLevel1();
void LoadLevel2();
void LoadLevel3();
void LoadLevel4();
void LoadLevel5();
void LoadFunMode();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int iCmdShow )
{
	try
	{
		srand((unsigned int)time(NULL));

		g_hInst = hInstance;

		InitDisplay();
		PlayIntro();
		LoadResources();
		SetupGame();

		MSG		msg;
		while(true) 
	    {
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	        {
				if(msg.message == WM_QUIT)
				{
					if(g_bFullScreen)
					{
						ChangeDisplaySettingsEx(NULL,&g_DefaultDisplay,NULL,NULL,NULL);
					}
					SaveGame();
					CSound::Music_GetFocus()->pause(true);
					AnimateWindow(g_hWnd,2000,AW_BLEND|AW_HIDE|AW_CENTER);
					break;
				}
				TranslateMessage( &msg );
				DispatchMessage ( &msg );
			}
			else
			{
				NextFrame();
			}
	    }

		ReleaseMemory();
	}
	catch (FatalError err)
	{
		MessageBox(NULL,err.getMsg().c_str(),"Fatal error!",MB_OK|MB_ICONERROR);
	}
    return 0;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	switch (Message)
    {
		case MM_MCINOTIFY:
			g_bMovieDone=true;
			break;

		case WM_CREATE:
            break;
		
        case WM_CLOSE:
			PostQuitMessage(0);
			break;
		
        case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
        case WM_SIZE:
            if ( wParam == SIZE_MINIMIZED )
            {
                g_bMinimized = true;
            }
            else
            {
                g_bMinimized = false;
                g_nViewWidth  = 1024;
                g_nViewHeight = 768;
				delete g_BBuffer;
				g_BBuffer=new BackBuffer(g_hWnd,g_nViewWidth,g_nViewHeight);
            }
			break;

		case WM_MOUSEMOVE:
			if(g_bFullScreen)
			{
				GetCursorPos( &g_Mouse.Pos );
			}
			else
			{
				POINT displacement;
				displacement.x=displacement.y=0;
				ClientToScreen(g_hWnd,&displacement);
				GetCursorPos( &g_Mouse.Pos ); 
				g_Mouse.Pos.x-=displacement.x;
				g_Mouse.Pos.y-=displacement.y;
			}
			g_Mouse.IsUsed=true;
			g_Mouse.ElapsedTime=0.0f;

			if(gamePhase==GP_MAIN_MENU)
			{
				for(register int i=0;i<4;++i)
				{
					if(PointInRectangle(MainMenuButtons[i].ZONE,g_Mouse.Pos) && MainMenuButtons[i].State!=BUTTON_CLICKED)
					{
						RAW_MAIN_MENU_BTNS->SetFrame(MainMenuButtons[i].Frame);
						COLORREF col=RAW_MAIN_MENU_BTNS->GetMaskPixel(g_Mouse.Pos.x-MainMenuButtons[i].Pos.x,g_Mouse.Pos.y-MainMenuButtons[i].Pos.y);
						if(col==0)
						{
							MainMenuButtons[i].State=BUTTON_HOVER;
						}
						else
						{
							MainMenuButtons[i].State=BUTTON_NORMAL;
						}
					}
					else if(MainMenuButtons[i].State!=BUTTON_CLICKED)
					{
						MainMenuButtons[i].State=BUTTON_NORMAL;
					}
				}
			}
			else if(gamePhase==GP_STARTGAME_MENU)
			{
				for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_LEVEL5;++i)
				{
					if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
					{
						if(PointInRectangle(StartGameMenuButtons[i].ZONE,g_Mouse.Pos) && StartGameMenuButtons[i].State!=BUTTON_CLICKED)
						{
							if(i==STARTGAME_LEVEL5)
							{
								RAW_LVLBTNS->SetFrame(MainMenuButtons[i].Frame-5);
							}
							COLORREF col=RAW_LVLBTNS->GetMaskPixel(g_Mouse.Pos.x-StartGameMenuButtons[i].Pos.x,g_Mouse.Pos.y-StartGameMenuButtons[i].Pos.y);
							if(col==0)
							{
								StartGameMenuButtons[i].State=BUTTON_HOVER;
							}
							else
							{
								StartGameMenuButtons[i].State=BUTTON_NORMAL;
							}
						}
						else if(StartGameMenuButtons[i].State!=BUTTON_CLICKED)
						{
							StartGameMenuButtons[i].State=BUTTON_NORMAL;
						}
					}
				}

				for(register int i=STARTGAME_CONTINUE;i<=STARTGAME_BACK;++i)
				{
					if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
					{
						if(PointInRectangle(StartGameMenuButtons[i].ZONE,g_Mouse.Pos) && StartGameMenuButtons[i].State!=BUTTON_CLICKED)
						{
							RAW_BCBTNS->SetFrame(StartGameMenuButtons[i].Frame);
							COLORREF col=RAW_BCBTNS->GetMaskPixel(g_Mouse.Pos.x-StartGameMenuButtons[i].Pos.x,g_Mouse.Pos.y-StartGameMenuButtons[i].Pos.y);
							if(col==0)
							{
								StartGameMenuButtons[i].State=BUTTON_HOVER;
							}
							else
							{
								StartGameMenuButtons[i].State=BUTTON_NORMAL;
							}
						}
						else if(StartGameMenuButtons[i].State!=BUTTON_CLICKED)
						{
							StartGameMenuButtons[i].State=BUTTON_NORMAL;
						}
					}
				}
			}
			else if(gamePhase==GP_OPTIONS_MENU)
			{
				if(PointInRectangle(OptionsMenuButtons[OPTION_LEFT].ZONE,g_Mouse.Pos) && OptionsMenuButtons[OPTION_LEFT].State!=BUTTON_CLICKED)
				{
					RAW_OPTBTNS->SetFrame(OptionsMenuButtons[OPTION_LEFT].Frame);
					COLORREF col=RAW_OPTBTNS->GetMaskPixel(g_Mouse.Pos.x-OptionsMenuButtons[OPTION_LEFT].Pos.x,g_Mouse.Pos.y-OptionsMenuButtons[OPTION_LEFT].Pos.y);
					if(col==0)
					{
						OptionsMenuButtons[OPTION_LEFT].State=BUTTON_HOVER;
					}
					else
					{
						OptionsMenuButtons[OPTION_LEFT].State=BUTTON_NORMAL;
					}
				}
				else if(OptionsMenuButtons[OPTION_LEFT].State!=BUTTON_CLICKED)
				{
					OptionsMenuButtons[OPTION_LEFT].State=BUTTON_NORMAL;
				}

				if(PointInRectangle(OptionsMenuButtons[OPTION_RIGHT].ZONE,g_Mouse.Pos) && OptionsMenuButtons[OPTION_RIGHT].State!=BUTTON_CLICKED)
				{
					RAW_OPTBTNS->SetFrame(OptionsMenuButtons[OPTION_RIGHT].Frame);
					COLORREF col=RAW_OPTBTNS->GetMaskPixel(g_Mouse.Pos.x-OptionsMenuButtons[OPTION_RIGHT].Pos.x,g_Mouse.Pos.y-OptionsMenuButtons[OPTION_RIGHT].Pos.y);
					if(col==0)
					{
						OptionsMenuButtons[OPTION_RIGHT].State=BUTTON_HOVER;
					}
					else
					{
						OptionsMenuButtons[OPTION_RIGHT].State=BUTTON_NORMAL;
					}
				}
				else if(OptionsMenuButtons[OPTION_RIGHT].State!=BUTTON_CLICKED)
				{
					OptionsMenuButtons[OPTION_RIGHT].State=BUTTON_NORMAL;
				}

				if(PointInRectangle(OptionsMenuButtons[OPTION_BACK].ZONE,g_Mouse.Pos) && OptionsMenuButtons[OPTION_BACK].State!=BUTTON_CLICKED)
				{
					RAW_BCBTNS->SetFrame(OptionsMenuButtons[OPTION_BACK].Frame);
					COLORREF col=RAW_BCBTNS->GetMaskPixel(g_Mouse.Pos.x-OptionsMenuButtons[OPTION_BACK].Pos.x,g_Mouse.Pos.y-OptionsMenuButtons[OPTION_BACK].Pos.y);
					if(col==0)
					{
						OptionsMenuButtons[OPTION_BACK].State=BUTTON_HOVER;
					}
					else
					{
						OptionsMenuButtons[OPTION_BACK].State=BUTTON_NORMAL;
					}
				}
				else if(OptionsMenuButtons[OPTION_BACK].State!=BUTTON_CLICKED)
				{
					OptionsMenuButtons[OPTION_BACK].State=BUTTON_NORMAL;
				}
			}
			else if(gamePhase==GP_GAMEOVER)
			{
				for(register int i=0;i<2;++i)
				{
					if(PointInRectangle(GameOverButtons[i].ZONE,g_Mouse.Pos) && GameOverButtons[i].State!=BUTTON_CLICKED)
					{
						RAW_GOFBTNS->SetFrame(GameOverButtons[i].Frame);
						COLORREF col=RAW_GOFBTNS->GetMaskPixel(g_Mouse.Pos.x-GameOverButtons[i].Pos.x,g_Mouse.Pos.y-GameOverButtons[i].Pos.y);
						if(col==0)
						{
							GameOverButtons[i].State=BUTTON_HOVER;
						}
						else
						{
							GameOverButtons[i].State=BUTTON_NORMAL;
						}
					}
					else if(GameOverButtons[i].State!=BUTTON_CLICKED)
					{
						GameOverButtons[i].State=BUTTON_NORMAL;
					}
				}
			}
			else if(gamePhase==GP_GAMEFINISHED)
			{
				for(register int i=0;i<3;++i)
				{
					if(PointInRectangle(GameFinishedButtons[i].ZONE,g_Mouse.Pos) && GameFinishedButtons[i].State!=BUTTON_CLICKED)
					{
						RAW_GOFBTNS->SetFrame(GameFinishedButtons[i].Frame);
						COLORREF col=RAW_GOFBTNS->GetMaskPixel(g_Mouse.Pos.x-GameFinishedButtons[i].Pos.x,g_Mouse.Pos.y-GameFinishedButtons[i].Pos.y);
						if(col==0)
						{
							GameFinishedButtons[i].State=BUTTON_HOVER;
						}
						else
						{
							GameFinishedButtons[i].State=BUTTON_NORMAL;
						}
					}
					else if(GameFinishedButtons[i].State!=BUTTON_CLICKED)
					{
						GameFinishedButtons[i].State=BUTTON_NORMAL;
					}
				}
			}
			break;

		case WM_LBUTTONDOWN:
			g_Mouse.Button.Left = true;
			g_Mouse.IsUsed=true;
			g_Mouse.ElapsedTime=0.0f;

			if(gamePhase==GP_MAIN_MENU)
			{
				for(register int i=0;i<4;++i)
				{
					if(MainMenuButtons[i].State==BUTTON_HOVER)
					{
						MainMenuButtons[i].State=BUTTON_CLICKED;
					}
				}
			}
			else if(gamePhase==GP_STARTGAME_MENU)
			{
				for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_BACK;++i)
				{
					if(StartGameMenuButtons[i].State==BUTTON_HOVER)
					{
						StartGameMenuButtons[i].State=BUTTON_CLICKED;
					}
				}
			}
			else if(gamePhase==GP_OPTIONS_MENU)
			{
				for(register int i=OPTION_LEFT;i<=OPTION_BACK;++i)
				{
					if(OptionsMenuButtons[i].State==BUTTON_HOVER)
					{
						OptionsMenuButtons[i].State=BUTTON_CLICKED;
					}
				}
			}
			else if(gamePhase==GP_GAMEOVER)
			{
				for(register int i=GAMEOVER_RESTART;i<=GAMEOVER_MAINMENU;++i)
				{
					if(GameOverButtons[i].State==BUTTON_HOVER)
					{
						GameOverButtons[i].State=BUTTON_CLICKED;
					}
				}
			}
			else if(gamePhase==GP_GAMEFINISHED)
			{
				for(register int i=GAMEFIN_RESTART;i<=GAMEFIN_NEXTLEVEL;++i)
				{
					if(GameFinishedButtons[i].State==BUTTON_HOVER)
					{
						GameFinishedButtons[i].State=BUTTON_CLICKED;
					}
				}
			}
            break;

        case WM_LBUTTONUP:
			g_Mouse.Button.Left = false;
            g_Mouse.IsUsed=true;
			g_Mouse.ElapsedTime=0.0f;

			if(gamePhase==GP_PLAYING)
			{
				if(g_bCheats[CHEAT_PICKME])
				{
					int posX=g_Mouse.Pos.x+(int)g_PointOfView.x;
					int posY=g_Mouse.Pos.y+(int)g_PointOfView.y;
					bool canTeleport=true;

					Vec2 lastPos=g_Player->mPosition;
					g_Player->mPosition=Vec2(posX,posY);

					RECT CR=g_Player->CRect();

					for(register int i=0;i<(int)g_Box.size();++i)
					{
						if(g_Box[i]->mIsSolid)
						{
							if(RectCollideRect(CR,g_Box[i]->Rect()))
							{
								canTeleport=false;
								break;
							}
						}
					}
					
					if(canTeleport)
					{
						for(register int i=0;i<(int)g_Stuff.size();++i)
						{
							if(g_Stuff[i]->mIsSolid)
							{
								if(RectCollideRect(g_Stuff[i]->Rect(),CR))
								{
									canTeleport=false;
									break;
								}
							}
						}
					}

					if(canTeleport)
					{
						for(register int i=0;i<(int)g_CRect.size();++i)
						{
							if(RectCollideRect(g_CRect[i],CR))
							{
								canTeleport=false;
								break;
							}
						}
					}

					if(canTeleport)
					{
						g_Player->mVelocity=Vec2(0,0);
					}
					else
					{
						g_Player->mPosition=lastPos;
					}
				}
			}
			if(gamePhase==GP_MAIN_MENU)
			{
				for(register int i=0;i<4;++i)
				{
					if(MainMenuButtons[i].State==BUTTON_CLICKED)
					{
						if(PointInRectangle(MainMenuButtons[i].ZONE,g_Mouse.Pos))
						{
							RAW_MAIN_MENU_BTNS->SetFrame(MainMenuButtons[i].Frame);
							COLORREF col=RAW_MAIN_MENU_BTNS->GetMaskPixel(g_Mouse.Pos.x-MainMenuButtons[i].Pos.x,g_Mouse.Pos.y-MainMenuButtons[i].Pos.y);
							if(col==0)
							{
								MainMenuButtons[i].State=BUTTON_HOVER;
								switch(i)
								{
									case MAIN_MENU_STARTGAME:
										for(register int i=0;i<4;++i)
											MainMenuButtons[i].State=BUTTON_NORMAL;
										gamePhase=GP_STARTGAME_MENU;
									break;
									case MAIN_MENU_OPTIONS:
										for(register int i=0;i<4;++i)
											MainMenuButtons[i].State=BUTTON_NORMAL;
										gamePhase=GP_OPTIONS_MENU;
									break;
									case MAIN_MENU_CREDITS:
										for(register int i=0;i<4;++i)
											MainMenuButtons[i].State=BUTTON_NORMAL;
										gamePhase=GP_CREDITS_MENU;
										CSound::Music_SetFocus(RAW_CREDITSMUSIC);
										RAW_CREDITSMUSIC->play();
									break;
									case MAIN_MENU_EXIT:
										for(register int i=0;i<4;++i)
											MainMenuButtons[i].State=BUTTON_NORMAL;
										PostQuitMessage(0);
									break;
								}
							}
							else
							{
								MainMenuButtons[i].State=BUTTON_NORMAL;
							}
						}
						else
						{
							MainMenuButtons[i].State=BUTTON_NORMAL;
						}
					}
				}
			}
			else if(gamePhase==GP_STARTGAME_MENU)
			{
				for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_LEVEL5;++i)
				{
					if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
					{
						if(StartGameMenuButtons[i].State==BUTTON_CLICKED)
						{
							if(PointInRectangle(StartGameMenuButtons[i].ZONE,g_Mouse.Pos))
							{
								RAW_LVLBTNS->SetFrame(StartGameMenuButtons[i].Frame);
								COLORREF col=RAW_LVLBTNS->GetMaskPixel(g_Mouse.Pos.x-StartGameMenuButtons[i].Pos.x,g_Mouse.Pos.y-StartGameMenuButtons[i].Pos.y);
								if(col==0)
								{
									StartGameMenuButtons[i].State=BUTTON_HOVER;
									switch(i)
									{
										case STARTGAME_LEVEL1:
											for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_BACK;++i)
											{
												if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
												{
													StartGameMenuButtons[i].State=BUTTON_NORMAL;
												}
											}
											LoadLevel1();
										break;
										case STARTGAME_LEVEL2:
											for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_BACK;++i)
											{
												if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
												{
													StartGameMenuButtons[i].State=BUTTON_NORMAL;
												}
											}
											LoadLevel2();
										break;
										case STARTGAME_LEVEL3:
											for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_BACK;++i)
											{
												if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
												{
													StartGameMenuButtons[i].State=BUTTON_NORMAL;
												}
											}
											LoadLevel3();
										break;
										case STARTGAME_LEVEL4:
											for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_BACK;++i)
											{
												if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
												{
													StartGameMenuButtons[i].State=BUTTON_NORMAL;
												}
											}
											LoadLevel4();
										break;
									}
								}
								else
								{
									StartGameMenuButtons[i].State=BUTTON_NORMAL;
								}
							}
							else
							{
								StartGameMenuButtons[i].State=BUTTON_NORMAL;
							}
						}
					}
				}

				for(register int i=STARTGAME_CONTINUE;i<=STARTGAME_BACK;++i)
				{
					if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
					{
						if(StartGameMenuButtons[i].State==BUTTON_CLICKED)
						{
							if(PointInRectangle(StartGameMenuButtons[i].ZONE,g_Mouse.Pos))
							{
								RAW_BCBTNS->SetFrame(StartGameMenuButtons[i].Frame);
								COLORREF col=RAW_BCBTNS->GetMaskPixel(g_Mouse.Pos.x-StartGameMenuButtons[i].Pos.x,g_Mouse.Pos.y-StartGameMenuButtons[i].Pos.y);
								if(col==0)
								{
									StartGameMenuButtons[i].State=BUTTON_HOVER;
									switch(i)
									{
										case STARTGAME_CONTINUE:
											for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_BACK;++i)
											{
												if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
												{
													StartGameMenuButtons[i].State=BUTTON_NORMAL;
												}
											}
											gamePhase=GP_PLAYING;
											CSound::Music_GetFocus()->pause(true);
											CSound::Music_SetFocus(Playing_Paused_Music);
										break;
										case STARTGAME_BACK:
											for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_BACK;++i)
											{
												if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
												{
													StartGameMenuButtons[i].State=BUTTON_NORMAL;
												}
											}
											gamePhase=GP_MAIN_MENU;
										break;
									}
								}
								else
								{
									StartGameMenuButtons[i].State=BUTTON_NORMAL;
								}
							}
							else
							{
								StartGameMenuButtons[i].State=BUTTON_NORMAL;
							}
						}
					}
				}
			}
			else if(gamePhase==GP_OPTIONS_MENU)
			{
				if(OptionsMenuButtons[OPTION_LEFT].State==BUTTON_CLICKED)
				{
					if(PointInRectangle(OptionsMenuButtons[OPTION_LEFT].ZONE,g_Mouse.Pos))
					{
						RAW_OPTBTNS->SetFrame(OptionsMenuButtons[OPTION_LEFT].Frame);
						COLORREF col=RAW_OPTBTNS->GetMaskPixel(g_Mouse.Pos.x-OptionsMenuButtons[OPTION_LEFT].Pos.x,g_Mouse.Pos.y-OptionsMenuButtons[OPTION_LEFT].Pos.y);
						if(col==0)
						{
							OptionsMenuButtons[OPTION_LEFT].State=BUTTON_HOVER;
						}
						else
						{
							OptionsMenuButtons[OPTION_LEFT].State=BUTTON_NORMAL;
						}
					}
					else
					{
						OptionsMenuButtons[OPTION_LEFT].State=BUTTON_NORMAL;
					}
				}

				if(OptionsMenuButtons[OPTION_RIGHT].State==BUTTON_CLICKED)
				{
					if(PointInRectangle(OptionsMenuButtons[OPTION_RIGHT].ZONE,g_Mouse.Pos))
					{
						RAW_OPTBTNS->SetFrame(OptionsMenuButtons[OPTION_RIGHT].Frame);
						COLORREF col=RAW_OPTBTNS->GetMaskPixel(g_Mouse.Pos.x-OptionsMenuButtons[OPTION_RIGHT].Pos.x,g_Mouse.Pos.y-OptionsMenuButtons[OPTION_RIGHT].Pos.y);
						if(col==0)
						{
							OptionsMenuButtons[OPTION_RIGHT].State=BUTTON_HOVER;
						}
						else
						{
							OptionsMenuButtons[OPTION_RIGHT].State=BUTTON_NORMAL;
						}
					}
					else
					{
						OptionsMenuButtons[OPTION_RIGHT].State=BUTTON_NORMAL;
					}
				}

				if(OptionsMenuButtons[OPTION_BACK].State==BUTTON_CLICKED)
				{
					if(PointInRectangle(OptionsMenuButtons[OPTION_BACK].ZONE,g_Mouse.Pos))
					{
						RAW_BCBTNS->SetFrame(OptionsMenuButtons[OPTION_BACK].Frame);
						COLORREF col=RAW_BCBTNS->GetMaskPixel(g_Mouse.Pos.x-OptionsMenuButtons[OPTION_BACK].Pos.x,g_Mouse.Pos.y-OptionsMenuButtons[OPTION_BACK].Pos.y);
						if(col==0)
						{
							OptionsMenuButtons[OPTION_BACK].State=BUTTON_HOVER;
							for(register int i=OPTION_LEFT;i<=OPTION_RIGHT;++i)
								OptionsMenuButtons[i].State=BUTTON_NORMAL;
							gamePhase=GP_MAIN_MENU;
						}
						else
						{
							OptionsMenuButtons[OPTION_BACK].State=BUTTON_NORMAL;
						}
					}
					else
					{
						OptionsMenuButtons[OPTION_BACK].State=BUTTON_NORMAL;
					}
				}
			}
			else if(gamePhase==GP_GAMEOVER)
			{
				for(register int i=GAMEOVER_RESTART;i<=GAMEOVER_MAINMENU;++i)
				{
					if(GameOverButtons[i].State==BUTTON_CLICKED)
					{
						if(PointInRectangle(GameOverButtons[i].ZONE,g_Mouse.Pos))
						{
							RAW_GOFBTNS->SetFrame(GameOverButtons[i].Frame);
							COLORREF col=RAW_GOFBTNS->GetMaskPixel(g_Mouse.Pos.x-GameOverButtons[i].Pos.x,g_Mouse.Pos.y-GameOverButtons[i].Pos.y);
							if(col==0)
							{
								GameOverButtons[i].State=BUTTON_HOVER;
								switch(i)
								{
									case GAMEOVER_RESTART:
										for(register int i=GAMEOVER_RESTART;i<=GAMEOVER_MAINMENU;++i)
											GameOverButtons[i].State=BUTTON_NORMAL;
										switch(currLevel)
										{
											case 1:
												if(CheckPoint::CanLoad())
												{
													CheckPoint::Load();
													gamePhase=GP_PLAYING;
													CSound::Music_SetFocus(RAW_LEVEL1MUSIC);
													RAW_LEVEL1MUSIC->play();
												}
												else
												{
													LoadLevel1();
												}
											break;
											case 2:
												if(CheckPoint::CanLoad())
												{
													CheckPoint::Load();
													gamePhase=GP_PLAYING;
													CSound::Music_SetFocus(RAW_LEVEL2MUSIC);
													RAW_LEVEL2MUSIC->play();
												}
												else
												{
													LoadLevel2();
												}
											break;
											case 3:
												if(CheckPoint::CanLoad())
												{
													CheckPoint::Load();
													gamePhase=GP_PLAYING;
													CSound::Music_SetFocus(RAW_LEVEL3MUSIC);
													RAW_LEVEL3MUSIC->play();
												}
												else
												{
													LoadLevel3();
												}
											break;
											case 5:
												if(CheckPoint::CanLoad())
												{
													CheckPoint::Load();
													gamePhase=GP_PLAYING;
													CSound::Music_SetFocus(RAW_LEVEL5MUSIC);
													RAW_LEVEL5MUSIC->play();
												}
												else
												{
													LoadLevel4();
												}
											break;
										}
									break;
									case GAMEOVER_MAINMENU:
										for(register int i=GAMEOVER_RESTART;i<=GAMEOVER_MAINMENU;++i)
											GameOverButtons[i].State=BUTTON_NORMAL;
										CSound *pSound;
										if(g_bIsFirstMenuMusic)
										{
											pSound=RAW_MENUMUSIC;
										}
										else
										{
											pSound=RAW_MENU2MUSIC;
										}
										g_bIsFirstMenuMusic=!g_bIsFirstMenuMusic;
										CSound::Music_SetFocus(pSound);
										pSound->play();
										gamePhase=GP_MAIN_MENU;
									break;
								}
							}
							else
							{
								GameOverButtons[i].State=BUTTON_NORMAL;
							}
						}
						else
						{
							GameOverButtons[i].State=BUTTON_NORMAL;
						}
					}
				}
			}
			else if(gamePhase==GP_GAMEFINISHED)
			{
				for(register int i=GAMEFIN_RESTART;i<=GAMEFIN_NEXTLEVEL;++i)
				{
					if(GameFinishedButtons[i].State==BUTTON_CLICKED)
					{
						if(PointInRectangle(GameFinishedButtons[i].ZONE,g_Mouse.Pos))
						{
							RAW_GOFBTNS->SetFrame(GameFinishedButtons[i].Frame);
							COLORREF col=RAW_GOFBTNS->GetMaskPixel(g_Mouse.Pos.x-GameFinishedButtons[i].Pos.x,g_Mouse.Pos.y-GameFinishedButtons[i].Pos.y);
							if(col==0)
							{
								GameFinishedButtons[i].State=BUTTON_HOVER;
								switch(i)
								{
									case GAMEFIN_RESTART:
										for(register int i=GAMEFIN_RESTART;i<=GAMEFIN_NEXTLEVEL;++i)
											GameFinishedButtons[i].State=BUTTON_NORMAL;
										switch(currLevel)
										{
											case 1:
												LoadLevel1();
											break;
											case 2:
												LoadLevel2();
											break;
											case 3:
												LoadLevel3();
											break;
											case 5:
												LoadLevel4();
											break;
										}
									break;
									case GAMEFIN_MAINMENU:
										for(register int i=GAMEFIN_RESTART;i<=GAMEFIN_NEXTLEVEL;++i)
											GameFinishedButtons[i].State=BUTTON_NORMAL;
										CSound *pSound;
										if(g_bIsFirstMenuMusic)
										{
											pSound=RAW_MENUMUSIC;
										}
										else
										{
											pSound=RAW_MENU2MUSIC;
										}
										g_bIsFirstMenuMusic=!g_bIsFirstMenuMusic;
										CSound::Music_SetFocus(pSound);
										pSound->play();
										gamePhase=GP_MAIN_MENU;
									break;
									case GAMEFIN_NEXTLEVEL:
										for(register int i=GAMEFIN_RESTART;i<=GAMEFIN_NEXTLEVEL;++i)
											GameFinishedButtons[i].State=BUTTON_NORMAL;
										switch(currLevel)
										{
											case 1:
												LoadLevel2();
											break;
											case 2:
												LoadLevel3();
											break;
											case 3:
												LoadLevel4();
											break;
											case 4:
												gamePhase=GP_CREDITS_MENU;
												CSound::Music_SetFocus(RAW_CREDITSMUSIC);
												RAW_CREDITSMUSIC->play();
											break;
										}
									break;
								}
							}
							else
							{
								GameFinishedButtons[i].State=BUTTON_NORMAL;
							}
						}
						else
						{
							GameFinishedButtons[i].State=BUTTON_NORMAL;
						}
					}
				}
			}
			break;

		case WM_RBUTTONDOWN:
			g_Mouse.Button.Right = true;
			g_Mouse.IsUsed=true;
			g_Mouse.ElapsedTime=0.0f;
			break;

		case WM_RBUTTONUP:
			g_Mouse.Button.Right = false;
			g_Mouse.IsUsed=true;
			g_Mouse.ElapsedTime=0.0f;
			break;

		case WM_MBUTTONDOWN:
			g_Mouse.Button.Middle = true;
			g_Mouse.IsUsed=true;
			g_Mouse.ElapsedTime=0.0f;
			break;

		case WM_MBUTTONUP:
			g_Mouse.Button.Middle = false;
			g_Mouse.IsUsed=true;
			g_Mouse.ElapsedTime=0.0f;
			break;

		case WM_KEYDOWN:
			if(gamePhase!=GP_LEVEL2_DIALOG2 && gamePhase!=GP_LEVEL2_DIALOG && gamePhase!=GP_LEVEL1_INTRO && gamePhase!=GP_LEVEL2_INTRO && gamePhase!=GP_LEVEL3_INTRO && gamePhase!=GP_LEVEL4_INTRO && gamePhase!=GP_LEVEL5_INTRO)
			{
				if(g_bCanCheat)
				{
					if('A'<=(char)wParam && (char)wParam<='Z' )
					{
						g_cheatKeyBuffer+=(char)(wParam);
						checkForCheat();
					}
				}
			}
			else
			{
				introPhase=JUST_QUIT;
			}
			switch(wParam)
			{
				case VK_ESCAPE:
					if(gamePhase==GP_PLAYING)
					{
						gamePhase=GP_MAIN_MENU;
						Playing_Paused_Music=CSound::Music_GetFocus();
						CSound *pSound;
						if(g_bIsFirstMenuMusic)
						{
							pSound=RAW_MENUMUSIC;
						}
						else
						{
							pSound=RAW_MENU2MUSIC;
						}
						g_bIsFirstMenuMusic=!g_bIsFirstMenuMusic;
						CSound::Music_SetFocus(pSound);
						pSound->play();
					}
					else if(gamePhase==GP_MAIN_MENU)
					{
						PostQuitMessage(0);
					}
					else if(gamePhase==GP_STARTGAME_MENU)
					{
						for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_BACK;++i)
						{
							if(StartGameMenuButtons[i].State!=BUTTON_DISABLED)
							{
								StartGameMenuButtons[i].State=BUTTON_NORMAL;
							}
						}
						gamePhase=GP_MAIN_MENU;
					}
					else if(gamePhase==GP_CREDITS_MENU)
					{
						CSound *pSound;
						if(g_bIsFirstMenuMusic)
						{
							pSound=RAW_MENUMUSIC;
						}
						else
						{
							pSound=RAW_MENU2MUSIC;
						}
						g_bIsFirstMenuMusic=!g_bIsFirstMenuMusic;
						CSound::Music_SetFocus(pSound);
						pSound->play();
						gamePhase=GP_MAIN_MENU;
					}
					else if(gamePhase==GP_OPTIONS_MENU)
					{
						gamePhase=GP_MAIN_MENU;
						for(register int i=OPTION_LEFT;i<=OPTION_RIGHT;++i)
							OptionsMenuButtons[i].State=BUTTON_NORMAL;
					}
					else if(gamePhase==GP_GAMEOVER)
					{
						CSound *pSound;
						if(g_bIsFirstMenuMusic)
						{
							pSound=RAW_MENUMUSIC;
						}
						else
						{
							pSound=RAW_MENU2MUSIC;
						}
						g_bIsFirstMenuMusic=!g_bIsFirstMenuMusic;
						CSound::Music_SetFocus(pSound);
						pSound->play();
						gamePhase=GP_MAIN_MENU;
					}
					else if(gamePhase==GP_GAMEFINISHED)
					{
						CSound *pSound;
						if(g_bIsFirstMenuMusic)
						{
							pSound=RAW_MENUMUSIC;
						}
						else
						{
							pSound=RAW_MENU2MUSIC;
						}
						g_bIsFirstMenuMusic=!g_bIsFirstMenuMusic;
						CSound::Music_SetFocus(pSound);
						pSound->play();
						gamePhase=GP_MAIN_MENU;
					}
					else if(gamePhase==GP_FUNMODE)
					{
						CSound *pSound;
						if(g_bIsFirstMenuMusic)
						{
							pSound=RAW_MENUMUSIC;
						}
						else
						{
							pSound=RAW_MENU2MUSIC;
						}
						g_bIsFirstMenuMusic=!g_bIsFirstMenuMusic;
						CSound::Music_SetFocus(pSound);
						pSound->play();
						gamePhase=GP_MAIN_MENU;
					}
				break;
			}
			break;

		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0;
}

void InitDisplay()
{
	FileCheck("Data\\START.ini");
	std::ifstream fin("Data\\START.ini");
	int x;
	fin>>x;
	if(x==1)
	{
		g_bFullScreen=true;
	}
	else if(x==0)
	{
		g_bFullScreen=false;
	}
	fin>>x;
	if(x==1)
	{
		g_bCanCheat=true;
	}
	else if(x==0)
	{
		g_bCanCheat=false;
	}
	fin.close();

	LPTSTR          WindowTitle  = _T("N-Hunter");
	LPCSTR			WindowClass  = _T("N-Hunter_Class");
   	WNDCLASSEX		wcex;
	g_nViewWidth 	= 1024;
	g_nViewHeight 	= 768;

	if(g_bFullScreen)
	{
		DEVMODEA devMode;
		EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&devMode);
		g_DefaultDisplay=devMode;
		devMode.dmPelsWidth=g_nViewWidth;
		devMode.dmPelsHeight=g_nViewHeight;
		ChangeDisplaySettingsEx(NULL,&devMode,NULL,NULL,NULL);
	}

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInst;
	wcex.hIcon			= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(g_hInst,IDC_ICON);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= WindowClass;
	wcex.hIconSm		= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON1));

	if(RegisterClassEx(&wcex)==0)
	{
		throw FatalError("Failed to register class");
	}

    g_nViewWidth  = 1024;
    g_nViewHeight = 768;

	if(g_bFullScreen)
	{
		g_hWnd = CreateWindow(WindowClass, WindowTitle, WS_POPUP,
			0, 0, g_nViewWidth, g_nViewHeight, NULL, NULL, g_hInst, NULL);
	}
	else
	{
		g_hWnd = CreateWindow(WindowClass, WindowTitle, WS_CAPTION|WS_SYSMENU|WS_OVERLAPPED|WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, g_nViewWidth, g_nViewHeight, NULL, NULL, g_hInst, NULL);
	}

	if(!g_hWnd)
	{
		throw FatalError("Invalid window handle");
	}

	SetClassLong(g_hWnd, GCL_HICON, (LONG)LoadIcon(g_hInst, (LPCTSTR)IDI_ICON1));
	SetClassLong(g_hWnd, GCL_HICONSM, (LONG)LoadIcon(g_hInst, (LPCTSTR)IDI_ICON1));

	if(g_bFullScreen)
	{
		ShowWindow(g_hWnd, SW_MAXIMIZE);
	}
	else
	{
		ShowWindow(g_hWnd, SW_SHOW);
	}

	g_BBuffer=new BackBuffer(g_hWnd,g_nViewWidth,g_nViewHeight);

	g_hdcSprite=CreateCompatibleDC(GetDC(g_hWnd));
	Sprite::setDefaultSpriteDC(g_hdcSprite);

	ShowCursor(false);
}

void LoadResources()
{
	RAW_LOADING_SCREEN.LoadBitmapFromFile("Data\\Images\\LSCR.bmp",GetDC(g_hWnd));
	g_BBuffer->reset();
	HDC hdc=g_BBuffer->getDC();
	RAW_LOADING_SCREEN.Paint(hdc,0,0);
	g_BBuffer->present();

	g_Player=new CPlayer();
	g_Player2=new CPlayer();
	g_Player2->getRifle_pl2();
	RAW_BACKGROUND_LV1BK1.LoadBitmapFromFile("Data\\Images\\L1BK1.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK2.LoadBitmapFromFile("Data\\Images\\L1BK2.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK3.LoadBitmapFromFile("Data\\Images\\L1BK3.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK4.LoadBitmapFromFile("Data\\Images\\L1BK4.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK5.LoadBitmapFromFile("Data\\Images\\L1BK5.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK6.LoadBitmapFromFile("Data\\Images\\L1BK6.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK7.LoadBitmapFromFile("Data\\Images\\L1BK7.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK8.LoadBitmapFromFile("Data\\Images\\L1BK8.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK9.LoadBitmapFromFile("Data\\Images\\L1BK9.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK10.LoadBitmapFromFile("Data\\Images\\L1BK10.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK11.LoadBitmapFromFile("Data\\Images\\L1BK11.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK12.LoadBitmapFromFile("Data\\Images\\L1BK12.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK13.LoadBitmapFromFile("Data\\Images\\L1BK13.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV1BK14.LoadBitmapFromFile("Data\\Images\\L1BK14.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV5BK1.LoadBitmapFromFile("Data\\Images\\L5BK1.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV5BK2.LoadBitmapFromFile("Data\\Images\\L5BK2.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV2BK1.LoadBitmapFromFile("Data\\Images\\L2P1BK1.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV2BK2.LoadBitmapFromFile("Data\\Images\\L2P1BK2.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK1.LoadBitmapFromFile("Data\\Images\\L3BK1.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK2.LoadBitmapFromFile("Data\\Images\\L3BK2.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK3.LoadBitmapFromFile("Data\\Images\\L3BK3.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK4.LoadBitmapFromFile("Data\\Images\\L3BK4.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK5.LoadBitmapFromFile("Data\\Images\\L3BK5.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK6.LoadBitmapFromFile("Data\\Images\\L3BK6.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK7.LoadBitmapFromFile("Data\\Images\\L3BK7.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK8.LoadBitmapFromFile("Data\\Images\\L3BK8.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK9.LoadBitmapFromFile("Data\\Images\\L3BK9.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK10.LoadBitmapFromFile("Data\\Images\\L3BK10.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK11.LoadBitmapFromFile("Data\\Images\\L3BK11.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK12.LoadBitmapFromFile("Data\\Images\\L3BK12.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK13.LoadBitmapFromFile("Data\\Images\\L3BK13.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK14.LoadBitmapFromFile("Data\\Images\\L3BK14.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK15.LoadBitmapFromFile("Data\\Images\\L3BK15.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK16.LoadBitmapFromFile("Data\\Images\\L3BK16.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK17.LoadBitmapFromFile("Data\\Images\\L3BK17.bmp",GetDC(g_hWnd));
	RAW_BACKGROUND_LV3BK18.LoadBitmapFromFile("Data\\Images\\L3BK18.bmp",GetDC(g_hWnd));
	RAW_MAINMENUNLBK.LoadBitmapFromFile("Data\\Images\\MBK.bmp",GetDC(g_hWnd));
	RAW_OPTMENUBK.LoadBitmapFromFile("Data\\Images\\OPTMBK.bmp",GetDC(g_hWnd));
	RAW_MAINMENUBK.LoadBitmapFromFile("Data\\Images\\MAINMENUBK.bmp",GetDC(g_hWnd));
	RAW_GFBK.LoadBitmapFromFile("Data\\Images\\GFBK.bmp",GetDC(g_hWnd));
	RAW_GOBK.LoadBitmapFromFile("Data\\Images\\GOBK.bmp",GetDC(g_hWnd));
	RAW_FUNMODEBK.LoadBitmapFromFile("Data\\Images\\FUNMODEBK.bmp",GetDC(g_hWnd));

	RECT BOSSHP_ANIMATION_FRAME_SIZE;
	BOSSHP_ANIMATION_FRAME_SIZE.top = BOSSHP_ANIMATION_FRAME_SIZE.left = 0;
	BOSSHP_ANIMATION_FRAME_SIZE.right = 350; BOSSHP_ANIMATION_FRAME_SIZE.bottom = 100;
	RAW_BOSSHP=new AnimatedSprite("Data\\Sprites\\BOSSHP.bmp","Data\\Sprites\\BOSSHPM.bmp",BOSSHP_ANIMATION_FRAME_SIZE,50);

	RECT PLAYER_ANIMATION_FRAME_SIZE;
	PLAYER_ANIMATION_FRAME_SIZE.left=0;
	PLAYER_ANIMATION_FRAME_SIZE.top=0;
	PLAYER_ANIMATION_FRAME_SIZE.bottom=200;
	PLAYER_ANIMATION_FRAME_SIZE.right=200;
	RAW_PLAYER_MOVEMENT_ANIMATION_RIFLE=new AnimatedSprite("Data\\Sprites\\PMA.bmp","Data\\Sprites\\PMAM.bmp",PLAYER_ANIMATION_FRAME_SIZE,80);
	RAW_PLAYER2_MOVEMENT_ANIMATION_RIFLE=new AnimatedSprite("Data\\Sprites\\PMA2.bmp","Data\\Sprites\\PMA2M.bmp",PLAYER_ANIMATION_FRAME_SIZE,80);
	RAW_PLAYER_MOVEMENT_ANIMATION_SHOTGUN=new AnimatedSprite("Data\\Sprites\\PMASHOTGUN.bmp","Data\\Sprites\\PMASHOTGUNM.bmp",PLAYER_ANIMATION_FRAME_SIZE,80);
	RAW_PLAYER_MOVEMENT_ANIMATION_PISTOL=new AnimatedSprite("Data\\Sprites\\PMAPISTOL.bmp","Data\\Sprites\\PMAPISTOLM.bmp",PLAYER_ANIMATION_FRAME_SIZE,80);
	RAW_PLAYER_MOVEMENT_ANIMATION_BAZOOKA=new AnimatedSprite("Data\\Sprites\\PMABAZOOKA.bmp","Data\\Sprites\\PMABAZOOKAM.bmp",PLAYER_ANIMATION_FRAME_SIZE,80);

	RAW_W1_PRJ=new Sprite("Data\\Sprites\\PRJ1.bmp","Data\\Sprites\\PRJ1M.bmp");
	RAW_W2_PRJ=new Sprite("Data\\Sprites\\PRJ2.bmp","Data\\Sprites\\PRJ2M.bmp");
	RAW_W3_PRJ=new Sprite("Data\\Sprites\\PRJ3.bmp","Data\\Sprites\\PRJ3M.bmp");
	RAW_CURSOR=new Sprite("Data\\Sprites\\Cursor.bmp","Data\\Sprites\\CursorMask.bmp");
	RAW_SBK=new Sprite("Data\\Sprites\\SBK.bmp","Data\\Sprites\\SBKM.bmp");
	RAW_PHPBK=new Sprite("Data\\Sprites\\PHPBK.bmp","Data\\Sprites\\PHPBKM.bmp");
	RAW_PHPBK2=new Sprite("Data\\Sprites\\PHPBK.bmp","Data\\Sprites\\PHPBKM.bmp");
	RAW_PARALLAX_L2P2BK1=new Sprite("Data\\Images\\L2P2BK1.bmp","Data\\Images\\L2P2BK1M.bmp");
	RAW_PARALLAX_L2P2BK2=new Sprite("Data\\Images\\L2P2BK2.bmp","Data\\Images\\L2P2BK2M.bmp");
	RAW_PARALLAX_L2P2BK3=new Sprite("Data\\Images\\L2P2BK3.bmp","Data\\Images\\L2P2BK3M.bmp");
	RAW_PARALLAX_L2P2BK4=new Sprite("Data\\Images\\L2P2BK4.bmp","Data\\Images\\L2P2BK4M.bmp");
	RAW_PARALLAX_L2P3BK1=new Sprite("Data\\Images\\L2P3BK1.bmp","Data\\Images\\L2P3BK1M.bmp");
	RAW_PARALLAX_L2P3BK2=new Sprite("Data\\Images\\L2P3BK2.bmp","Data\\Images\\L2P3BK2M.bmp");
	RAW_REPAIRKIT=new Sprite("Data\\Sprites\\REPAIRKIT.bmp","Data\\Sprites\\REPAIRKITM.bmp");

	RECT EXP1_ANIMATION_FRAME_SIZE;
	EXP1_ANIMATION_FRAME_SIZE.left = EXP1_ANIMATION_FRAME_SIZE.top = 0;
	EXP1_ANIMATION_FRAME_SIZE.right = 120; EXP1_ANIMATION_FRAME_SIZE.bottom = 90;
	RAW_EXP1=new AnimatedSprite("Data\\Sprites\\EXP1.bmp","Data\\Sprites\\EXP1M.bmp",EXP1_ANIMATION_FRAME_SIZE,20);

	RECT EXP2_ANIMATION_FRAME_SIZE;
	EXP2_ANIMATION_FRAME_SIZE.left = EXP2_ANIMATION_FRAME_SIZE.top = 0;
	EXP2_ANIMATION_FRAME_SIZE.right = 320; EXP2_ANIMATION_FRAME_SIZE.bottom = 240;
	RAW_EXP2=new AnimatedSprite("Data\\Sprites\\EXP2.bmp","Data\\Sprites\\EXP2M.bmp",EXP2_ANIMATION_FRAME_SIZE,20);

	RECT T1_ANIMATION_FRAME_SIZE;
	T1_ANIMATION_FRAME_SIZE.left = T1_ANIMATION_FRAME_SIZE.top = 0;
	T1_ANIMATION_FRAME_SIZE.bottom = 40; T1_ANIMATION_FRAME_SIZE.right = 25;
	RAW_TURRET1=new AnimatedSprite("Data\\Sprites\\T1.bmp","Data\\Sprites\\T1M.bmp",T1_ANIMATION_FRAME_SIZE,8);

	RECT GOFBTNS_ANIMATION_FRAME_SIZE;
	GOFBTNS_ANIMATION_FRAME_SIZE.left = GOFBTNS_ANIMATION_FRAME_SIZE.top = 0;
	GOFBTNS_ANIMATION_FRAME_SIZE.bottom = 215; GOFBTNS_ANIMATION_FRAME_SIZE.right=340;
	RAW_GOFBTNS=new AnimatedSprite("Data\\Sprites\\GOFBTNS.bmp","Data\\Sprites\\GOFBTNSM.bmp",GOFBTNS_ANIMATION_FRAME_SIZE,9);

	RECT BOX1_ANIMATION_FRAME_SIZE;
	BOX1_ANIMATION_FRAME_SIZE.top = BOX1_ANIMATION_FRAME_SIZE.left = 0;
	BOX1_ANIMATION_FRAME_SIZE.bottom = BOX1_ANIMATION_FRAME_SIZE.right = 138;
	RAW_BOX1_ANIMATION = new AnimatedSprite("Data\\Sprites\\B1.bmp","Data\\Sprites\\B1M.bmp",BOX1_ANIMATION_FRAME_SIZE,8);

	RECT BOX2_ANIMATION_FRAME_SIZE;
	BOX2_ANIMATION_FRAME_SIZE.top = BOX2_ANIMATION_FRAME_SIZE.left = 0;
	BOX2_ANIMATION_FRAME_SIZE.bottom = BOX2_ANIMATION_FRAME_SIZE.right = 140;
	RAW_BOX2_ANIMATION = new AnimatedSprite("Data\\Sprites\\B2.bmp","Data\\Sprites\\B2M.bmp",BOX2_ANIMATION_FRAME_SIZE,8);

	RECT BOX3_ANIMATION_FRAME_SIZE;
	BOX3_ANIMATION_FRAME_SIZE.top = BOX3_ANIMATION_FRAME_SIZE.left = 0;
	BOX3_ANIMATION_FRAME_SIZE.bottom = BOX3_ANIMATION_FRAME_SIZE.right = 150;
	RAW_BOX3_ANIMATION = new AnimatedSprite("Data\\Sprites\\B3.bmp","Data\\Sprites\\B3M.bmp",BOX3_ANIMATION_FRAME_SIZE,8);

	RECT FLOOR1_ANIMATION_FRAME_SIZE;
	FLOOR1_ANIMATION_FRAME_SIZE.top = FLOOR1_ANIMATION_FRAME_SIZE.left = 0;
	FLOOR1_ANIMATION_FRAME_SIZE.bottom = 50;
	FLOOR1_ANIMATION_FRAME_SIZE.right = 100;
	RAW_FLOOR1_ANIMATION = new AnimatedSprite("Data\\Sprites\\FF1.bmp","Data\\Sprites\\FF1M.bmp",FLOOR1_ANIMATION_FRAME_SIZE,4);

	RECT DOOR1_ANIMATION_FRAME_SIZE;
	DOOR1_ANIMATION_FRAME_SIZE.top = DOOR1_ANIMATION_FRAME_SIZE.left = 0;
	DOOR1_ANIMATION_FRAME_SIZE.bottom = 200;
	DOOR1_ANIMATION_FRAME_SIZE.right = 60;
	RAW_DOOR1_ANIMATION  = new AnimatedSprite("Data\\Sprites\\D1.bmp","Data\\Sprites\\D1M.bmp",DOOR1_ANIMATION_FRAME_SIZE,9);
	
	RECT SWITCH1_ANIMATION_FRAME_SIZE;
	SWITCH1_ANIMATION_FRAME_SIZE.top = SWITCH1_ANIMATION_FRAME_SIZE.left = 0;
	SWITCH1_ANIMATION_FRAME_SIZE.right = SWITCH1_ANIMATION_FRAME_SIZE.bottom = 70;
	RAW_SWITCH1_ANIMATION = new AnimatedSprite("Data\\Sprites\\SW1.bmp","Data\\Sprites\\SW1M.bmp",SWITCH1_ANIMATION_FRAME_SIZE,2);

	RECT TURRET1_ANIMATION_FRAME_SIZE;
	TURRET1_ANIMATION_FRAME_SIZE.top = TURRET1_ANIMATION_FRAME_SIZE.left = 0;
	TURRET1_ANIMATION_FRAME_SIZE.right = 120; TURRET1_ANIMATION_FRAME_SIZE.bottom = 100;
	RAW_TURRET1_ANIMATION = new AnimatedSprite("Data\\Sprites\\T1.bmp","Data\\Sprites\\T1M.bmp",TURRET1_ANIMATION_FRAME_SIZE,1);

	RECT DUMMY1L_ANIMATION_FRAME_SIZE;
	DUMMY1L_ANIMATION_FRAME_SIZE.top = DUMMY1L_ANIMATION_FRAME_SIZE.left = 0;
	DUMMY1L_ANIMATION_FRAME_SIZE.right = 100;
	DUMMY1L_ANIMATION_FRAME_SIZE.bottom = 150;
	RAW_DUMMY1L_ANIMATION = new AnimatedSprite("Data\\Sprites\\DUMMY1L.bmp","Data\\Sprites\\DUMMY1LM.bmp",DUMMY1L_ANIMATION_FRAME_SIZE,24);

	RECT DUMMY1R_ANIMATION_FRAME_SIZE;
	DUMMY1R_ANIMATION_FRAME_SIZE.top = DUMMY1R_ANIMATION_FRAME_SIZE.left = 0;
	DUMMY1R_ANIMATION_FRAME_SIZE.right = 100;
	DUMMY1R_ANIMATION_FRAME_SIZE.bottom = 150;
	RAW_DUMMY1R_ANIMATION = new AnimatedSprite("Data\\Sprites\\DUMMY1R.bmp","Data\\Sprites\\DUMMY1RM.bmp",DUMMY1R_ANIMATION_FRAME_SIZE,24);
	
	RECT PLATFORM1_ANIMATION_FRAME_SIZE;
	PLATFORM1_ANIMATION_FRAME_SIZE.top = PLATFORM1_ANIMATION_FRAME_SIZE.left = 0;
	PLATFORM1_ANIMATION_FRAME_SIZE.right = 200;
	PLATFORM1_ANIMATION_FRAME_SIZE.bottom = 15;
	RAW_PLATFORM1_ANIMATION = new AnimatedSprite("Data\\Sprites\\P1.bmp","Data\\Sprites\\P1M.bmp",PLATFORM1_ANIMATION_FRAME_SIZE,1);

	RECT POWERUP_ANIMATION_FRAME_SIZE;
	POWERUP_ANIMATION_FRAME_SIZE.top = POWERUP_ANIMATION_FRAME_SIZE.left = 0;
	POWERUP_ANIMATION_FRAME_SIZE.right = POWERUP_ANIMATION_FRAME_SIZE.bottom= 200;
	RAW_LARMOR_PWUP_ANIMATION = new AnimatedSprite("Data\\Sprites\\PWUP_ARMORL.bmp","Data\\Sprites\\PWUP_ARMORLM.bmp",POWERUP_ANIMATION_FRAME_SIZE,6);
	RAW_LSPEED_PWUP_ANIMATION = new AnimatedSprite("Data\\Sprites\\PWUP_ARMORL.bmp","Data\\Sprites\\PWUP_ARMORLM.bmp",POWERUP_ANIMATION_FRAME_SIZE,6);
	RAW_LSTRENGTH_PWUP_ANIMATION = new AnimatedSprite("Data\\Sprites\\PWUP_ARMORL.bmp","Data\\Sprites\\PWUP_ARMORLM.bmp",POWERUP_ANIMATION_FRAME_SIZE,6);
	RAW_RARMOR_PWUP_ANIMATION = new AnimatedSprite("Data\\Sprites\\PWUP_ARMORR.bmp","Data\\Sprites\\PWUP_ARMORRM.bmp",POWERUP_ANIMATION_FRAME_SIZE,6);
	RAW_RSPEED_PWUP_ANIMATION = new AnimatedSprite("Data\\Sprites\\PWUP_ARMORR.bmp","Data\\Sprites\\PWUP_ARMORRM.bmp",POWERUP_ANIMATION_FRAME_SIZE,6);
	RAW_RSTRENGTH_PWUP_ANIMATION = new AnimatedSprite("Data\\Sprites\\PWUP_ARMORR.bmp","Data\\Sprites\\PWUP_ARMORRM.bmp",POWERUP_ANIMATION_FRAME_SIZE,6);

	RECT MMBTNS_ANIMATION_FRAME_SIZE;
	MMBTNS_ANIMATION_FRAME_SIZE.top = MMBTNS_ANIMATION_FRAME_SIZE.left = 0;
	MMBTNS_ANIMATION_FRAME_SIZE.right = 300;
	MMBTNS_ANIMATION_FRAME_SIZE.bottom = 200;
	RAW_MAIN_MENU_BTNS = new AnimatedSprite("Data\\Sprites\\MMBTNS.bmp","Data\\Sprites\\MMBTNSM.bmp",MMBTNS_ANIMATION_FRAME_SIZE,12);

	RECT BCBTNS_ANIMATION_FRAME_SIZE;
	BCBTNS_ANIMATION_FRAME_SIZE.top = BCBTNS_ANIMATION_FRAME_SIZE.left = 0;
	BCBTNS_ANIMATION_FRAME_SIZE.right = 370;
	BCBTNS_ANIMATION_FRAME_SIZE.bottom = 150;
	RAW_BCBTNS = new AnimatedSprite("Data\\Sprites\\BCBTNS.bmp","Data\\Sprites\\BCBTNSM.bmp",BCBTNS_ANIMATION_FRAME_SIZE,8);
		
	RECT LVLBTNS_ANIMATION_FRAME_SIZE;
	LVLBTNS_ANIMATION_FRAME_SIZE.top = LVLBTNS_ANIMATION_FRAME_SIZE.left = 0;
	LVLBTNS_ANIMATION_FRAME_SIZE.right = 280;
	LVLBTNS_ANIMATION_FRAME_SIZE.bottom = 110;
	RAW_LVLBTNS = new AnimatedSprite("Data\\Sprites\\LVLBTNS.bmp","Data\\Sprites\\LVLBTNSM.bmp",LVLBTNS_ANIMATION_FRAME_SIZE,20);

	RECT OPTBTNS_ANIMATION_FRAME_SIZE;
	OPTBTNS_ANIMATION_FRAME_SIZE.top = OPTBTNS_ANIMATION_FRAME_SIZE.left = 0;
	OPTBTNS_ANIMATION_FRAME_SIZE.right = OPTBTNS_ANIMATION_FRAME_SIZE.bottom = 31;
	RAW_OPTBTNS = new AnimatedSprite("Data\\Sprites\\SBTNS.bmp","Data\\Sprites\\SBTNSM.bmp",OPTBTNS_ANIMATION_FRAME_SIZE,4);

	RECT SPIKE1_ANIMATION_FRAME_SIZE;
	SPIKE1_ANIMATION_FRAME_SIZE.top = SPIKE1_ANIMATION_FRAME_SIZE.left = 0;
	SPIKE1_ANIMATION_FRAME_SIZE.right = 105;
	SPIKE1_ANIMATION_FRAME_SIZE.bottom = 50;
	RAW_SPIKE1_ANIMATION = new AnimatedSprite("Data\\Sprites\\SP1.bmp","Data\\Sprites\\SP1M.bmp",SPIKE1_ANIMATION_FRAME_SIZE,4);

	RECT PDAL_ANIMATION_FRAME_SIZE;
	PDAL_ANIMATION_FRAME_SIZE.top = PDAL_ANIMATION_FRAME_SIZE.left = 0;
	PDAL_ANIMATION_FRAME_SIZE.right = PDAL_ANIMATION_FRAME_SIZE.bottom = 200;
	RAW_PDAL_ANIMATION = new AnimatedSprite("Data\\Sprites\\PDAL.bmp","Data\\Sprites\\PDALM.bmp",PDAL_ANIMATION_FRAME_SIZE,24);
	RAW_PDAL2_ANIMATION = new AnimatedSprite("Data\\Sprites\\PDAL2.bmp","Data\\Sprites\\PDAL2M.bmp",PDAL_ANIMATION_FRAME_SIZE,24);

	RECT PDAR_ANIMATION_FRAME_SIZE;
	PDAR_ANIMATION_FRAME_SIZE.top = PDAR_ANIMATION_FRAME_SIZE.left = 0;
	PDAR_ANIMATION_FRAME_SIZE.right = PDAR_ANIMATION_FRAME_SIZE.bottom = 200;
	RAW_PDAR_ANIMATION = new AnimatedSprite("Data\\Sprites\\PDAR.bmp","Data\\Sprites\\PDARM.bmp",PDAR_ANIMATION_FRAME_SIZE,24);
	RAW_PDAR2_ANIMATION = new AnimatedSprite("Data\\Sprites\\PDAR2.bmp","Data\\Sprites\\PDAR2M.bmp",PDAR_ANIMATION_FRAME_SIZE,24);

	RECT PHPEB_ANIMATION_FRAME_SIZE;
	PHPEB_ANIMATION_FRAME_SIZE.top = PHPEB_ANIMATION_FRAME_SIZE.left = 0;
	PHPEB_ANIMATION_FRAME_SIZE.bottom = 140; PHPEB_ANIMATION_FRAME_SIZE.right = 45;
	RAW_PHPEB = new AnimatedSprite("Data\\Sprites\\PHPEB.bmp","Data\\Sprites\\PHPEBM.bmp",PHPEB_ANIMATION_FRAME_SIZE,11);
		
	RECT PHPOH_ANIMATION_FRAME_SIZE;
	PHPOH_ANIMATION_FRAME_SIZE.top = PHPOH_ANIMATION_FRAME_SIZE.left = 0;
	PHPOH_ANIMATION_FRAME_SIZE.bottom = 140; PHPOH_ANIMATION_FRAME_SIZE.right = 45;
	RAW_PHPOH = new AnimatedSprite("Data\\Sprites\\PHPOH.bmp","Data\\Sprites\\PHPOHM.bmp",PHPOH_ANIMATION_FRAME_SIZE,11);
	RAW_PHPOH2 = new AnimatedSprite("Data\\Sprites\\PHPOH2.bmp","Data\\Sprites\\PHPOH2M.bmp",PHPOH_ANIMATION_FRAME_SIZE,11);
	
	RECT PHPPW_ANIMATION_FRAME_SIZE;
	PHPPW_ANIMATION_FRAME_SIZE.top = PHPPW_ANIMATION_FRAME_SIZE.left = 0;
	PHPPW_ANIMATION_FRAME_SIZE.bottom = 39; PHPPW_ANIMATION_FRAME_SIZE.right = 40;
	RAW_PHPPW = new AnimatedSprite("Data\\Sprites\\PHPPW.bmp","Data\\Sprites\\PHPPWM.bmp",PHPPW_ANIMATION_FRAME_SIZE,6);
		
	RECT PHPROB_ANIMATION_FRAME_SIZE;
	PHPROB_ANIMATION_FRAME_SIZE.top = PHPROB_ANIMATION_FRAME_SIZE.left = 0;
	PHPROB_ANIMATION_FRAME_SIZE.bottom = 135; PHPROB_ANIMATION_FRAME_SIZE.right = 70;
	RAW_PHPROB = new AnimatedSprite("Data\\Sprites\\PHPROB.bmp","Data\\Sprites\\PHPROBM.bmp",PHPROB_ANIMATION_FRAME_SIZE,13);
	RAW_PHPROB2 = new AnimatedSprite("Data\\Sprites\\PHPROB.bmp","Data\\Sprites\\PHPROBM.bmp",PHPROB_ANIMATION_FRAME_SIZE,13);
		
	RECT PHPWEP_ANIMATION_FRAME_SIZE;
	PHPWEP_ANIMATION_FRAME_SIZE.top = PHPWEP_ANIMATION_FRAME_SIZE.left = 0;
	PHPWEP_ANIMATION_FRAME_SIZE.bottom = 33; PHPWEP_ANIMATION_FRAME_SIZE.right = 45;
	RAW_PHPWEP = new AnimatedSprite("Data\\Sprites\\PHPWEP.bmp","Data\\Sprites\\PHPWEPM.bmp",PHPWEP_ANIMATION_FRAME_SIZE,12);

	Sprite::checkForErrors();

	RAW_MENUMUSIC = new CSound("Data\\Sound\\Music\\Albino2.mp3",SOUND_MUSIC,true);
	RAW_MENU2MUSIC = new CSound("Data\\Sound\\Music\\EnterTheTrixMat.mp3",SOUND_MUSIC,true);
	RAW_LEVEL1MUSIC = new CSound("Data\\Sound\\Music\\EscapeFromOblivion.mp3",SOUND_MUSIC,true);
	RAW_LEVEL2MUSIC = new CSound("Data\\Sound\\Music\\WHDarkOmenEerie.mp3",SOUND_MUSIC,true);
	RAW_LEVEL3MUSIC = new CSound("Data\\Sound\\Music\\ChaingunInMyHands.mp3",SOUND_MUSIC,true);
	RAW_LEVEL4MUSIC = new CSound("Data\\Sound\\Music\\WHDarkOmenEerie.mp3",SOUND_MUSIC,true);
	RAW_LEVEL5MUSIC = new CSound("Data\\Sound\\Music\\Rammstein-Hitler.mp3",SOUND_MUSIC,true);
	RAW_CREDITSMUSIC = new CSound("Data\\Sound\\Music\\ComeBackFight.mp3",SOUND_MUSIC,true);
	RAW_ACTION1MUSIC = new CSound("Data\\Sound\\Music\\ChaingunInMyHands.mp3",SOUND_MUSIC,true);
	RAW_ACTION2MUSIC = new CSound("Data\\Sound\\Music\\FightToTheDeath.mp3",SOUND_MUSIC,true);
	RAW_GAMEOVERMUSIC = new CSound("Data\\Sound\\Music\\FromTheDark.mp3",SOUND_MUSIC,true);
	RAW_GAMEFINISHEDMUSIC = new CSound("Data\\Sound\\Music\\EndCredits.mp3",SOUND_MUSIC,true);
	RAW_FUNMODEMUSIC = new CSound("Data\\Sound\\Music\\CloseToTheEdge.mp3",SOUND_MUSIC,true);
	RAW_DIGITAL_TYPING = new CSound("Data\\Sound\\SFX\\DigitalTyping.wav",SOUND_MUSIC,true);

	RAW_FUNMODE1 = new CSound("Data\\Sound\\SFX\\FunMode0.wav",SOUND_SFX,false);
	RAW_FUNMODE2 = new CSound("Data\\Sound\\SFX\\FunMode1.wav",SOUND_SFX,false);
	RAW_FUNMODE3 = new CSound("Data\\Sound\\SFX\\FunMode2.wav",SOUND_SFX,false);
	RAW_FUNMODE4 = new CSound("Data\\Sound\\SFX\\FunMode3.wav",SOUND_SFX,false);
	RAW_FUNMODE5 = new CSound("Data\\Sound\\SFX\\FunMode4.wav",SOUND_SFX,false);
	RAW_FUNMODE6 = new CSound("Data\\Sound\\SFX\\FunMode5.wav",SOUND_SFX,false);
	RAW_FUNMODE7 = new CSound("Data\\Sound\\SFX\\FunMode6.wav",SOUND_SFX,false);
	RAW_FUNMODE8 = new CSound("Data\\Sound\\SFX\\FunMode7.wav",SOUND_SFX,false);
	RAW_FUNMODE9 = new CSound("Data\\Sound\\SFX\\FunMode8.wav",SOUND_SFX,false);
	RAW_FUNMODE10 = new CSound("Data\\Sound\\SFX\\FunMode9.wav",SOUND_SFX,false);
	RAW_FUNMODE11 = new CSound("Data\\Sound\\SFX\\FunMode10.wav",SOUND_SFX,false);
	RAW_FUNMODE12 = new CSound("Data\\Sound\\SFX\\FunMode11.wav",SOUND_SFX,false);

	RAW_CHANGE_POWER = new CSound("Data\\Sound\\SFX\\PowerChange.wav",SOUND_SFX,false);
	RAW_PLAYER_DEATH = new CSound("Data\\Sound\\SFX\\PlayerDeath.mp3",SOUND_SFX,false);
	RAW_BALLSOFSTEEL = new CSound("Data\\Sound\\SFX\\IGotBallsOfSteel.mp3",SOUND_SFX,false);
}

void NextFrame()
{
	g_Timer.Tick();
	float delta=g_Timer.GetTimeElapsed();
	if(g_bCheats[CHEAT_SLOWTIME])
	{
		delta/=2;
	}
	if(g_bFullScreen)
	{
		GetCursorPos( &g_Mouse.Pos );
		DEVMODEA devMode;
		EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&devMode);
		if(devMode.dmPelsHeight!=g_nViewHeight || devMode.dmPelsWidth!=g_nViewWidth)
		{
			devMode.dmPelsWidth=g_nViewWidth;
			devMode.dmPelsHeight=g_nViewHeight;
			ChangeDisplaySettingsEx(NULL,&devMode,NULL,NULL,NULL);
		}
	}
	else
	{
		POINT displacement;
		displacement.x=displacement.y=0;
		ClientToScreen(g_hWnd,&displacement);
		GetCursorPos( &g_Mouse.Pos ); 
		g_Mouse.Pos.x-=displacement.x;
		g_Mouse.Pos.y-=displacement.y;
	}
	GameLogic(delta);
	GameRender();
}

std::string MakeSpace(int count)
{
	std::string str("");
	for(register int i=0;i<count;++i)
	{
		str+=" ";
	}
	return str;
}
void GameLogic(float delta)
{
GameLogicStart:
	static int lastGamePhase=gamePhase;
	if(gamePhase!=lastGamePhase)
	{
		if(gamePhase!=GP_PLAY_ANIMATION)
		{
			g_PointOfView=Vec2(0,0);
		}
		lastGamePhase=gamePhase;
	}
	if(gamePhase==GP_FUNMODE)
	{
		static float time2=0.0f;
		if(bFunModeStartAnim)
		{
			time2+=delta;
			float delay=1.0f;
			if(funmode_StartAnimFrame==4)
			{
				delay=0.5f;
			}
			while(time2>delay)
			{
				time2-=delay;
				++funmode_StartAnimFrame;
			}
			if(funmode_StartAnimFrame>4)
			{
				funmode_StartAnimFrame=0;
				bFunModeStartAnim=false;
			}
		}

		if(!bFunModeStartAnim)
			ProcessInput(delta);

		CProjectile::UpdateAll(delta);
		CExplosion::UpdateAll(delta);

		g_Player->mEP=g_Player->mMaxEP;
		g_Player->mVelocity.y+=delta*g_cGravity;
		g_Player->update(delta);
		g_Player->update_part2();

		g_Player2->mEP=g_Player2->mMaxEP;
		g_Player2->mVelocity.y+=delta*g_cGravity;
		g_Player2->update(delta);
		g_Player2->update_part2();

		g_PointOfView=Vec2(0,0);

		if(g_Player2->bDead && !bFunModeGotAnim)
		{
			deathPlayer=2;
			bFunModeGotAnim=true;
			RAW_PLAYER_DEATH->play();
			bool soundPlayed=true;
			switch(funmode_Player1Score-funmode_Player2Score)
			{
				case 0:
					if(!bFunModePlayer[0])
					{
						bFunModePlayer[0]=true;
						RAW_FUNMODE1->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 1:
					if(!bFunModePlayer[1])
					{
						bFunModePlayer[1]=true;
						RAW_FUNMODE2->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 2:
					if(!bFunModePlayer[2])
					{
						bFunModePlayer[2]=true;
						RAW_FUNMODE3->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 3:
					if(!bFunModePlayer[3])
					{
						bFunModePlayer[3]=true;
						RAW_FUNMODE4->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 4:
					if(!bFunModePlayer[4])
					{
						bFunModePlayer[4]=true;
						RAW_FUNMODE5->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 5:
					if(!bFunModePlayer[5])
					{
						bFunModePlayer[5]=true;
						RAW_FUNMODE6->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 6:
					if(!bFunModePlayer[6])
					{
						bFunModePlayer[6]=true;
						RAW_FUNMODE7->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 7:
					if(!bFunModePlayer[7])
					{
						bFunModePlayer[7]=true;
						RAW_FUNMODE8->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				default:
					soundPlayed=false;
				break;
			}
			if(!soundPlayed)
			{
				++soundCount;
				if(soundCount>3)soundCount=0;
				switch(soundCount+8)
				{
					case 8:
						RAW_FUNMODE9->play();
					break;
					case 9:
						RAW_FUNMODE10->play();
					break;
					case 10:
						RAW_FUNMODE11->play();
					break;
					case 11:
						RAW_FUNMODE12->play();
					break;
				}
			}
			if(g_Player2->isLeft())
			{
				anim=RAW_PDAL2_ANIMATION;
				anim->SetFrame(0);
			}
			else
			{
				anim=RAW_PDAR2_ANIMATION;
				anim->SetFrame(0);
			}
			anim->mPosition=g_Player2->mPosition;
			anim->mPosition.x-=25;
			if(g_Player2->isRight())anim->mPosition.x-=30;
		}
		else if(bFunModeGotAnim && deathPlayer==1)
		{
			g_Player2->bDead=false;
			if(g_Player2->mHP<=0)
			{
				g_Player2->mHP=1;
			}
		}

		if(g_Player->bDead && !bFunModeGotAnim)
		{
			deathPlayer=1;
			bFunModeGotAnim=true;
			RAW_PLAYER_DEATH->play();
			bool soundPlayed=true;
			switch(funmode_Player2Score-funmode_Player1Score)
			{
				case 0:
					if(!bFunModePlayer[0])
					{
						bFunModePlayer[0]=true;
						RAW_FUNMODE1->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 1:
					if(!bFunModePlayer[1])
					{
						bFunModePlayer[1]=true;
						RAW_FUNMODE2->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 2:
					if(!bFunModePlayer[2])
					{
						bFunModePlayer[2]=true;
						RAW_FUNMODE3->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 3:
					if(!bFunModePlayer[3])
					{
						bFunModePlayer[3]=true;
						RAW_FUNMODE4->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 4:
					if(!bFunModePlayer[4])
					{
						bFunModePlayer[4]=true;
						RAW_FUNMODE5->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 5:
					if(!bFunModePlayer[5])
					{
						bFunModePlayer[5]=true;
						RAW_FUNMODE6->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 6:
					if(!bFunModePlayer[6])
					{
						bFunModePlayer[6]=true;
						RAW_FUNMODE7->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				case 7:
					if(!bFunModePlayer[7])
					{
						bFunModePlayer[7]=true;
						RAW_FUNMODE8->play();
					}
					else
					{
						soundPlayed=false;
					}
				break;
				default:
					soundPlayed=false;
				break;
			}
			if(!soundPlayed)
			{
				++soundCount;
				if(soundCount>3)soundCount=0;
				switch(soundCount+8)
				{
					case 8:
						RAW_FUNMODE9->play();
					break;
					case 9:
						RAW_FUNMODE10->play();
					break;
					case 10:
						RAW_FUNMODE11->play();
					break;
					case 11:
						RAW_FUNMODE12->play();
					break;
				}
			}
			if(g_Player->isLeft())
			{
				anim=RAW_PDAL_ANIMATION;
				anim->SetFrame(0);
			}
			else
			{
				anim=RAW_PDAR_ANIMATION;
				anim->SetFrame(0);
			}
			anim->mPosition=g_Player->mPosition;
			anim->mPosition.x-=25;
			if(g_Player->isRight())anim->mPosition.x-=30;
		}
		else if(bFunModeGotAnim && deathPlayer==2)
		{
			g_Player->bDead=false;
			if(g_Player->mHP<=0)
			{
				g_Player->mHP=1;
			}
		}

		static float time=0.0f;
		if(bFunModeGotAnim)
		{
			time+=delta;
			while(time>0.07f)
			{
				time-=0.07f;
				anim->SetFrame(anim->GetFrameId()+1);
			}
			if(anim->GetFrameId()>anim->GetFrameCount())
			{
				funmode_StartAnimFrame=1;
				bFunModeStartAnim=true;

				CExplosion::DeleteAll();
				CProjectile::DeleteAll();

				g_Player->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
				g_Player->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
				g_Player->mWeaponState[WEP_SHOTGUN]=WEPSTATE_ENABLED;
				g_Player->mWeaponState[WEP_BAZOOKA]=WEPSTATE_ENABLED;

				g_Player->mPosition=Vec2(300,266);
				g_Player->mFootState=FOOTSTATE_LEFTSTOP;
				g_Player->mVelocity=Vec2(0,0);
				g_Player->bDead=false;
				g_Player->mHP=g_Player->mMaxHP;

				for(register int i=0;i<WEAPONS;++i)
				{
					g_Player->mHeat[i]=0.0f;
				}

				g_Player2->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
				g_Player2->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
				g_Player2->mWeaponState[WEP_SHOTGUN]=WEPSTATE_ENABLED;
				g_Player2->mWeaponState[WEP_BAZOOKA]=WEPSTATE_ENABLED;

				g_Player2->mPosition=Vec2(619,262);
				g_Player2->mFootState=FOOTSTATE_RIGHTSTOP;
				g_Player2->mVelocity=Vec2(0,0);
				g_Player2->bDead=false;
				g_Player2->mHP=g_Player2->mMaxHP;

				for(register int i=0;i<WEAPONS;++i)
				{
					g_Player2->mHeat[i]=0.0f;
				}

				if(deathPlayer==1)
				{
					++funmode_Player2Score;
				}
				else
				{
					++funmode_Player1Score;
				}

				deathPlayer=0;
				bFunModeGotAnim=false;
			}
		}
	}
	else if(gamePhase==GP_LEVEL2_DIALOG)
	{
		std::string str("-Anonimous scientist:\"Zain you are the best machine humanity can build. You are\n");
		str+="going on a one-way trip back in the time of the Third Reich where you will try to\n";
		str+="assassinate the nazist leader\"\n";
		str+=MakeSpace(20)+"\n";
		str+="-Zain: \"Information acknowledged\"\n";
		str+=MakeSpace(20)+"\n";
		str+="-Anonimous scientist:\"We’ve  enabled the time-travelling device.Go through the\n";
		str+="device, go back in time and stop the genocide from happening\"\n";
		str+=MakeSpace(20)+"\n";
		str+="-Zain: \"Information acknowledged\"\n";
		str+=MakeSpace(20)+"\n";
		INTRO1TEXT = "";
		static float time=0.0f;
		int str_i=INTROTEXTOFFSET;
		time+=delta;

		bool wasInThere=false;

		while(time>introFrameDelay)
		{
			time-=introFrameDelay;
			++introFrame;
		}
		if(introPhase==WAIT_STATE)
		{
			if(introFrame>82)
			{
				introFrame=0;
				introPhase=FADE_IN;
			}
		}
		else if(introPhase==FADE_IN)
		{
			INTRO1COLOR=255;
			if(str_i<(int)str.size()-1)
			{
				if(introFrame>2)
				{
					introFrame=0;
					++str_i;
				}
			}
			else
			{
				static int frameToGo=0;
				frameToGo+=introFrame;
				introFrame=0;
				if(frameToGo>10)
				{
					introPhase=FADE_OUT;
				}
			}
		}
		else if(introPhase==FADE_OUT)
		{
			introFrame*=5;
			INTRO1COLOR-=introFrame;
			introFrame=0;
			if(INTRO1COLOR<0)
			{
				INTRO1COLOR=0;
				gamePhase=GP_LEVEL2_INTRO;
				g_Mouse.Hide=true;
				INTROY=250;
				INTROX=150;
				introPhase=WAIT_STATE;
				introFrame=0;
				introFrameDelay=0.0185f;
				INTROTEXTOFFSET=0;
				CSound::Music_SetFocus(RAW_DIGITAL_TYPING);
				RAW_DIGITAL_TYPING->play();
				wasInThere=true;
			}
		}
		else if(introPhase==JUST_QUIT)
		{
			INTRO1COLOR=0;
			gamePhase=GP_LEVEL2_INTRO;
			g_Mouse.Hide=true;
			INTROY=250;
			INTROX=150;
			introPhase=WAIT_STATE;
			introFrame=0;
			introFrameDelay=0.0185f;
			INTROTEXTOFFSET=0;
			CSound::Music_SetFocus(RAW_DIGITAL_TYPING);
			RAW_DIGITAL_TYPING->play();
			wasInThere=true;
		}
		for(register int i=0;i<=str_i;++i)
		{
			INTRO1TEXT+=str[i];
		}
		if(!wasInThere)
			INTROTEXTOFFSET=str_i;
	}
	else if(gamePhase==GP_LEVEL2_DIALOG2)
	{
		CSound::Music_GetFocus()->pause(true);
		std::string str;
		str+="-Radio voice:\"It looks like we accidentaly scrambled the coordonates.Don’t worry,\n";
		str+="we can fix it. Just hold on for a minute or two.\"\n"+MakeSpace(20);
		INTRO1TEXT = "";
		static float time=0.0f;
		int str_i=INTROTEXTOFFSET;
		time+=delta;
		bool wasInThere=false;

		while(time>introFrameDelay)
		{
			time-=introFrameDelay;
			++introFrame;
		}
		if(introPhase==WAIT_STATE)
		{
			if(introFrame>82)
			{
				introFrame=0;
				introPhase=FADE_IN;
			}
		}
		else if(introPhase==FADE_IN)
		{
			INTRO1COLOR=255;
			if(str_i<(int)str.size()-1)
			{
				if(introFrame>2)
				{
					introFrame=0;
					++str_i;
				}
			}
			else
			{
				static int frameToGo=0;
				frameToGo+=introFrame;
				introFrame=0;
				if(frameToGo>10)
				{
					introPhase=FADE_OUT;
				}
			}
		}
		else if(introPhase==FADE_OUT)
		{
			introFrame*=5;
			INTRO1COLOR-=introFrame;
			introFrame=0;
			if(INTRO1COLOR<0)
			{
				INTRO1COLOR=0;
				gamePhase=GP_PLAYING;
				g_Mouse.Hide=false;
				CSound::Music_SetFocus(RAW_LEVEL2MUSIC);
				RAW_LEVEL2MUSIC->play();
				wasInThere=true;
			}
		}
		else if(introPhase==JUST_QUIT)
		{
			INTRO1COLOR=0;
			gamePhase=GP_PLAYING;
			g_Mouse.Hide=false;
			CSound::Music_SetFocus(RAW_LEVEL2MUSIC);
			RAW_LEVEL2MUSIC->play();
			wasInThere=true;
		}
		for(register int i=0;i<=str_i;++i)
		{
			INTRO1TEXT+=str[i];
		}
		INTROTEXTOFFSET=str_i;
		if(wasInThere)
			goto GameLogicStart;
	}
	else if(gamePhase==GP_LEVEL5_INTRO)
	{
		std::string str("Date: \"13 January 1937\""+MakeSpace(27)+"\n");
		str+="Location: \"Hitler's Castle\""+MakeSpace(23)+"\n";
		str+="Mission Name: \"Judgement Day\"";
		INTRO1TEXT = "";
		static float time=0.0f;
		int str_i=INTROTEXTOFFSET;
		time+=delta;
		bool wasInThere=false;

		while(time>introFrameDelay)
		{
			time-=introFrameDelay;
			++introFrame;
		}
		if(introPhase==WAIT_STATE)
		{
			if(introFrame>82)
			{
				introFrame=0;
				introPhase=FADE_IN;
			}
		}
		else if(introPhase==FADE_IN)
		{
			INTRO1COLOR=255;
			if(str_i<(int)str.size()-1)
			{
				if(introFrame>2)
				{
					introFrame=0;
					++str_i;
				}
			}
			else
			{
				static int frameToGo=0;
				frameToGo+=introFrame;
				introFrame=0;
				if(frameToGo>10)
				{
					introPhase=FADE_OUT;
				}
			}
		}
		else if(introPhase==FADE_OUT)
		{
			introFrame*=5;
			INTRO1COLOR-=introFrame;
			introFrame=0;
			if(INTRO1COLOR<0)
			{
				INTRO1COLOR=0;
				gamePhase=GP_PLAYING;
				g_Mouse.Hide=false;
				CSound::Music_SetFocus(RAW_LEVEL5MUSIC);
				RAW_LEVEL5MUSIC->play();
				wasInThere=true;
			}
		}
		else if(introPhase==JUST_QUIT)
		{
			INTRO1COLOR=0;
			gamePhase=GP_PLAYING;
			g_Mouse.Hide=false;
			CSound::Music_SetFocus(RAW_LEVEL5MUSIC);
			RAW_LEVEL5MUSIC->play();
			wasInThere=true;
		}
		for(register int i=0;i<=str_i;++i)
		{
			INTRO1TEXT+=str[i];
		}
		INTROTEXTOFFSET=str_i;
		if(wasInThere)
			goto GameLogicStart;
	}
	else if(gamePhase==GP_LEVEL3_INTRO)
	{
		std::string str("Date: \"12 January 1937\""+MakeSpace(27)+"\n");
		str+="Location: \"Nazi Germany\""+MakeSpace(26)+"\n";
		str+="Mission Name: \"Infiltrator\"\n";
		INTRO1TEXT = "";
		static float time=0.0f;
		int str_i=INTROTEXTOFFSET;
		time+=delta;
		bool wasInThere=false;

		while(time>introFrameDelay)
		{
			time-=introFrameDelay;
			++introFrame;
		}
		if(introPhase==WAIT_STATE)
		{
			if(introFrame>82)
			{
				introFrame=0;
				introPhase=FADE_IN;
			}
		}
		else if(introPhase==FADE_IN)
		{
			INTRO1COLOR=255;
			if(str_i<(int)str.size()-1)
			{
				if(introFrame>2)
				{
					introFrame=0;
					++str_i;
				}
			}
			else
			{
				static int frameToGo=0;
				frameToGo+=introFrame;
				introFrame=0;
				if(frameToGo>10)
				{
					introPhase=FADE_OUT;
				}
			}
		}
		else if(introPhase==FADE_OUT)
		{
			introFrame*=5;
			INTRO1COLOR-=introFrame;
			introFrame=0;
			if(INTRO1COLOR<0)
			{
				INTRO1COLOR=0;
				gamePhase=GP_PLAYING;
				g_Mouse.Hide=false;
				CSound::Music_SetFocus(RAW_LEVEL3MUSIC);
				RAW_LEVEL3MUSIC->play();
				wasInThere=true;
			}
		}
		else if(introPhase==JUST_QUIT)
		{
			INTRO1COLOR=0;
			gamePhase=GP_PLAYING;
			g_Mouse.Hide=false;
			CSound::Music_SetFocus(RAW_LEVEL3MUSIC);
			RAW_LEVEL3MUSIC->play();
			wasInThere=true;
		}
		for(register int i=0;i<=str_i;++i)
		{
			INTRO1TEXT+=str[i];
		}
		INTROTEXTOFFSET=str_i;
		if(wasInThere)
			goto GameLogicStart;
	}
	else if(gamePhase==GP_LEVEL2_INTRO)
	{
		std::string str("Date: \"12 september 2001\""+MakeSpace(15+10)+"\nLocation: Unknown ..."+MakeSpace(19+10)+"\n");
		str+="Searching ... "+MakeSpace(27+10)+"\n";
		str+="Searching ... "+MakeSpace(27+10)+"\n";
		str+="Location found: \"Afghanistan\""+MakeSpace(21)+"\n";
		str+="Mission Name: \"Hold on\"\n";
		INTRO1TEXT = "";
		static float time=0.0f;
		int str_i=INTROTEXTOFFSET;
		time+=delta;
		bool wasInThere=false;

		while(time>introFrameDelay)
		{
			time-=introFrameDelay;
			++introFrame;
		}
		if(introPhase==WAIT_STATE)
		{
			if(introFrame>82)
			{
				introFrame=0;
				introPhase=FADE_IN;
			}
		}
		else if(introPhase==FADE_IN)
		{
			INTRO1COLOR=255;
			if(str_i<(int)str.size()-1)
			{
				if(introFrame>2)
				{
					introFrame=0;
					++str_i;
				}
			}
			else
			{
				static int frameToGo=0;
				frameToGo+=introFrame;
				introFrame=0;
				if(frameToGo>10)
				{
					introPhase=FADE_OUT;
				}
			}
		}
		else if(introPhase==FADE_OUT)
		{
			introFrame*=5;
			INTRO1COLOR-=introFrame;
			introFrame=0;
			if(INTRO1COLOR<0)
			{
				INTRO1COLOR=0;
				gamePhase=GP_LEVEL2_DIALOG2;
				INTROY=768/2-13/2;
				INTROX=0;
				introPhase=WAIT_STATE;
				introFrame=0;
				introFrameDelay=0.0185f;
				g_Mouse.Hide=true;
				INTROTEXTOFFSET=0;
				wasInThere=true;
			}
		}
		else if(introPhase==JUST_QUIT)
		{
			INTRO1COLOR=0;
			gamePhase=GP_LEVEL2_DIALOG2;
			INTROY=768/2-13/2;
			INTROX=0;
			introPhase=WAIT_STATE;
			introFrame=0;
			introFrameDelay=0.0185f;
			g_Mouse.Hide=true;
			INTROTEXTOFFSET=0;
			wasInThere=true;
		}
		for(register int i=0;i<=str_i;++i)
		{
			INTRO1TEXT+=str[i];
		}
		if(!wasInThere)
			INTROTEXTOFFSET=str_i;
	}
	else if(gamePhase==GP_LEVEL1_INTRO)
	{
		std::string str("Date: \"20 september 2054\""+MakeSpace(26)+"\nLocation: \"U.S.A\""+MakeSpace(33)+"\nMission Name: \"Timeborn\"");
		INTRO1TEXT = "";
		static float time=0.0f;
		int str_i=INTROTEXTOFFSET;
		time+=delta;
		bool wasInThere=false;

		while(time>introFrameDelay)
		{
			time-=introFrameDelay;
			++introFrame;
		}
		if(introPhase==WAIT_STATE)
		{
			if(introFrame>82)
			{
				introFrame=0;
				introPhase=FADE_IN;
			}
		}
		else if(introPhase==FADE_IN)
		{
			INTRO1COLOR=255;
			if(str_i<(int)str.size()-1)
			{
				if(introFrame>2)
				{
					introFrame=0;
					++str_i;
				}
			}
			else
			{
				static int frameToGo=0;
				frameToGo+=introFrame;
				introFrame=0;
				if(frameToGo>10)
				{
					introPhase=FADE_OUT;
				}
			}
		}
		else if(introPhase==FADE_OUT)
		{
			introFrame*=5;
			INTRO1COLOR-=introFrame;
			introFrame=0;
			if(INTRO1COLOR<0)
			{
				INTRO1COLOR=0;
				gamePhase=GP_PLAYING;
				g_Mouse.Hide=false;
				CSound::Music_SetFocus(RAW_LEVEL1MUSIC);
				RAW_LEVEL1MUSIC->play();
				wasInThere=true;
			}
		}
		else if(introPhase==JUST_QUIT)
		{
			INTRO1COLOR=0;
			gamePhase=GP_PLAYING;
			g_Mouse.Hide=false;
			CSound::Music_SetFocus(RAW_LEVEL1MUSIC);
			RAW_LEVEL1MUSIC->play();
			wasInThere=true;
		}
		for(register int i=0;i<=str_i;++i)
		{
			INTRO1TEXT+=str[i];
		}
		INTROTEXTOFFSET=str_i;
		if(wasInThere)
			goto GameLogicStart;
	}
	else if(gamePhase==GP_PLAYING)
	{
		if(currLevel==2)
		{
			g_TTimer+=delta;
			if(g_TTimer>g_TTimerLimit && !madeShake)
			{
				madeShake=true;
				MakeShake(30,30,0.1f);
			}
			if(g_TTimer>g_TTimerLimit3)
			{
				g_bGameFinished=true;
			}
			if(g_TTimer<g_TTimerLimit)
			{
				ProcessInput(delta);
			}
			else
			{
				g_Player->stopMoving();
			}
			if(g_TTimer>60.0f && !firstSave)
			{
				firstSave=true;
				CheckPoint::Save();
			}
			if(g_TTimer>90.0f && !secondSave)
			{
				secondSave=true;
				CheckPoint::Save();
			}

			static float time=0.0f;
			time+=delta;
			float delay=25.0f;
			if(time>delay)
			{
				time-=delay;
				
				if(g_Player->mPosition.x>1300 && g_Player->mPosition.x<1024*g_pBackground.size()-200-150*3-600)
				{
					AddEnemyT1(150*0+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*1+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*2+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*3+5,200,0,1024*g_pBackground.size());

					AddEnemyT1(1024*g_pBackground.size()-200-150*0,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*1,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*2,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*3,200,0,1024*g_pBackground.size());
				}
				else if(g_Player->mPosition.x<=1300)
				{
					AddEnemyT1(1024*g_pBackground.size()-200-150*0,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*1,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*2,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*3,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*4,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*5,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*6,200,0,1024*g_pBackground.size());
					AddEnemyT1(1024*g_pBackground.size()-200-150*7,200,0,1024*g_pBackground.size());
				}
				else if(g_Player->mPosition.x>=1024*g_pBackground.size()-200-150*3-600)
				{
					AddEnemyT1(150*0+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*1+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*2+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*3+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*4+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*5+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*6+5,200,0,1024*g_pBackground.size());
					AddEnemyT1(150*7+5,200,0,1024*g_pBackground.size());
				}
			}
		}
		else
		{
			ProcessInput(delta);
		}

		if(currLevel==1)
		{
			RECT R1;
			R1.left=4100;
			R1.right=4363;
			R1.top=298;
			R1.bottom=725;
			if(RectCollideRect(g_Player->CRect(),R1))
			{
				if(g_Player->mWeaponState[WEP_RIFLE]==WEPSTATE_DISABLED)
				{
					g_Player->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
				}
			}
		}
		if(currLevel==5)
		{
			bool found=false;
			for(register int i=0;i<(int)g_Stuff.size();++i)
			{
				if(g_Stuff[i]->FOT==FOT_BOSS)
				{
					found=true;
					break;
				}
			}
			if(!found)
			{
				g_Player->bDead=true;
				g_Player->bGotToDoAnimation=true;
			}
		}


		CProjectile::UpdateAll(delta);
		CExplosion::UpdateAll(delta);

		g_Player->mVelocity.y+=delta*g_cGravity;
		ApplyGravityOnBoxes(delta,g_cGravity);

		g_Player->update(delta);
		UpdateBoxes(delta);
		UpdateStuff(delta);

		if(g_bCheats[CHEAT_FIREBOY])
		{
			RECT CR=g_Player->CRect();
			int gap=25;
			CR.top-=gap;
			CR.right+=gap;
			CR.left-=gap;
			CR.bottom+=gap;
			for(register int i=0;i<(int)g_Stuff.size();++i)
			{
				if(RectCollideRect(g_Stuff[i]->Rect(),CR))
				{
					g_Stuff[i]->TakeDamage(2<<30);
				}
			}
			for(register int i=0;i<(int)g_Box.size();++i)
			{
				if(RectCollideRect(g_Box[i]->Rect(),CR))
				{
					g_Box[i]->TakeDamage(2<<30);
				}
			}
		}

		g_Player->update_part2();

		UpdateShake(delta);

		if(g_PointOfView.x<0)g_PointOfView.x=0;
		if(g_PointOfView.x>1024*(g_pBackground.size()-1))g_PointOfView.x=1024*(g_pBackground.size()-1);

		if(g_Player->bGotToDoAnimation)
		{
			if(g_Player->bDead)
			{
				g_fAnimDelay=0.07f;
				if(g_Player->isLeft())
				{
					anim=RAW_PDAL_ANIMATION;
				}
				else
				{
					anim=RAW_PDAR_ANIMATION;
				}
			}
			else if(g_Player->CurrPW==1)
			{
				g_fAnimDelay=0.15f;
				if(g_Player->isRight())
				{
					anim=RAW_RSPEED_PWUP_ANIMATION;
				}
				else
				{
					anim=RAW_LSPEED_PWUP_ANIMATION;
				}
			}
			else if(g_Player->CurrPW==2)
			{
				g_fAnimDelay=0.15f;
				if(g_Player->isRight())
				{
					anim=RAW_RARMOR_PWUP_ANIMATION;
				}
				else
				{
					anim=RAW_LARMOR_PWUP_ANIMATION;
				}
			}
			else if(g_Player->CurrPW==3)
			{
				g_fAnimDelay=0.15f;
				if(g_Player->isRight())
				{
					anim=RAW_RSTRENGTH_PWUP_ANIMATION;
				}
				else
				{
					anim=RAW_LSTRENGTH_PWUP_ANIMATION;
				}
			}
			anim->SetFrame(1);
			anim->mPosition=g_Player->mPosition;
			anim->mPosition.x-=g_PointOfView.x+25;
			anim->mPosition.y-=g_PointOfView.y;
			if(g_Player->isRight())anim->mPosition.x-=30;
			g_Player->bGotToDoAnimation=false;
			gamePhase=GP_PLAY_ANIMATION;
			if(g_Player->bDead)
			{
				RAW_PLAYER_DEATH->play();
			}
			else
			{
				RAW_CHANGE_POWER->play();
			}
		}

		if(g_bGameFinished)
		{
			StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_DISABLED;
			if(currLevel!=5)
			{
				gamePhase=GP_GAMEFINISHED;
				CSound::Music_SetFocus(RAW_GAMEFINISHEDMUSIC);
				RAW_GAMEFINISHEDMUSIC->play();
				switch(currLevel)
				{
					case 1:
						g_bUnlockedLevels[1]=true;
						UpdateLevelButtonState();
					break;
					case 2:
						g_bUnlockedLevels[2]=true;
						UpdateLevelButtonState();
					break;
					case 3:
						g_bUnlockedLevels[3]=true;
						UpdateLevelButtonState();
					break;
					case 4:
						g_bUnlockedLevels[4]=true;
						UpdateLevelButtonState();
					break;
				}
			}
			else
			{
				StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_DISABLED;
				gamePhase=GP_CREDITS_MENU;
				CSound::Music_SetFocus(RAW_CREDITSMUSIC);
				RAW_CREDITSMUSIC->play();
			}
			g_bGameFinished=false;
		}

		if(g_bCheats[CHEAT_NEVERHEAT] || g_bCheats[CHEAT_GOD])
		{
			g_Player->mHeat[g_Player->mWeaponId]=0.0f;
		}
		
		if(g_bCheats[CHEAT_ENERGY] || g_bCheats[CHEAT_GOD])
		{
			g_Player->mEP=g_Player->mMaxEP;
		}				
		
		if(g_bCheats[CHEAT_IMMORTAL] || g_bCheats[CHEAT_GOD])
		{
			g_Player->mHP=g_Player->mMaxHP;
		}	

		bool isCheater=false;
		for(register int i=0;i<MAXCHEATS;++i)
		{
			if(g_bCheats[i])
			{
				isCheater=true;
				break;
			}
		}

		if(isCheater)
		{
			const float cheaterBlinkDelay=0.5f;
			static float time=0.0f;
			time+=delta;
			while(time>cheaterBlinkDelay)
			{
				time-=cheaterBlinkDelay;
				g_bCheaterFirstColor=!g_bCheaterFirstColor;
			}
		}
	}
	else if(gamePhase==GP_MAIN_MENU)
	{
		switch(MainMenuButtons[0].State)
		{
			case BUTTON_NORMAL:
				MainMenuButtons[0].Frame=1;
			break;
			case BUTTON_CLICKED:
				MainMenuButtons[0].Frame=5;
			break;
			case BUTTON_HOVER:
				MainMenuButtons[0].Frame=3;
			break;
		}
		switch(MainMenuButtons[1].State)
		{
			case BUTTON_NORMAL:
				MainMenuButtons[1].Frame=2;
			break;
			case BUTTON_CLICKED:
				MainMenuButtons[1].Frame=6;
			break;
			case BUTTON_HOVER:
				MainMenuButtons[1].Frame=4;
			break;
		}
		switch(MainMenuButtons[2].State)
		{
			case BUTTON_NORMAL:
				MainMenuButtons[2].Frame=7;
			break;
			case BUTTON_CLICKED:
				MainMenuButtons[2].Frame=11;
			break;
			case BUTTON_HOVER:
				MainMenuButtons[2].Frame=9;
			break;
		}
		switch(MainMenuButtons[3].State)
		{
			case BUTTON_NORMAL:
				MainMenuButtons[3].Frame=8;
			break;
			case BUTTON_CLICKED:
				MainMenuButtons[3].Frame=12;
			break;
			case BUTTON_HOVER:
				MainMenuButtons[3].Frame=10;
			break;
		}
	}
	else if(gamePhase==GP_STARTGAME_MENU)
	{
		for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_LEVEL5;++i)
		{
			if(StartGameMenuButtons[i].State==BUTTON_DISABLED)
			{
				StartGameMenuButtons[i].Frame=i*4+4;
			}
			else if(StartGameMenuButtons[i].State==BUTTON_NORMAL)
			{
				StartGameMenuButtons[i].Frame=i*4+1;
			}
			else if(StartGameMenuButtons[i].State==BUTTON_HOVER)
			{
				StartGameMenuButtons[i].Frame=i*4+2;
			}
			else if(StartGameMenuButtons[i].State==BUTTON_CLICKED)
			{
				StartGameMenuButtons[i].Frame=i*4+3;
			}
		}
		switch(StartGameMenuButtons[5].State)
		{
			case BUTTON_NORMAL:
				StartGameMenuButtons[5].Frame=1;
			break;
			case BUTTON_CLICKED:
				StartGameMenuButtons[5].Frame=3;
			break;
			case BUTTON_HOVER:
				StartGameMenuButtons[5].Frame=2;
			break;
			case BUTTON_DISABLED:
				StartGameMenuButtons[5].Frame=4;
			break;
		}
		switch(StartGameMenuButtons[6].State)
		{
			case BUTTON_NORMAL:
				StartGameMenuButtons[6].Frame=5;
			break;
			case BUTTON_CLICKED:
				StartGameMenuButtons[6].Frame=7;
			break;
			case BUTTON_HOVER:
				StartGameMenuButtons[6].Frame=6;
			break;
		}
	}
	else if(gamePhase==GP_GAMEOVER)
	{
		StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_DISABLED;

		switch(GameOverButtons[GAMEOVER_MAINMENU].State)
		{
			case BUTTON_NORMAL:
				GameOverButtons[GAMEOVER_MAINMENU].Frame=1;
			break;
			case BUTTON_CLICKED:
				GameOverButtons[GAMEOVER_MAINMENU].Frame=7;
			break;
			case BUTTON_HOVER:
				GameOverButtons[GAMEOVER_MAINMENU].Frame=4;
			break;
		}
		switch(GameOverButtons[GAMEOVER_RESTART].State)
		{
			case BUTTON_NORMAL:
				GameOverButtons[GAMEOVER_RESTART].Frame=2;
			break;
			case BUTTON_CLICKED:
				GameOverButtons[GAMEOVER_RESTART].Frame=8;
			break;
			case BUTTON_HOVER:
				GameOverButtons[GAMEOVER_RESTART].Frame=5;
			break;
		}
	}
	else if(gamePhase==GP_GAMEFINISHED)
	{
		StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_DISABLED;

		switch(GameFinishedButtons[GAMEFIN_MAINMENU].State)
		{
			case BUTTON_NORMAL:
				GameFinishedButtons[GAMEFIN_MAINMENU].Frame=1;
			break;
			case BUTTON_CLICKED:
				GameFinishedButtons[GAMEFIN_MAINMENU].Frame=7;
			break;
			case BUTTON_HOVER:
				GameFinishedButtons[GAMEFIN_MAINMENU].Frame=4;
			break;
		}
		switch(GameFinishedButtons[GAMEFIN_RESTART].State)
		{
			case BUTTON_NORMAL:
				GameFinishedButtons[GAMEFIN_RESTART].Frame=2;
			break;
			case BUTTON_CLICKED:
				GameFinishedButtons[GAMEFIN_RESTART].Frame=8;
			break;
			case BUTTON_HOVER:
				GameFinishedButtons[GAMEFIN_RESTART].Frame=5;
			break;
		}
		switch(GameFinishedButtons[GAMEFIN_NEXTLEVEL].State)
		{
			case BUTTON_NORMAL:
				GameFinishedButtons[GAMEFIN_NEXTLEVEL].Frame=3;
			break;
			case BUTTON_CLICKED:
				GameFinishedButtons[GAMEFIN_NEXTLEVEL].Frame=9;
			break;
			case BUTTON_HOVER:
				GameFinishedButtons[GAMEFIN_NEXTLEVEL].Frame=6;
			break;
		}
	}
	else if(gamePhase==GP_OPTIONS_MENU)
	{
		switch(OptionsMenuButtons[OPTION_BACK].State)
		{
			case BUTTON_NORMAL:
				OptionsMenuButtons[OPTION_BACK].Frame=5;
			break;
			case BUTTON_CLICKED:
				OptionsMenuButtons[OPTION_BACK].Frame=7;
			break;
			case BUTTON_HOVER:
				OptionsMenuButtons[OPTION_BACK].Frame=6;
			break;
		}
		switch(OptionsMenuButtons[OPTION_LEFT].State)
		{
			case BUTTON_NORMAL:
				OptionsMenuButtons[OPTION_LEFT].Frame=1;
			break;
			case BUTTON_CLICKED:
				OptionsMenuButtons[OPTION_LEFT].Frame=3;
			break;
			case BUTTON_HOVER:
				OptionsMenuButtons[OPTION_LEFT].Frame=1;
			break;
		}
		switch(OptionsMenuButtons[OPTION_RIGHT].State)
		{
			case BUTTON_NORMAL:
				OptionsMenuButtons[OPTION_RIGHT].Frame=2;
			break;
			case BUTTON_CLICKED:
				OptionsMenuButtons[OPTION_RIGHT].Frame=4;
			break;
			case BUTTON_HOVER:
				OptionsMenuButtons[OPTION_RIGHT].Frame=2;
			break;
		}

		if(OptionsMenuButtons[OPTION_LEFT].State==BUTTON_CLICKED)
		{
			g_fMusicVolumeSpeed-=g_cMVAcc*delta;
		}
		else if(OptionsMenuButtons[OPTION_RIGHT].State==BUTTON_CLICKED)
		{
			g_fMusicVolumeSpeed+=g_cMVAcc*delta;
		}
		else
		{
			g_fMusicVolumeSpeed=0.0f;
		}

		if(g_fMusicVolumeSpeed>g_cMaxMVSpeed)
		{
			g_fMusicVolumeSpeed=g_cMaxMVSpeed;
		}

		g_fMusicVolume+=g_fMusicVolumeSpeed*delta;

		if(g_fMusicVolume>1000.0f)
		{
			g_fMusicVolume=1000.0f;
		}
		if(g_fMusicVolume<0.0f)
		{
			g_fMusicVolume=0.0f;
		}

		g_iMusicVolume=(int)g_fMusicVolume;
		CSound::Music_SetVolume(g_iMusicVolume);
	}
	else if(gamePhase==GP_PLAY_ANIMATION)
	{
		static float time=0.0f;
		time+=delta;

		while(time>g_fAnimDelay)
		{
			time-=g_fAnimDelay;
			anim->SetFrame(anim->GetFrameId()+1);
		}
		if(anim->GetFrameId()>anim->GetFrameCount())
		{
			if(g_Player->bDead)
			{
				if(currLevel==5)
				{
					bool found=false;
					for(register int i=0;i<(int)g_Stuff.size();++i)
					{
						if(g_Stuff[i]->FOT==FOT_BOSS)
						{
							found=true;
							break;
						}
					}
					if(!found)
					{
						g_bGameFinished=true;
						g_Player->bDead=false;
						g_Player->bGotToAttack=false;
						StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_DISABLED;
						gamePhase=GP_CREDITS_MENU;
						CSound::Music_SetFocus(RAW_CREDITSMUSIC);
						RAW_CREDITSMUSIC->play();
					}
					else
					{
						gamePhase=GP_GAMEOVER;
						g_Player->bDead=false;
						CSound::Music_SetFocus(RAW_GAMEOVERMUSIC);
						RAW_GAMEOVERMUSIC->play();
						StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_DISABLED;
						g_bGameFinished=false;
					}
				}
				else
				{
					gamePhase=GP_GAMEOVER;
					g_Player->bDead=false;
					CSound::Music_SetFocus(RAW_GAMEOVERMUSIC);
					RAW_GAMEOVERMUSIC->play();
					StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_DISABLED;
					g_bGameFinished=false;
				}
			}
			else
			{
				gamePhase=GP_PLAYING;
			}
			time=0.0f;
		}
	}
	else if(gamePhase==GP_CREDITS_MENU)
	{
		const int creditsScroolingSpeed=100;
		g_PointOfView.y+=delta*creditsScroolingSpeed;
		if(g_CreditsData[g_CreditsData.size()-1]->PosY-(int)g_PointOfView.y<-40)
			g_PointOfView=Vec2(0,0);
	}

	g_Mouse.ElapsedTime+=delta;
	if(g_Mouse.ElapsedTime>g_Mouse.TimeWait)
	{
		g_Mouse.IsUsed=false;
	}
}

void ProcessInput(float delta)
{
	static UCHAR pKeyBuffer[ 256 ];
	bool bEventOccured=false;

    if ( !GetKeyboardState( pKeyBuffer ) ) throw FatalError("Failed to get keyboard input");

	if(gamePhase==GP_PLAYING)
	{
		if( (pKeyBuffer[ VK_UP    ] & 0xF0) && g_Player->mYCollide ) 	
		{
			g_Player->jump();
			bEventOccured=true;
		}
		if( (pKeyBuffer[ VK_LEFT  ] & 0xF0) && g_Player->mYCollide )							
		{
			g_Player->leftGroundMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ VK_LEFT  ] & 0xF0) )							
		{
			g_Player->leftAirMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ VK_RIGHT ] & 0xF0) && g_Player->mYCollide )
		{
			g_Player->rightGroundMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ VK_RIGHT ] & 0xF0) )
		{
			g_Player->rightAirMove();
			bEventOccured=true;
		}

		if(!bEventOccured)
			g_Player->stopMoving();

		if( (pKeyBuffer['W'] & 0xF0) && (pKeyBuffer['D'] & 0xF0) )			g_Player->attackWD();
		else if( (pKeyBuffer['S'] & 0xF0) && (pKeyBuffer['D'] & 0xF0) )		g_Player->attackDS();
		else if( (pKeyBuffer['S'] & 0xF0) && (pKeyBuffer['A'] & 0xF0) )		g_Player->attackSA();
		else if( (pKeyBuffer['W'] & 0xF0) && (pKeyBuffer['A'] & 0xF0) )		g_Player->attackAW();
		else if( pKeyBuffer['W'] & 0xF0 )									g_Player->attackW();
		else if( pKeyBuffer['A'] & 0xF0 )									g_Player->attackA();
		else if( pKeyBuffer['S'] & 0xF0 )									g_Player->attackS();
		else if( pKeyBuffer['D'] & 0xF0 )									g_Player->attackD();

		if( (pKeyBuffer['1'] & 0xF0) && (pKeyBuffer[VK_LSHIFT] & 0xF0) )		g_Player->PW_Strength();
		else if( (pKeyBuffer['2'] & 0xF0) && (pKeyBuffer[VK_LSHIFT] & 0xF0) )	g_Player->PW_Armor();
		else if( (pKeyBuffer['3'] & 0xF0) && (pKeyBuffer[VK_LSHIFT] & 0xF0) )	g_Player->PW_Speed();
		else if( pKeyBuffer['1'] & 0xF0 )										g_Player->getPistol();
		else if( pKeyBuffer['2'] & 0xF0 )										g_Player->getRifle();
		else if( pKeyBuffer['3'] & 0xF0 )										g_Player->getShotgun();
		else if( pKeyBuffer['4'] & 0xF0 )										g_Player->getBazooka();
	}
	else if(gamePhase==GP_FUNMODE)
	{
		if( (pKeyBuffer[ VK_UP    ] & 0xF0) && g_Player->mYCollide ) 	
		{
			g_Player->jump();
			bEventOccured=true;
		}
		if( (pKeyBuffer[ VK_LEFT  ] & 0xF0) && g_Player->mYCollide )							
		{
			g_Player->leftGroundMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ VK_LEFT  ] & 0xF0) )							
		{
			g_Player->leftAirMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ VK_RIGHT ] & 0xF0) && g_Player->mYCollide )
		{
			g_Player->rightGroundMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ VK_RIGHT ] & 0xF0) )
		{
			g_Player->rightAirMove();
			bEventOccured=true;
		}

		if(!bEventOccured)
			g_Player->stopMoving();

		if( (pKeyBuffer['T'] & 0xF0) && (pKeyBuffer['H'] & 0xF0) )			g_Player->attackWD();
		else if( (pKeyBuffer['G'] & 0xF0) && (pKeyBuffer['H'] & 0xF0) )		g_Player->attackDS();
		else if( (pKeyBuffer['G'] & 0xF0) && (pKeyBuffer['F'] & 0xF0) )		g_Player->attackSA();
		else if( (pKeyBuffer['T'] & 0xF0) && (pKeyBuffer['F'] & 0xF0) )		g_Player->attackAW();
		else if( pKeyBuffer['T'] & 0xF0 )									g_Player->attackW();
		else if( pKeyBuffer['F'] & 0xF0 )									g_Player->attackA();
		else if( pKeyBuffer['G'] & 0xF0 )									g_Player->attackS();
		else if( pKeyBuffer['H'] & 0xF0 )									g_Player->attackD();


		bEventOccured=false;
		if( (pKeyBuffer[ 'I'    ] & 0xF0) && g_Player2->mYCollide ) 	
		{
			g_Player2->jump();
			bEventOccured=true;
		}
		if( (pKeyBuffer[ 'J'  ] & 0xF0) && g_Player2->mYCollide )							
		{
			g_Player2->leftGroundMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ 'J'  ] & 0xF0) )							
		{
			g_Player2->leftAirMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ 'L' ] & 0xF0) && g_Player2->mYCollide )
		{
			g_Player2->rightGroundMove();
			bEventOccured=true;
		}
		else if( (pKeyBuffer[ 'L' ] & 0xF0) )
		{
			g_Player2->rightAirMove();
			bEventOccured=true;
		}

		if(!bEventOccured)
			g_Player2->stopMoving();

		if( (pKeyBuffer['W'] & 0xF0) && (pKeyBuffer['D'] & 0xF0) )			g_Player2->attackWD();
		else if( (pKeyBuffer['D'] & 0xF0) && (pKeyBuffer['S'] & 0xF0) )		g_Player2->attackDS();
		else if( (pKeyBuffer['S'] & 0xF0) && (pKeyBuffer['A'] & 0xF0) )		g_Player2->attackSA();
		else if( (pKeyBuffer['W'] & 0xF0) && (pKeyBuffer['A'] & 0xF0) )		g_Player2->attackAW();
		else if( pKeyBuffer['W'] & 0xF0 )									g_Player2->attackW();
		else if( pKeyBuffer['A'] & 0xF0 )									g_Player2->attackA();
		else if( pKeyBuffer['S'] & 0xF0 )									g_Player2->attackS();
		else if( pKeyBuffer['D'] & 0xF0 )									g_Player2->attackD();
	}
}

void GameRender()
{
	g_BBuffer->reset();
	HDC hdc=g_BBuffer->getDC();

	if(gamePhase==GP_LEVEL2_DIALOG2 || gamePhase==GP_LEVEL2_DIALOG || gamePhase==GP_LEVEL1_INTRO || gamePhase==GP_LEVEL2_INTRO || gamePhase==GP_LEVEL3_INTRO || gamePhase==GP_LEVEL4_INTRO || gamePhase==GP_LEVEL5_INTRO)
	{
		HBRUSH oldBrush=(HBRUSH)SelectObject(hdc,CreateSolidBrush(RGB(0,0,0)));
		Rectangle(hdc,0,0,1024,768);
		SelectObject(hdc,oldBrush);

		int oldColor = SetTextColor(hdc,RGB(INTRO1COLOR,INTRO1COLOR,INTRO1COLOR));
		int oldBkMode = SetBkMode(hdc,TRANSPARENT);
		HFONT oldFont;
		if(GP_LEVEL2_DIALOG==gamePhase || GP_LEVEL2_DIALOG2==gamePhase)
		{
			oldFont = (HFONT)SelectObject(hdc,g_hFontLevel2Dialog);
		}
		else
		{
			oldFont = (HFONT)SelectObject(hdc,g_hFontNormal);
		}
		int PosY=INTROY,posi=0;
		std::string str="";
		while(posi<(int)INTRO1TEXT.size())
		{
			str="";
			while(INTRO1TEXT[posi]!='\n')
			{
				str+=INTRO1TEXT[posi];
				++posi;
				if(posi>=(int)INTRO1TEXT.size())break;
			}
			TextOut(hdc,INTROX,PosY,str.c_str(),str.size());
			++posi;
			if(GP_LEVEL2_DIALOG==gamePhase || GP_LEVEL2_DIALOG2==gamePhase)
			{
				PosY+=15;
			}
			else
			{
				PosY+=65;
			}
		}
		SelectObject(hdc,oldFont);
		SetBkMode(hdc,oldBkMode);
		SetTextColor(hdc,oldColor);
	}
	else if(gamePhase==GP_PLAYING || gamePhase==GP_PLAY_ANIMATION)
	{
		//screen rectangle calculation
		RECT RS;
		RS.left=(int)g_PointOfView.x;
		RS.right=RS.left+1024;
		RS.top=(int)g_PointOfView.y;
		RS.bottom=RS.top+768;

		
		//background rendering 
		if(!g_bLevelIsParallax)
		{
			for(register int i=0;i<(int)g_pBackground.size();++i)
			{
				RECT R;
				R.left=i*1024;
				R.right=R.left+1024;
				R.top=0;
				R.bottom=R.top+768;
				if(RectCollideRect(R,RS))
				{
					g_pBackground[i]->Paint(hdc,i*1024-(int)g_PointOfView.x,-(int)g_PointOfView.y);
				}
			}
		}
		else
		{
			int x=(int)(g_PointOfView.x/g_ParallaxBackgroundDist)%(g_ParallaxBackground.size()*1024);
			int bknumber=x/1024;
			x%=1024;
			g_ParallaxBackground[bknumber]->Paint(hdc,-x,0);
			++bknumber;
			if(bknumber>(int)g_ParallaxBackground.size()-1)
				bknumber=0;
			g_ParallaxBackground[bknumber]->Paint(hdc,-x+1024,0);

			for(register int i=0;i<g_iNumberOfPlanes;++i)
			{
				x=(int)(g_PointOfView.x/g_ParallaxPlanesDist[i])%(g_ParallaxPlanes[i].size()*1024);
				int bknumber=x/1024;
				x%=1024;
				if(i==1)
				{
					g_ParallaxPlanes[i][bknumber]->mPosition=Vec2(-x,548);
				}
				else
				{
					g_ParallaxPlanes[i][bknumber]->mPosition=Vec2(-x,0);
				}
				g_ParallaxPlanes[i][bknumber]->draw(hdc);
				++bknumber;
				if(bknumber>(int)g_ParallaxPlanes[i].size()-1)
					bknumber=0;
				if(i==1)
				{
					g_ParallaxPlanes[i][bknumber]->mPosition=Vec2(-x+1024,548);
				}
				else
				{
					g_ParallaxPlanes[i][bknumber]->mPosition=Vec2(-x+1024,0);
				}
				g_ParallaxPlanes[i][bknumber]->draw(hdc);
			}
		}

		//projectile rendering
		CProjectile::DrawAll(hdc);

		//stuff rendering
		for(register int i = 0 ; i < (int) g_Stuff.size();++i)
		{
			if(RectCollideRect(RS,g_Stuff[i]->Rect()))
			{
				g_Stuff[i]->Draw(hdc);
			}
		}
		for(register int i = 0 ; i < (int) g_Box.size();++i)
		{
			if(RectCollideRect(RS,g_Box[i]->Rect()))
			{
				g_Box[i]->Draw(hdc);
			}
		}

		//drawing player

		if(gamePhase==GP_PLAY_ANIMATION)
		{
			anim->draw(hdc);
		}
		else
		{
			if(currLevel==2)
			{
				if(g_TTimer<g_TTimerLimit2)
				{
					g_Player->draw(hdc);
				}
			}
			else
			{
				g_Player->draw(hdc);
			}
		}

		CExplosion::DrawAll(hdc);

		int k=1;
		while(g_Player->mHeat[g_Player->mWeaponId]>9.90f*k)
		{
			++k;
		}
		if(k>11){k=11;}
		RAW_PHPOH->SetFrame(k);

		k=1;
		while(g_Player->mEP>90.909090f*k)
		{
			++k;
		}
		if(k>11){k=11;}
		k=12-k;
		RAW_PHPEB->SetFrame(k);

		k=1;
		while(g_Player->mHP>77*k)
		{
			++k;
		}
		if(k>13){k=13;}
		k=14-k;
		RAW_PHPROB->SetFrame(k);

		RAW_PHPBK->draw(hdc);
		RAW_PHPEB->draw(hdc);
		RAW_PHPOH->draw(hdc);
		RAW_PHPROB->draw(hdc);

		int oldColor = SetTextColor(hdc,RGB(255,100,0));
		int oldTextAlign = SetTextAlign(hdc,TA_CENTER);
		int oldBkMode = SetBkMode(hdc,TRANSPARENT);
		HFONT oldFont = (HFONT)SelectObject(hdc,g_hFontHP);
		char str[256];
		memset(str,0,256);
		std::ostrstream converter(str,256);
		converter<<g_Player->mHP/10<<"."<<g_Player->mHP%10<<"%";
		TextOut(hdc,65,137,str,strlen(str));
		SelectObject(hdc,oldFont);
		SetBkMode(hdc,oldBkMode);
		SetTextAlign(hdc,oldTextAlign);
		SetTextColor(hdc,oldColor);

		RAW_PHPPW->mPosition=Vec2(5,150);
		if(g_Player->CurrPW==3)
		{
			RAW_PHPPW->SetFrame(4);
		}
		else
		{
			RAW_PHPPW->SetFrame(1);
		}
		RAW_PHPPW->draw(hdc);

		RAW_PHPPW->mPosition=Vec2(45,150);
		if(g_Player->CurrPW==2)
		{
			RAW_PHPPW->SetFrame(5);
		}
		else
		{
			RAW_PHPPW->SetFrame(2);
		}
		RAW_PHPPW->draw(hdc);

		RAW_PHPPW->mPosition=Vec2(85,150);
		if(g_Player->CurrPW==1)
		{
			RAW_PHPPW->SetFrame(6);
		}
		else
		{
			RAW_PHPPW->SetFrame(3);
		}
		RAW_PHPPW->draw(hdc);

		int gap=10;
		RAW_PHPWEP->mPosition=Vec2(115+gap,15);
		if(g_Player->mWeaponState[WEP_PISTOL]==WEPSTATE_DISABLED)
		{
			RAW_PHPWEP->SetFrame(9);
		}
		else if(g_Player->mWeaponState[WEP_PISTOL]==WEPSTATE_CHOOSED)
		{
			RAW_PHPWEP->SetFrame(5);
		}
		else if(g_Player->mWeaponState[WEP_PISTOL]==WEPSTATE_ENABLED)
		{
			RAW_PHPWEP->SetFrame(1);
		}
		RAW_PHPWEP->draw(hdc);

		RAW_PHPWEP->mPosition=Vec2(125+gap,47);
		if(g_Player->mWeaponState[WEP_RIFLE]==WEPSTATE_DISABLED)
		{
			RAW_PHPWEP->SetFrame(10);
		}
		else if(g_Player->mWeaponState[WEP_RIFLE]==WEPSTATE_CHOOSED)
		{
			RAW_PHPWEP->SetFrame(6);
		}
		else if(g_Player->mWeaponState[WEP_RIFLE]==WEPSTATE_ENABLED)
		{
			RAW_PHPWEP->SetFrame(2);
		}
		RAW_PHPWEP->draw(hdc);

		RAW_PHPWEP->mPosition=Vec2(125+gap,79);
		if(g_Player->mWeaponState[WEP_SHOTGUN]==WEPSTATE_DISABLED)
		{
			RAW_PHPWEP->SetFrame(11);
		}
		else if(g_Player->mWeaponState[WEP_SHOTGUN]==WEPSTATE_CHOOSED)
		{
			RAW_PHPWEP->SetFrame(7);
		}
		else if(g_Player->mWeaponState[WEP_SHOTGUN]==WEPSTATE_ENABLED)
		{
			RAW_PHPWEP->SetFrame(3);
		}
		RAW_PHPWEP->draw(hdc);

		RAW_PHPWEP->mPosition=Vec2(115+gap,111);
		if(g_Player->mWeaponState[WEP_BAZOOKA]==WEPSTATE_DISABLED)
		{
			RAW_PHPWEP->SetFrame(12);
		}
		else if(g_Player->mWeaponState[WEP_BAZOOKA]==WEPSTATE_CHOOSED)
		{
			RAW_PHPWEP->SetFrame(8);
		}
		else if(g_Player->mWeaponState[WEP_BAZOOKA]==WEPSTATE_ENABLED)
		{
			RAW_PHPWEP->SetFrame(4);
		}
		RAW_PHPWEP->draw(hdc);

		bool isCheater=false;
		for(register int i=0;i<MAXCHEATS;++i)
		{
			if(g_bCheats[i])
			{
				isCheater=true;
				break;
			}
		}

		if(isCheater)
		{
			COLORREF col;
			if(g_bCheaterFirstColor)
			{
				col=RGB(255,0,0);
			}
			else
			{
				col=RGB(0,0,255);
			}
			int oldColor=SetTextColor(hdc,col);
			HFONT oldFont=(HFONT)SelectObject(hdc,g_hFontCheater);
			int oldBkMode=SetBkMode(hdc,TRANSPARENT);
			TextOut(hdc,15,195,"CHEATER",7);
			SetBkMode(hdc,oldBkMode);
			SelectObject(hdc,oldFont);
			SetTextColor(hdc,oldColor);
		}

		if(currLevel==5)
		{
			bool found=false;
			Boss *pBoss;
			for(register int i=0;i<(int)g_Stuff.size();++i)
			{
				if(g_Stuff[i]->FOT==FOT_BOSS)
				{
					pBoss=(Boss*)g_Stuff[i];
					found=true;
					break;
				}
			}
			int k;
			if(found)
			{
				k=0;
				while(k*(BOSSHP/50.0f)<pBoss->mHP)++k;
				--k;
				if(k<1)k=1;
			}
			else
			{
				k=1;
			}
			RAW_BOSSHP->SetFrame(51-k);
			RAW_BOSSHP->draw(hdc);
		}

		if(currLevel==2)
		{
			if(g_TTimerLimit-g_TTimer>0.0f)
			{
				float time=g_TTimerLimit-g_TTimer;
				int oldColor = SetTextColor(hdc,RGB(255,100,0));
				int oldTextAlign = SetTextAlign(hdc,TA_CENTER);
				int oldBkMode = SetBkMode(hdc,TRANSPARENT);
				HFONT oldFont = (HFONT)SelectObject(hdc,g_hFontTimer);
				char str[256];
				memset(str,0,256);
				std::ostrstream converter2(str,256);
				converter2<<floor((double)time)<<" seconds";
				TextOut(hdc,512,10,str,strlen(str));
				SelectObject(hdc,oldFont);
				SetBkMode(hdc,oldBkMode);
				SetTextAlign(hdc,oldTextAlign);
				SetTextColor(hdc,oldColor);
			}
		}
	}
	else if(gamePhase==GP_GAMEOVER)
	{
		RAW_GOBK.Paint(hdc,0,0);

		for(register int i=0;i<2;++i)
		{
			RAW_GOFBTNS->mPosition=Vec2(GameOverButtons[i].Pos.x,GameOverButtons[i].Pos.y);
			RAW_GOFBTNS->SetFrame(GameOverButtons[i].Frame);
			RAW_GOFBTNS->draw(hdc);
		}
	}
	else if(gamePhase==GP_GAMEFINISHED)
	{
		RAW_GFBK.Paint(hdc,0,0);

		for(register int i=0;i<3;++i)
		{
			RAW_GOFBTNS->mPosition=Vec2(GameFinishedButtons[i].Pos.x,GameFinishedButtons[i].Pos.y);
			RAW_GOFBTNS->SetFrame(GameFinishedButtons[i].Frame);
			RAW_GOFBTNS->draw(hdc);
		}
	}
	else if(gamePhase==GP_MAIN_MENU)
	{
		RAW_MAINMENUBK.Paint(hdc,0,0);

		for(register int i=0;i<4;++i)
		{
			RAW_MAIN_MENU_BTNS->mPosition=Vec2(MainMenuButtons[i].Pos.x,MainMenuButtons[i].Pos.y);
			RAW_MAIN_MENU_BTNS->SetFrame(MainMenuButtons[i].Frame);
			RAW_MAIN_MENU_BTNS->draw(hdc);
		}
	}
	else if(gamePhase==GP_STARTGAME_MENU)
	{
		RAW_MAINMENUNLBK.Paint(hdc,0,0);

		for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_LEVEL4;++i)
		{
			RAW_LVLBTNS->mPosition=Vec2(StartGameMenuButtons[i].Pos.x,StartGameMenuButtons[i].Pos.y);
			RAW_LVLBTNS->SetFrame(StartGameMenuButtons[i].Frame);
			RAW_LVLBTNS->draw(hdc);
		}

		for(register int i=STARTGAME_CONTINUE;i<=STARTGAME_BACK;++i)
		{
			RAW_BCBTNS->mPosition=Vec2(StartGameMenuButtons[i].Pos.x,StartGameMenuButtons[i].Pos.y);
			RAW_BCBTNS->SetFrame(StartGameMenuButtons[i].Frame);
			RAW_BCBTNS->draw(hdc);
		}
	}
	else if(gamePhase==GP_CREDITS_MENU)
	{
		RAW_MAINMENUNLBK.Paint(hdc,0,0);
		int oldTextAlign=SetTextAlign(hdc,TA_CENTER);
		HFONT oldFont = (HFONT)SelectObject(hdc,g_hFont);
		int oldBkMode=SetBkMode(hdc,TRANSPARENT);
		COLORREF oldColor = SetTextColor(hdc,RGB(185,0,100));

		static bool bNormalColor=true;
		static float time=0.0f;
		const float blinkTime=10.0f;
		for(register int i=0;i<(int)g_CreditsData.size();++i)
		{
			time+=g_Timer.GetTimeElapsed();
			if(g_CreditsData[i]->Line=="Murtaza Alexandru")
			{
				while(time>blinkTime)
				{
					time-=blinkTime;
					bNormalColor=!bNormalColor;
				}

				if(!bNormalColor)
				{
					SetTextColor(hdc,RGB(255,255,0));
				}
			}

			SelectObject(hdc,*g_CreditsData[i]->pFont);
			TextOut(hdc,(int)(-g_PointOfView.x)+512,(int)(-g_PointOfView.y)+g_CreditsData[i]->PosY,g_CreditsData[i]->Line.c_str(),g_CreditsData[i]->Line.size());
			
			if(g_CreditsData[i]->Line=="Murtaza Alexandru")
			{
				if(!bNormalColor)
				{
					SetTextColor(hdc,RGB(185,0,100));
				}
			}
		}
		SetTextColor(hdc,oldColor);
		SetBkMode(hdc,oldBkMode);
		SelectObject(hdc,oldFont);
		SetTextAlign(hdc,oldTextAlign);
	}
	else if(gamePhase==GP_OPTIONS_MENU)
	{
		RAW_OPTMENUBK.Paint(hdc,0,0);

		RAW_SBK->mPosition=Vec2(410,307);
		RAW_SBK->draw(hdc);

		RAW_OPTBTNS->mPosition=Vec2(OptionsMenuButtons[OPTION_RIGHT].Pos.x,OptionsMenuButtons[OPTION_RIGHT].Pos.y);
		RAW_OPTBTNS->SetFrame(OptionsMenuButtons[OPTION_RIGHT].Frame);
		RAW_OPTBTNS->draw(hdc);

		RAW_OPTBTNS->mPosition=Vec2(OptionsMenuButtons[OPTION_LEFT].Pos.x,OptionsMenuButtons[OPTION_LEFT].Pos.y);
		RAW_OPTBTNS->SetFrame(OptionsMenuButtons[OPTION_LEFT].Frame);
		RAW_OPTBTNS->draw(hdc);

		RAW_BCBTNS->mPosition=Vec2(OptionsMenuButtons[OPTION_BACK].Pos.x,OptionsMenuButtons[OPTION_BACK].Pos.y);
		RAW_BCBTNS->SetFrame(OptionsMenuButtons[OPTION_BACK].Frame);
		RAW_BCBTNS->draw(hdc);

		int oldTextAlign = SetTextAlign(hdc,TA_CENTER);
		int oldBkMode=SetBkMode(hdc,TRANSPARENT);
		char str[256];
		memset(str,0,256);
		std::ostrstream converter(str,256);
		converter<<g_iMusicVolume/10<<"."<<g_iMusicVolume%10<<"%";
		TextOut(hdc,499,323,str,strlen(str));
		SetBkMode(hdc,oldBkMode);
		SetTextAlign(hdc,oldTextAlign);
	}
	else if(gamePhase==GP_FUNMODE)
	{
		RAW_FUNMODEBK.Paint(hdc,0,0);

		CProjectile::DrawAll(hdc);
		CExplosion::DrawAll(hdc);

		if(!bFunModeGotAnim)
		{
			g_Player->draw(hdc);
			g_Player2->draw(hdc);
		}
		else
		{
			if(deathPlayer==1)
			{
				g_Player2->draw(hdc);
				anim->draw(hdc);
			}
			else
			{
				g_Player->draw(hdc);
				anim->draw(hdc);
			}
		}

		int k=1;
		while(g_Player->mHeat[g_Player->mWeaponId]>9.90f*k)
		{
			++k;
		}
		if(k>11){k=11;}
		RAW_PHPOH->SetFrame(k);

		k=1;
		while(g_Player->mHP>77*k)
		{
			++k;
		}
		if(k>13){k=13;}
		k=14-k;
		RAW_PHPROB->SetFrame(k);

		RAW_PHPBK->draw(hdc);
		RAW_PHPOH->draw(hdc);
		RAW_PHPROB->draw(hdc);

		int oldColor = SetTextColor(hdc,RGB(255,100,0));
		int oldTextAlign = SetTextAlign(hdc,TA_CENTER);
		int oldBkMode = SetBkMode(hdc,TRANSPARENT);
		HFONT oldFont = (HFONT)SelectObject(hdc,g_hFontHP);
		char str[256];
		memset(str,0,256);
		std::ostrstream converter(str,256);
		converter<<g_Player->mHP/10<<"."<<g_Player->mHP%10<<"%";
		TextOut(hdc,65,137,str,strlen(str));
		SelectObject(hdc,oldFont);
		SetBkMode(hdc,oldBkMode);
		SetTextAlign(hdc,oldTextAlign);
		SetTextColor(hdc,oldColor);

		oldColor = SetTextColor(hdc,RGB(255,100,0));
		oldTextAlign = SetTextAlign(hdc,TA_CENTER);
		oldBkMode = SetBkMode(hdc,TRANSPARENT);
		oldFont = (HFONT)SelectObject(hdc,g_hFontNormal);
		memset(str,0,256);
		std::ostrstream converter2(str,256);
		converter2<<funmode_Player1Score<<" - "<<funmode_Player2Score;
		TextOut(hdc,512,0,str,strlen(str));
		SelectObject(hdc,oldFont);
		SetBkMode(hdc,oldBkMode);
		SetTextAlign(hdc,oldTextAlign);
		SetTextColor(hdc,oldColor);

		k=1;
		while(g_Player2->mHeat[g_Player2->mWeaponId]>9.90f*k)
		{
			++k;
		}
		if(k>11){k=11;}
		RAW_PHPOH2->SetFrame(12-k);

		k=1;
		while(g_Player2->mHP>77*k)
		{
			++k;
		}
		if(k>13){k=13;}
		k=14-k;
		RAW_PHPROB2->SetFrame(k);

		RAW_PHPBK2->draw(hdc);
		RAW_PHPOH2->draw(hdc);
		RAW_PHPROB2->draw(hdc);

		oldColor = SetTextColor(hdc,RGB(255,100,0));
		oldTextAlign = SetTextAlign(hdc,TA_CENTER);
		oldBkMode = SetBkMode(hdc,TRANSPARENT);
		oldFont = (HFONT)SelectObject(hdc,g_hFontHP);
		memset(str,0,256);
		std::ostrstream converter3(str,256);
		converter3<<g_Player2->mHP/10<<"."<<g_Player2->mHP%10<<"%";
		TextOut(hdc,65+1024-130,137,str,strlen(str));
		SelectObject(hdc,oldFont);
		SetBkMode(hdc,oldBkMode);
		SetTextAlign(hdc,oldTextAlign);
		SetTextColor(hdc,oldColor);

		if(bFunModeStartAnim)
		{
			oldColor = SetTextColor(hdc,RGB(255,100,0));
			oldTextAlign = SetTextAlign(hdc,TA_CENTER);
			oldBkMode = SetBkMode(hdc,TRANSPARENT);
			oldFont = (HFONT)SelectObject(hdc,g_hFontTitle);
			memset(str,0,256);
			std::ostrstream converter4(str,256);
			if(funmode_StartAnimFrame==1)
			{
				converter4<<"3";
			}
			else if(funmode_StartAnimFrame==2)
			{
				converter4<<"2";
			}
			else if(funmode_StartAnimFrame==3)
			{
				converter4<<"1";
			}
			else if(funmode_StartAnimFrame==4)
			{
				converter4<<"GO!";
			}
			TextOut(hdc,1024/2,768/2,str,strlen(str));
			SelectObject(hdc,oldFont);
			SetBkMode(hdc,oldBkMode);
			SetTextAlign(hdc,oldTextAlign);
			SetTextColor(hdc,oldColor);
		}
	}
	
	if(g_Mouse.IsUsed && !g_Mouse.Hide)
	{
		RAW_CURSOR->mPosition=Vec2(g_Mouse.Pos.x,g_Mouse.Pos.y);
		RAW_CURSOR->draw(hdc);
	}

	//drawing some debug variables
	/*int oldTextAlign=SetTextAlign(hdc,TA_CENTER);
	char str[256];
	memset(str,0,256);
	sprintf(str,"FPS: %d rx=%d ry=%d",g_Timer.GetFrameRate(),g_Mouse.Pos.x+(int)g_PointOfView.x,g_Mouse.Pos.y+(int)g_PointOfView.y);
	TextOut(hdc,512,20,str,strlen(str));
	SetTextAlign(hdc,oldTextAlign);*/

	g_BBuffer->present();
}

void ReleaseMemory()
{
	if(g_BBuffer!=NULL)
		delete g_BBuffer;
	if(g_Player!=NULL)
		delete g_Player;
	if(g_Player2!=NULL)
		delete g_Player2;
	Sprite::deleteAllMemory();
	CSound::SFX_DeleteAllMemory();
	CSound::Music_DeleteAllMemory();
	DeleteBoxes();
    DeleteStuff();
	CExplosion::DeleteAll();
	CProjectile::DeleteAll();
}

void AddCreditLine(int PosY, std::string Text, HFONT* pFont)
{
	static int LastPosY=0;
	CreditLine *ptr=new CreditLine;
	ptr->PosY=PosY+LastPosY;
	ptr->Line=Text;
	ptr->pFont=pFont;
	g_CreditsData.push_back(ptr);
	LastPosY=ptr->PosY;
}

void SetupGame()
{
	g_Mouse.Hide=false;

	for(register int i=0;i<MAXCHEATS;++i)
		g_bCheats[i]=false;

	RAW_BOSSHP->mPosition=Vec2(674,660);
	RAW_PHPBK->mPosition=Vec2(0,0);
	RAW_PHPEB->mPosition=Vec2(0,4);
	RAW_PHPOH->mPosition=Vec2(85,4);
	RAW_PHPROB->mPosition=Vec2(30,2);

	int gap=1024-130;
	RAW_PHPBK2->mPosition=Vec2(0+gap,0);
	RAW_PHPOH2->mPosition=Vec2(0+gap,4);
	RAW_PHPROB2->mPosition=Vec2(30+gap,2);

	g_hFontHP=CreateFont(10,10,0,0,FW_BOLD,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"Arial");
	g_hFontTitle=CreateFont(70,40,0,0,FW_HEAVY,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"Arial");
	g_hFontNormal=CreateFont(60,30,0,0,FW_BOLD,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"Arial");
	g_hFont=CreateFont(15,10,0,0,0,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"Arial");
	g_hFontCheater=CreateFont(20,10,0,0,FW_HEAVY,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"Arial");
	g_hFontTimer=CreateFont(25,20,0,0,FW_HEAVY,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"Arial");
	g_hFontLevel2Dialog=CreateFont(14,14,0,0,FW_HEAVY,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_SCRIPT,"Arial");

	SelectObject(g_BBuffer->getDC(),g_hFont); 

	const int START=768;
	const int TITLE_GAP=75;
	const int NORMAL_GAP=65;
	const int NEW_CATEGORY_GAP=100;

	AddCreditLine(START,std::string("Game Design"),&g_hFontTitle);
	AddCreditLine(TITLE_GAP,std::string("Iordache Silviu Costin"),&g_hFontNormal);

	AddCreditLine(NORMAL_GAP+NEW_CATEGORY_GAP,std::string("Programming"),&g_hFontTitle);
	AddCreditLine(TITLE_GAP,std::string("Murtaza Alexandru"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Tabacu Raul"),&g_hFontNormal);

	AddCreditLine(NORMAL_GAP+NEW_CATEGORY_GAP,std::string("Characters & Animations"),&g_hFontTitle);
	AddCreditLine(TITLE_GAP,std::string("Mitroi Ionut"),&g_hFontNormal);

	AddCreditLine(NORMAL_GAP+NEW_CATEGORY_GAP,std::string("Levels Artwork"),&g_hFontTitle);
	AddCreditLine(TITLE_GAP,std::string("Sandu Bogdan"),&g_hFontNormal);

	AddCreditLine(NORMAL_GAP+NEW_CATEGORY_GAP,std::string("Music"),&g_hFontTitle);
	AddCreditLine(TITLE_GAP,std::string("Rammstein - Hitler"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("From gamesounds.co.uk:"),&g_hFontTitle);
	AddCreditLine(TITLE_GAP,std::string("Albino 2"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Escape From Oblivion"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Dark Omen Eerie"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Come Back Fight"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Chaingun In My Hands"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Close To The Edge"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("End Credits"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Enter The Trix Mat"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Dark Omen Battle"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("From The Dark"),&g_hFontNormal);

	AddCreditLine(NORMAL_GAP+NEW_CATEGORY_GAP,std::string("Sound"),&g_hFontTitle);
	AddCreditLine(TITLE_GAP,std::string("Total Annihilation sounds"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Duke Nukem sounds"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("Dota sounds"),&g_hFontNormal);
	AddCreditLine(NORMAL_GAP,std::string("www.freesound.org"),&g_hFontNormal);

	FileCheck("Data\\Save.ini");
	std::ifstream fin("Data\\SAVE.ini");
	for(register int i=0;i<5;++i)
	{
		int x;
		fin>>x;
		if(x==1)
		{
			g_bUnlockedLevels[i]=true;
		}
		else
		{
			g_bUnlockedLevels[i]=false;
		}
	}
	fin>>g_iMusicVolume;
	int x;
	fin>>x;
	g_bSoundMute=false;
	if(x==1)
	{
		g_bSoundMute=true;
	}
	fin.close();

	CSound::Music_SetMute(g_bSoundMute);
	CSound::SFX_SetMute(g_bSoundMute);

	g_fMusicVolume=(float)g_iMusicVolume;

	MainMenuButtons[MAIN_MENU_STARTGAME].Pos.x=715;
	MainMenuButtons[MAIN_MENU_STARTGAME].Pos.y=20;
	MainMenuButtons[MAIN_MENU_STARTGAME].ZONE.left=MainMenuButtons[MAIN_MENU_STARTGAME].Pos.x;
	MainMenuButtons[MAIN_MENU_STARTGAME].ZONE.top=MainMenuButtons[MAIN_MENU_STARTGAME].Pos.y;
	MainMenuButtons[MAIN_MENU_STARTGAME].ZONE.right=MainMenuButtons[MAIN_MENU_STARTGAME].ZONE.left+300;
	MainMenuButtons[MAIN_MENU_STARTGAME].ZONE.bottom=MainMenuButtons[MAIN_MENU_STARTGAME].ZONE.top+200;
	MainMenuButtons[MAIN_MENU_STARTGAME].State=BUTTON_NORMAL;

	MainMenuButtons[MAIN_MENU_OPTIONS].Pos.x=715;
	MainMenuButtons[MAIN_MENU_OPTIONS].Pos.y=210;
	MainMenuButtons[MAIN_MENU_OPTIONS].ZONE.left=MainMenuButtons[MAIN_MENU_OPTIONS].Pos.x;
	MainMenuButtons[MAIN_MENU_OPTIONS].ZONE.top=MainMenuButtons[MAIN_MENU_OPTIONS].Pos.y;
	MainMenuButtons[MAIN_MENU_OPTIONS].ZONE.right=MainMenuButtons[MAIN_MENU_OPTIONS].ZONE.left+300;
	MainMenuButtons[MAIN_MENU_OPTIONS].ZONE.bottom=MainMenuButtons[MAIN_MENU_OPTIONS].ZONE.top+200;
	MainMenuButtons[MAIN_MENU_OPTIONS].State=BUTTON_NORMAL;

	MainMenuButtons[MAIN_MENU_CREDITS].Pos.x=715;
	MainMenuButtons[MAIN_MENU_CREDITS].Pos.y=400;
	MainMenuButtons[MAIN_MENU_CREDITS].ZONE.left=MainMenuButtons[MAIN_MENU_CREDITS].Pos.x;
	MainMenuButtons[MAIN_MENU_CREDITS].ZONE.top=MainMenuButtons[MAIN_MENU_CREDITS].Pos.y;
	MainMenuButtons[MAIN_MENU_CREDITS].ZONE.right=MainMenuButtons[MAIN_MENU_CREDITS].ZONE.left+300;
	MainMenuButtons[MAIN_MENU_CREDITS].ZONE.bottom=MainMenuButtons[MAIN_MENU_CREDITS].ZONE.top+200;
	MainMenuButtons[MAIN_MENU_CREDITS].State=BUTTON_NORMAL;

	MainMenuButtons[MAIN_MENU_EXIT].Pos.x=715;
	MainMenuButtons[MAIN_MENU_EXIT].Pos.y=590;
	MainMenuButtons[MAIN_MENU_EXIT].ZONE.left=MainMenuButtons[MAIN_MENU_EXIT].Pos.x;
	MainMenuButtons[MAIN_MENU_EXIT].ZONE.top=MainMenuButtons[MAIN_MENU_EXIT].Pos.y;
	MainMenuButtons[MAIN_MENU_EXIT].ZONE.right=MainMenuButtons[MAIN_MENU_EXIT].ZONE.left+300;
	MainMenuButtons[MAIN_MENU_EXIT].ZONE.bottom=MainMenuButtons[MAIN_MENU_EXIT].ZONE.top+200;
	MainMenuButtons[MAIN_MENU_EXIT].State=BUTTON_NORMAL;

	StartGameMenuButtons[STARTGAME_LEVEL1].Pos.x=744;
	StartGameMenuButtons[STARTGAME_LEVEL1].Pos.y=142;
	StartGameMenuButtons[STARTGAME_LEVEL2].Pos.x=744;
	StartGameMenuButtons[STARTGAME_LEVEL2].Pos.y=252;
	StartGameMenuButtons[STARTGAME_LEVEL3].Pos.x=744;
	StartGameMenuButtons[STARTGAME_LEVEL3].Pos.y=362;
	StartGameMenuButtons[STARTGAME_LEVEL4].Pos.x=744;
	StartGameMenuButtons[STARTGAME_LEVEL4].Pos.y=472;
	StartGameMenuButtons[STARTGAME_LEVEL5].Pos.x=744;
	StartGameMenuButtons[STARTGAME_LEVEL5].Pos.y=582;
	for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_LEVEL5;++i)
	{
		StartGameMenuButtons[i].ZONE.left=StartGameMenuButtons[i].Pos.x;
		StartGameMenuButtons[i].ZONE.top=StartGameMenuButtons[i].Pos.y;
		StartGameMenuButtons[i].ZONE.right=StartGameMenuButtons[i].ZONE.left+280;
		StartGameMenuButtons[i].ZONE.bottom=StartGameMenuButtons[i].ZONE.top+110;
	}
	UpdateLevelButtonState();

	StartGameMenuButtons[STARTGAME_CONTINUE].Pos.x=656;
	StartGameMenuButtons[STARTGAME_CONTINUE].Pos.y=0;
	StartGameMenuButtons[STARTGAME_CONTINUE].ZONE.left=StartGameMenuButtons[STARTGAME_CONTINUE].Pos.x;
	StartGameMenuButtons[STARTGAME_CONTINUE].ZONE.top=StartGameMenuButtons[STARTGAME_CONTINUE].Pos.y;
	StartGameMenuButtons[STARTGAME_CONTINUE].ZONE.right=StartGameMenuButtons[STARTGAME_CONTINUE].ZONE.left+370;
	StartGameMenuButtons[STARTGAME_CONTINUE].ZONE.bottom=StartGameMenuButtons[STARTGAME_CONTINUE].ZONE.top+150;
	StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_DISABLED;

	StartGameMenuButtons[STARTGAME_BACK].Pos.x=330;
	StartGameMenuButtons[STARTGAME_BACK].Pos.y=614;
	StartGameMenuButtons[STARTGAME_BACK].ZONE.left=StartGameMenuButtons[STARTGAME_BACK].Pos.x;
	StartGameMenuButtons[STARTGAME_BACK].ZONE.top=StartGameMenuButtons[STARTGAME_BACK].Pos.y;
	StartGameMenuButtons[STARTGAME_BACK].ZONE.right=StartGameMenuButtons[STARTGAME_BACK].ZONE.left+370;
	StartGameMenuButtons[STARTGAME_BACK].ZONE.bottom=StartGameMenuButtons[STARTGAME_BACK].ZONE.top+150;
	StartGameMenuButtons[STARTGAME_BACK].State=BUTTON_NORMAL;

	OptionsMenuButtons[OPTION_LEFT].Pos.x=435;
	OptionsMenuButtons[OPTION_LEFT].Pos.y=315;
	OptionsMenuButtons[OPTION_LEFT].ZONE.left=OptionsMenuButtons[OPTION_LEFT].Pos.x;
	OptionsMenuButtons[OPTION_LEFT].ZONE.top=OptionsMenuButtons[OPTION_LEFT].Pos.y;
	OptionsMenuButtons[OPTION_LEFT].ZONE.right=OptionsMenuButtons[OPTION_LEFT].ZONE.left+31;
	OptionsMenuButtons[OPTION_LEFT].ZONE.bottom=OptionsMenuButtons[OPTION_LEFT].ZONE.top+31;
	OptionsMenuButtons[OPTION_LEFT].State=BUTTON_NORMAL;

	OptionsMenuButtons[OPTION_RIGHT].Pos.x=575;
	OptionsMenuButtons[OPTION_RIGHT].Pos.y=315;
	OptionsMenuButtons[OPTION_RIGHT].ZONE.left=OptionsMenuButtons[OPTION_RIGHT].Pos.x;
	OptionsMenuButtons[OPTION_RIGHT].ZONE.top=OptionsMenuButtons[OPTION_RIGHT].Pos.y;
	OptionsMenuButtons[OPTION_RIGHT].ZONE.right=OptionsMenuButtons[OPTION_RIGHT].ZONE.left+31;
	OptionsMenuButtons[OPTION_RIGHT].ZONE.bottom=OptionsMenuButtons[OPTION_RIGHT].ZONE.top+31;
	OptionsMenuButtons[OPTION_RIGHT].State=BUTTON_NORMAL;

	OptionsMenuButtons[OPTION_BACK].Pos.x=330;
	OptionsMenuButtons[OPTION_BACK].Pos.y=614;
	OptionsMenuButtons[OPTION_BACK].ZONE.left=OptionsMenuButtons[OPTION_BACK].Pos.x;
	OptionsMenuButtons[OPTION_BACK].ZONE.top=OptionsMenuButtons[OPTION_BACK].Pos.y;
	OptionsMenuButtons[OPTION_BACK].ZONE.right=OptionsMenuButtons[OPTION_BACK].ZONE.left+370;
	OptionsMenuButtons[OPTION_BACK].ZONE.bottom=OptionsMenuButtons[OPTION_BACK].ZONE.top+150;
	OptionsMenuButtons[OPTION_BACK].State=BUTTON_NORMAL;

	GameOverButtons[GAMEOVER_RESTART].Pos.x=0;
	GameOverButtons[GAMEOVER_RESTART].Pos.y=425;
	GameOverButtons[GAMEOVER_RESTART].ZONE.left=GameOverButtons[GAMEOVER_RESTART].Pos.x;
	GameOverButtons[GAMEOVER_RESTART].ZONE.top=GameOverButtons[GAMEOVER_RESTART].Pos.y;
	GameOverButtons[GAMEOVER_RESTART].ZONE.right=GameOverButtons[GAMEOVER_RESTART].ZONE.left+340;
	GameOverButtons[GAMEOVER_RESTART].ZONE.bottom=GameOverButtons[GAMEOVER_RESTART].ZONE.top+215;
	GameOverButtons[GAMEOVER_RESTART].State=BUTTON_NORMAL;

	GameOverButtons[GAMEOVER_MAINMENU].Pos.x=680;
	GameOverButtons[GAMEOVER_MAINMENU].Pos.y=425;
	GameOverButtons[GAMEOVER_MAINMENU].ZONE.left=GameOverButtons[GAMEOVER_MAINMENU].Pos.x;
	GameOverButtons[GAMEOVER_MAINMENU].ZONE.top=GameOverButtons[GAMEOVER_MAINMENU].Pos.y;
	GameOverButtons[GAMEOVER_MAINMENU].ZONE.right=GameOverButtons[GAMEOVER_MAINMENU].ZONE.left+340;
	GameOverButtons[GAMEOVER_MAINMENU].ZONE.bottom=GameOverButtons[GAMEOVER_MAINMENU].ZONE.top+215;
	GameOverButtons[GAMEOVER_MAINMENU].State=BUTTON_NORMAL;

	GameFinishedButtons[GAMEFIN_RESTART].Pos.y=425;
	GameFinishedButtons[GAMEFIN_RESTART].ZONE.left=GameFinishedButtons[GAMEFIN_RESTART].Pos.x;
	GameFinishedButtons[GAMEFIN_RESTART].ZONE.top=GameFinishedButtons[GAMEFIN_RESTART].Pos.y;
	GameFinishedButtons[GAMEFIN_RESTART].ZONE.right=GameFinishedButtons[GAMEFIN_RESTART].ZONE.left+340;
	GameFinishedButtons[GAMEFIN_RESTART].ZONE.bottom=GameFinishedButtons[GAMEFIN_RESTART].ZONE.top+215;
	GameFinishedButtons[GAMEFIN_RESTART].State=BUTTON_NORMAL;

	GameFinishedButtons[GAMEFIN_MAINMENU].Pos.x=680;
	GameFinishedButtons[GAMEFIN_MAINMENU].Pos.y=425;
	GameFinishedButtons[GAMEFIN_MAINMENU].ZONE.left=GameFinishedButtons[GAMEFIN_MAINMENU].Pos.x;
	GameFinishedButtons[GAMEFIN_MAINMENU].ZONE.top=GameFinishedButtons[GAMEFIN_MAINMENU].Pos.y;
	GameFinishedButtons[GAMEFIN_MAINMENU].ZONE.right=GameFinishedButtons[GAMEFIN_MAINMENU].ZONE.left+340;
	GameFinishedButtons[GAMEFIN_MAINMENU].ZONE.bottom=GameFinishedButtons[GAMEFIN_MAINMENU].ZONE.top+215;
	GameFinishedButtons[GAMEFIN_MAINMENU].State=BUTTON_NORMAL;

	GameFinishedButtons[GAMEFIN_NEXTLEVEL].Pos.x=340;
	GameFinishedButtons[GAMEFIN_NEXTLEVEL].Pos.y=425;
	GameFinishedButtons[GAMEFIN_NEXTLEVEL].ZONE.left=GameFinishedButtons[GAMEFIN_NEXTLEVEL].Pos.x;
	GameFinishedButtons[GAMEFIN_NEXTLEVEL].ZONE.top=GameFinishedButtons[GAMEFIN_NEXTLEVEL].Pos.y;
	GameFinishedButtons[GAMEFIN_NEXTLEVEL].ZONE.right=GameFinishedButtons[GAMEFIN_NEXTLEVEL].ZONE.left+340;
	GameFinishedButtons[GAMEFIN_NEXTLEVEL].ZONE.bottom=GameFinishedButtons[GAMEFIN_NEXTLEVEL].ZONE.top+215;
	GameFinishedButtons[GAMEFIN_NEXTLEVEL].State=BUTTON_NORMAL;

	gamePhase=GP_MAIN_MENU;

	g_Mouse.IsUsed=true;
	g_Mouse.TimeWait=5.0f;

	CSound *pSound;
	if(g_bIsFirstMenuMusic)
	{
		pSound=RAW_MENUMUSIC;
	}
	else
	{
		pSound=RAW_MENU2MUSIC;
	}
	g_bIsFirstMenuMusic=!g_bIsFirstMenuMusic;
	CSound::Music_SetFocus(pSound);
	pSound->play();
	if(g_iMusicVolume==0)
	{
		CSound::Music_SetVolume(1);
	}
	else
	{
		CSound::Music_SetVolume(g_iMusicVolume);
	}
}

void PlayIntro()
{
	ShowWindow(g_hWnd,SW_HIDE);
	g_VideoPlayer->load("Data\\Video\\intro.wmv");
	g_VideoPlayer->setFullscreen(true);
	g_VideoPlayer->setNotify(true);
	g_VideoPlayer->setNotifyHWND(g_hWnd);
	g_VideoPlayer->play();

	static MSG msg;
	while(!g_bMovieDone)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	    {
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		}
		else
		{
			static UCHAR pKeyBuffer[ 256 ];
			
		    if ( !GetKeyboardState( pKeyBuffer ) ) throw FatalError("Failed to get keyboard input");

			if( pKeyBuffer[ VK_ESCAPE ] & 0xF0 ) 
			{
				g_VideoPlayer->unload();
				g_bMovieDone=true;
			}
		}
	}
	ShowWindow(g_hWnd,SW_SHOW);
}

void LoadRectangleData(std::string filename)
{
	FileCheck(filename);

	g_CRect.resize(0);

	RECT r;

	std::ifstream fin(filename.c_str());
	int count;
	fin>>count;
	for(register int i=0;i<count;++i)
	{
		fin>>r.left>>r.top>>r.right>>r.bottom;r.top+=5;
		g_CRect.push_back(r);
	}
	fin.close();

	RECT R;
	R.left=-1000;
	R.top=-100;
	R.right=0;
	R.bottom=1000;
	g_CRect.push_back(R);

	R.left=g_pBackground.size()*1024;
	R.top=-1000;
	R.right=R.left+1000;
	R.bottom=1000;
	g_CRect.push_back(R);

	R.left=0;
	R.top=-1000;
	R.right=g_pBackground.size()*1024;
	R.bottom=0;
	g_CRect.push_back(R);
}

void UpdateLevelButtonState()
{
	for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_LEVEL5;++i)
	{
		if(g_bUnlockedLevels[i])
		{
			StartGameMenuButtons[i].State=BUTTON_NORMAL;
		}
		else
		{
			StartGameMenuButtons[i].State=BUTTON_DISABLED;
		}
	}
}

void SaveGame()
{
	std::ofstream fout("Data\\SAVE.ini");
	for(register int i=STARTGAME_LEVEL1;i<=STARTGAME_LEVEL5;++i)
	{
		if(g_bUnlockedLevels[i])
		{
			fout<<"1 ";
		}
		else
		{
			fout<<"0 ";
		}
	}
	fout<<g_iMusicVolume<<" ";
	if(g_bSoundMute)
	{
		fout<<"1";
	}
	else
	{
		fout<<"0";
	}
	fout.close();
}

bool checkCheat(std::string cheat)
{
	if(cheat.size()>g_cheatKeyBuffer.size())
		return false;
	bool ok=true;
	int cheat_i=cheat.size()-1;
	int buffer_i=g_cheatKeyBuffer.size()-1;
	while(cheat_i>=0)
	{
		if(cheat[cheat_i]!=g_cheatKeyBuffer[buffer_i])
		{
			ok=false;
			break;
		}
		--cheat_i;
		--buffer_i;
	}
	if(ok)
	{
		g_cheatKeyBuffer="";
		return true;
	}
	return false;
}
void checkForCheat()
{
	if(gamePhase==GP_PLAYING)
	{
		if(checkCheat("WINTERDAY"))
		{
			g_bCheats[CHEAT_NEVERHEAT]=!g_bCheats[CHEAT_NEVERHEAT];
		}
		else if(checkCheat("HEAVYFIRE"))
		{
			g_bCheats[CHEAT_1HIT1KILL]=!g_bCheats[CHEAT_1HIT1KILL];
		}
		else if(checkCheat("COFFESHOP"))
		{
			g_bCheats[CHEAT_ENERGY]=!g_bCheats[CHEAT_ENERGY];
		}
		else if(checkCheat("REDBULL"))
		{
			if(g_bCheats[CHEAT_SPEED])
			{
				g_bCheats[CHEAT_SPEED]=false;
				bool wasIn=false;
				if(g_Player->CurrPW==1)
				{
					wasIn=true;
					g_Player->PW_Armor();
				}
				g_cSpeedModifier/=2;
				SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;
				if(wasIn)
				{
					g_Player->PW_Speed();
					g_Player->bGotToDoAnimation=false;
				}
			}
			else
			{
				g_bCheats[CHEAT_SPEED]=true;
				bool wasIn=false;
				if(g_Player->CurrPW==1)
				{
					wasIn=true;
					g_Player->PW_Armor();
				}
				g_cSpeedModifier*=2;
				SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;
				if(wasIn)
				{
					g_Player->PW_Speed();
					g_Player->bGotToDoAnimation=false;
				}
			}
		}
		else if(checkCheat("IAMCHUCKNORRIS"))
		{
			g_bCheats[CHEAT_GOD]=!g_bCheats[CHEAT_GOD];
		}
		else if(checkCheat("IGOTBALLSOFSTEEL"))
		{
			g_bCheats[CHEAT_IMMORTAL]=!g_bCheats[CHEAT_IMMORTAL];
			if(g_bCheats[CHEAT_IMMORTAL]==true)
			{
				RAW_BALLSOFSTEEL->play();
			}
		}
		else if(checkCheat("SCREWDRIVER"))
		{
			g_bCheats[CHEAT_FULLHP]=true;
			g_Player->mHP=g_Player->mMaxHP;
		}
		else if(checkCheat("BATTERY"))
		{
			g_Player->mEP=g_Player->mMaxEP;
			g_bCheats[CHEAT_FULLEP]=true;
		}
		else if(checkCheat("IAMBORED"))
		{
			g_bGameFinished=true;
		}
		else if(checkCheat("LIFEISCRUEL"))
		{
			g_Player->bDead=true;
			g_Player->bGotToDoAnimation=true;
		}
		else if(checkCheat("ICECOLD"))
		{
			g_bCheats[CHEAT_NOHEAT]=true;
			g_Player->mHeat[g_Player->mWeaponId]=0.0f;
		}
		else if(checkCheat("FISTOFSTRENGTH"))
		{
			g_bCheats[CHEAT_STRENGTH]=!g_bCheats[CHEAT_STRENGTH];
		}
		else if(checkCheat("FIREBOY"))
		{
			g_bCheats[CHEAT_FIREBOY]=!g_bCheats[CHEAT_FIREBOY];
		}
		else if(checkCheat("PICKMEPICKME"))
		{
			g_bCheats[CHEAT_PICKME]=!g_bCheats[CHEAT_PICKME];
		}
		else if(checkCheat("SHAKEME"))
		{
			MakeShake(30,30,0.1f);
		}
		else if(checkCheat("WATERGIRL"))
		{
			g_bCheats[CHEAT_WATERGIRL]=!g_bCheats[CHEAT_WATERGIRL];
		}
		else if(checkCheat("VOIDMASTER"))
		{
			g_bCheats[CHEAT_SLOWTIME]=!g_bCheats[CHEAT_SLOWTIME];
		}
		else if(checkCheat("UBERBULLET"))
		{
			g_bCheats[CHEAT_UBERBULLET]=!g_bCheats[CHEAT_UBERBULLET];
		}
		else if(checkCheat("WALLSHOT"))
		{
			g_bCheats[CHEAT_WALLSHOT]=!g_bCheats[CHEAT_WALLSHOT];
		}
		else if(checkCheat("DIDICHEAT"))
		{
			if(g_bCheats[CHEAT_SPEED])
			{
				bool wasIn=false;
				if(g_Player->CurrPW==1)
				{
					wasIn=true;
					g_Player->PW_Armor();
				}
				g_cSpeedModifier/=2;
				SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;
				if(wasIn)
				{

					g_Player->PW_Speed();
					g_Player->bGotToDoAnimation=false;
				}
			}
			g_bCheats[CHEAT_NEVERHEAT]=false;
			g_bCheats[CHEAT_1HIT1KILL]=false;
			g_bCheats[CHEAT_ENERGY]=false;
			g_bCheats[CHEAT_GOD]=false;
			g_bCheats[CHEAT_SPEED]=false;
			g_bCheats[CHEAT_IMMORTAL]=false;
			g_bCheats[CHEAT_STRENGTH]=false;
			g_bCheats[CHEAT_FIREBOY]=false;
			g_bCheats[CHEAT_PICKME]=false;
			g_bCheats[CHEAT_WATERGIRL]=false;
			g_bCheats[CHEAT_SLOWTIME]=false;
			g_bCheats[CHEAT_UBERBULLET]=false;
			g_bCheats[CHEAT_WALLSHOT]=false;
		}
		else if(checkCheat("IWASHERE"))
		{
			g_bCheats[CHEAT_GENERIC]=true;
			CheckPoint::Save();
		}
		else if(checkCheat("WHEREIWAS"))
		{
			g_bCheats[CHEAT_GENERIC]=true;
			CheckPoint::Load();
		}
		else if(checkCheat("ARSENAL"))
		{
			g_bCheats[CHEAT_GENERIC]=true;
			g_Player->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
			g_Player->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
			g_Player->mWeaponState[WEP_SHOTGUN]=WEPSTATE_ENABLED;
			g_Player->mWeaponState[WEP_BAZOOKA]=WEPSTATE_ENABLED;
			g_Player->mWeaponState[g_Player->mWeaponId]=WEPSTATE_CHOOSED;
		}
	}
	else if(gamePhase==GP_STARTGAME_MENU || gamePhase==GP_GAMEFINISHED || gamePhase==GP_GAMEOVER || gamePhase==GP_CREDITS_MENU || gamePhase==GP_OPTIONS_MENU || gamePhase==GP_MAIN_MENU)
	{
		if(checkCheat("FUNMODE"))
		{
			LoadFunMode();
		}
		else if(checkCheat("LAZYSUNDAY"))
		{
			g_bUnlockedLevels[0]=true;
			g_bUnlockedLevels[1]=true;
			g_bUnlockedLevels[2]=true;
			g_bUnlockedLevels[3]=true;
			g_bUnlockedLevels[4]=true;
			UpdateLevelButtonState();
		}
	}
}
void LoadLevel1()
{
	g_bGameFinished=false;
	ClearShake();

	bool wasSpeedCheat=g_bCheats[CHEAT_SPEED];

	StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_NORMAL;

	for(register int i=0;i<MAXCHEATS;++i)
		g_bCheats[i]=false;

	g_bLevelIsParallax = false;

	g_Player->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_RIFLE]=WEPSTATE_DISABLED;
	g_Player->mWeaponState[WEP_SHOTGUN]=WEPSTATE_DISABLED;
	g_Player->mWeaponState[WEP_BAZOOKA]=WEPSTATE_DISABLED;

	currLevel=1;
	INTROTEXTOFFSET=0;
	gamePhase=GP_LEVEL1_INTRO;
	introPhase=WAIT_STATE;
	introFrame=0;
	introFrameDelay=0.0185f;
	INTROY=300;
	INTROX=150;
	g_Mouse.Hide=true;

	g_pBackground.resize(0);
	DeleteBoxes();
    DeleteStuff();
	CExplosion::DeleteAll();
	CProjectile::DeleteAll();

	g_Player->mFootState=FOOTSTATE_RIGHTSTOP;
	g_Player->getPistol();
	g_Player->PWSPhase=0;
	g_Player->PW_Armor();
	g_Player->bGotToDoAnimation=false;
	g_Player->mHP=g_Player->mMaxHP;
	g_Player->mEP=g_Player->mMaxEP;

	g_PointOfView= Vec2(0,0);
	g_Player->mPosition=Vec2(200,200);
	g_Player->mVelocity=Vec2(0,0);

	if(wasSpeedCheat)
	{
		g_cSpeedModifier/=2;
		SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;
	}

	for(register int i=0;i<WEAPONS;++i)
	{
		g_Player->mHeat[i]=0.0f;
	}

	g_Mouse.Button.Left=false;
	g_Mouse.Button.Middle=false;
	g_Mouse.Button.Right=false;

	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK1);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK2);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK3);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK4);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK5);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK6);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK7);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK8);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK9);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK10);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK11);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK12);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK13);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK14);

	LoadRectangleData(std::string("Data\\Levels\\level1.dat"));

	int iBoxHP=750;
	AddBox(2501,447,true,false,iBoxHP,RAW_BOX1_ANIMATION);
	AddBox(2501,447+139,true,false,iBoxHP,RAW_BOX1_ANIMATION);
	AddBox(2904,447,true,false,iBoxHP,RAW_BOX1_ANIMATION);
	AddBox(2904,447+139,true,false,iBoxHP,RAW_BOX1_ANIMATION);

	MovingDoor *DummyDoor = AddDoor(5376,528,RAW_DOOR1_ANIMATION);

	AddDummy(5268-70+50,439-200,300,RAW_DUMMY1L_ANIMATION,DummyDoor);
	AddDummy(4931-70+50,256-200,300,RAW_DUMMY1L_ANIMATION,DummyDoor);
	AddDummy(5036+50,728-220,300,RAW_DUMMY1L_ANIMATION,DummyDoor);
	AddDummy(4147+75,256-220,300,RAW_DUMMY1R_ANIMATION,DummyDoor);

	AddPlatform(5501,768-70-190,RAW_PLATFORM1_ANIMATION);
	AddPlatform(5501,768-70-190*2,RAW_PLATFORM1_ANIMATION);

	int iFFPosY=262;
	AddFloorTile(6240,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*2,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*3,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*4,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*5,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*6,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*7,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*8,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*9,iFFPosY,RAW_FLOOR1_ANIMATION);
	AddFloorTile(6240+50*10,iFFPosY,RAW_FLOOR1_ANIMATION);

	int gap=2;
	AddSpikes(100+1024*6,287,RAW_SPIKE1_ANIMATION);
	AddSpikes(200+1024*6-gap*1,287,RAW_SPIKE1_ANIMATION);
	AddSpikes(300+1024*6-gap*2,287,RAW_SPIKE1_ANIMATION);
	AddSpikes(400+1024*6-gap*3,287,RAW_SPIKE1_ANIMATION);
	AddSpikes(500+1024*6-gap*4,287,RAW_SPIKE1_ANIMATION);
	AddSpikes(600+1024*6-gap*5,287,RAW_SPIKE1_ANIMATION);

	MovingDoor *SwitchDoor = AddDoor(7177,527,RAW_DOOR1_ANIMATION);

	AddSwitch(6144,768-40-70,SwitchDoor,RAW_SWITCH1_ANIMATION);

	AddElevator(340+1024*7,650,270,700,200,RAW_PLATFORM1_ANIMATION);

	AddSpikes(8995,725-50+3,RAW_SPIKE1_ANIMATION);
	AddSpikes(8646,725-50+3,RAW_SPIKE1_ANIMATION);
	AddSpikes(8202,725-50+3,RAW_SPIKE1_ANIMATION);
	AddSpikes(8202+100-gap,725-50+3,RAW_SPIKE1_ANIMATION);

	AddBox(9316,370,false,true,iBoxHP,RAW_BOX2_ANIMATION);

	AddSpikes(9515,658,RAW_SPIKE1_ANIMATION);
	AddSpikes(100+9515-gap*1,658,RAW_SPIKE1_ANIMATION);
	AddSpikes(200+9515-gap*2,658,RAW_SPIKE1_ANIMATION);
	AddSpikes(300+9515-gap*3,658,RAW_SPIKE1_ANIMATION);
	AddSpikes(400+9515-gap*4,658,RAW_SPIKE1_ANIMATION);

	int iBoxHP2=iBoxHP+250;
	AddBox(10640-75,350,true,false,iBoxHP2,RAW_BOX3_ANIMATION);
	AddBox(10640-75,350+151,true,false,iBoxHP2,RAW_BOX3_ANIMATION);
	AddBox(12840-75,350,true,false,iBoxHP2,RAW_BOX3_ANIMATION);
	AddBox(12840-75,350+151,true,false,iBoxHP2,RAW_BOX3_ANIMATION);

	AddElevator(13900,650,280,620,200,RAW_PLATFORM1_ANIMATION);

	AddFinishZone(13900,100,1000,20);

	gap=50;
	AddTurret(6286,378,60,60,0.1,RAW_TURRET1,700,RAW_W2_PRJ,false,6285-gap,387,6308+gap,387,6450+gap,728,6143-gap,728,1.0);
	AddTurret(6486,378,60,60,0.1,RAW_TURRET1,700,RAW_W2_PRJ,false,6285-gap+200,387,6308+gap+200,387,6450+gap+200,728,6143-gap+200,728,1.0);
	AddTurret(6686,378,60,60,0.1,RAW_TURRET1,700,RAW_W2_PRJ,false,6285-gap+400,387,6308+gap+400,387,6450+gap+400,728,6143-gap+400,728,1.0);
	
	int speed=1000;
	int minDmg=85;
	int maxDmg=95;
	AddTurret(11302,258,minDmg,maxDmg,0.1,RAW_TURRET1,speed,RAW_W2_PRJ,true,11272-gap,258,11357+gap,258,11539+gap,650,11357-gap,650,0.1);
	AddTurret(11302+200,258,minDmg,maxDmg,0.1,RAW_TURRET1,speed,RAW_W2_PRJ,true,11272-gap+200,258,11357+gap+200,258,11539+gap+200,650,11357-gap+200,650,0.1);
	AddTurret(11302+400,258,minDmg,maxDmg,0.1,RAW_TURRET1,speed,RAW_W2_PRJ,true,11272-gap+400,258,11357+gap+400,258,11539+gap+400,650,11357-gap+400,650,0.1);
	AddTurret(11302+500,258,minDmg,maxDmg,0.1,RAW_TURRET1,speed,RAW_W2_PRJ,true,11272-gap+500,258,11357+gap+500,258,11539+gap+500,650,11357-gap+500,650,0.1);
	AddTurret(11302+700,258,minDmg,maxDmg,0.1,RAW_TURRET1,speed,RAW_W2_PRJ,true,11272-gap+700,258,11357+gap+700,258,11539+gap+700,650,11357-gap+700,650,0.1);
	AddTurret(11302+900,258,minDmg,maxDmg,0.1,RAW_TURRET1,speed,RAW_W2_PRJ,true,11272-gap+900,258,11357+gap+900,258,11539+gap+900,650,11357-gap+900,650,0.1);
	
	AddRepairKit(390+1025*7,650+33,RAW_REPAIRKIT,500);

	RECT R;
	R.left=5863;
	R.top=38;
	R.right=6040;
	R.bottom=257;
	AddCheckPoint(R);

	R.left=6848;
	R.top=525;
	R.right=6944;
	R.bottom=724;
	AddCheckPoint(R);

	R.left=8192;
	R.top=46;
	R.right=8282;
	R.bottom=268;
	AddCheckPoint(R);

	R.left=10842;
	R.top=44;
	R.right=10984;
	R.bottom=647;
	AddCheckPoint(R);

	R.left=13313;
	R.top=41;
	R.right=13461;
	R.bottom=728;
	AddCheckPoint(R);

	CSound::Music_SetFocus(RAW_DIGITAL_TYPING);
	RAW_DIGITAL_TYPING->play();
	
	CheckPoint::Clear();

	g_Timer.Tick();
	g_Timer.Tick();
}

void LoadLevel2()
{
	g_bGameFinished=false;
	ClearShake();

	bool wasSpeedCheat=g_bCheats[CHEAT_SPEED];

	StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_NORMAL;

	for(register int i=0;i<MAXCHEATS;++i)
		g_bCheats[i]=false;

	g_bLevelIsParallax = true;

	g_Player->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_SHOTGUN]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_BAZOOKA]=WEPSTATE_DISABLED;

	currLevel=2;
	gamePhase=GP_LEVEL2_DIALOG;
	introPhase=WAIT_STATE;
	introFrame=0;
	introFrameDelay=0.0185f;
	g_Mouse.Hide=true;
	INTROTEXTOFFSET=0;
	INTROY=250;
	INTROX=0;

	g_TTimer=0.0f;
	madeShake=false;
	g_TTimerLimit=120.0f;
	g_TTimerLimit2=122.0f;
	g_TTimerLimit3=123.0f;
	firstSave=false;
	secondSave=false;

	g_pBackground.resize(0);
	DeleteBoxes();
    DeleteStuff();
	CExplosion::DeleteAll();
	CProjectile::DeleteAll();

	g_Player->mFootState=FOOTSTATE_RIGHTSTOP;
	g_Player->getRifle();
	g_Player->PWSPhase=0;
	g_Player->PW_Armor();
	g_Player->bGotToDoAnimation=false;
	g_Player->mHP=g_Player->mMaxHP;
	g_Player->mEP=g_Player->mMaxEP;

	g_PointOfView= Vec2(0,0);
	g_Player->mPosition=Vec2(200,200);
	g_Player->mVelocity=Vec2(0,0);

	if(wasSpeedCheat)
	{
		g_cSpeedModifier/=2;
		SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;
	}

	for(register int i=0;i<WEAPONS;++i)
	{
		g_Player->mHeat[i]=0.0f;
	}

	g_Mouse.Button.Left=false;
	g_Mouse.Button.Middle=false;
	g_Mouse.Button.Right=false;

	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK1);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK1);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK1);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK1);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK1);
	g_pBackground.push_back(&RAW_BACKGROUND_LV1BK1);

	g_ParallaxBackground.push_back(&RAW_BACKGROUND_LV2BK1);
	g_ParallaxBackground.push_back(&RAW_BACKGROUND_LV2BK2);
	g_ParallaxBackgroundDist=3.0f;

	g_ParallaxPlanes[0].push_back(RAW_PARALLAX_L2P2BK1);
	g_ParallaxPlanes[0].push_back(RAW_PARALLAX_L2P2BK2);
	g_ParallaxPlanes[0].push_back(RAW_PARALLAX_L2P2BK3);
	g_ParallaxPlanes[0].push_back(RAW_PARALLAX_L2P2BK4);
	g_ParallaxPlanesDist[0]=2.0f;

	g_ParallaxPlanes[1].push_back(RAW_PARALLAX_L2P3BK1);
	g_ParallaxPlanes[1].push_back(RAW_PARALLAX_L2P3BK2);
	g_ParallaxPlanesDist[1]=1.0f;

	g_iNumberOfPlanes=2;

	CheckPoint::Clear();

	LoadRectangleData(std::string("Data\\Levels\\level2.dat"));

	CSound::Music_GetFocus()->pause(true);
}

void LoadLevel3()
{
	g_bGameFinished=false;
	ClearShake();

	bool wasSpeedCheat=g_bCheats[CHEAT_SPEED];

	StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_NORMAL;

	for(register int i=0;i<MAXCHEATS;++i)
		g_bCheats[i]=false;

	g_bLevelIsParallax = false;

	g_Player->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_SHOTGUN]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_BAZOOKA]=WEPSTATE_DISABLED;

	currLevel=3;
	INTROTEXTOFFSET=0;
	gamePhase=GP_LEVEL3_INTRO;
	introPhase=WAIT_STATE;
	introFrame=0;
	introFrameDelay=0.0185f;
	g_Mouse.Hide=true;
	INTROTEXTOFFSET=0;
	INTROY=300;
	INTROX=150;

	g_pBackground.resize(0);
	DeleteBoxes();
    DeleteStuff();
	CExplosion::DeleteAll();
	CProjectile::DeleteAll();

	g_Player->mFootState=FOOTSTATE_RIGHTSTOP;
	g_Player->getShotgun();
	g_Player->PWSPhase=0;
	g_Player->PW_Armor();
	g_Player->bGotToDoAnimation=false;
	g_Player->mHP=g_Player->mMaxHP;
	g_Player->mEP=g_Player->mMaxEP;

	g_PointOfView= Vec2(0,0);
	g_Player->mPosition=Vec2(200,200);
	g_Player->mVelocity=Vec2(0,0);

	if(wasSpeedCheat)
	{
		g_cSpeedModifier/=2;
		SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;
	}

	for(register int i=0;i<WEAPONS;++i)
	{
		g_Player->mHeat[i]=0.0f;
	}

	g_Mouse.Button.Left=false;
	g_Mouse.Button.Middle=false;
	g_Mouse.Button.Right=false;

	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK1);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK2);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK3);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK4);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK5);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK6);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK7);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK8);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK9);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK10);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK11);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK12);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK13);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK14);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK15);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK16);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK17);
	g_pBackground.push_back(&RAW_BACKGROUND_LV3BK18);

	LoadRectangleData(std::string("Data\\Levels\\level3.dat"));

	int iBoxHP = 800;
	MovingDoor *EDoor,*Sw1Door,*Sw2Door;
	//BK2Data
	AddEnemyT1(488+1024,286,280+1024,1000+1025,NULL,true);
	AddBox(850+1024,426,true,true,iBoxHP,RAW_BOX1_ANIMATION);

	//BK3DATA
	AddEnemyT1(1024*2,286,0,0,NULL,false);
	AddEnemyT1(500+1024*2,30,0,0,0,false);
	AddBox(700+1024*2,100,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	AddBox(855+1024*2,155+30,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	AddBox(855+1024*2,30,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	//BK4DATA
	AddFloorTile(30+1024*3+50*0,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*1,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*2,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*3,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*4,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*5,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*6,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*7,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*8,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*9,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*10,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*11,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*12,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*13,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*14,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*15,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*16,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*17,370,RAW_FLOOR1_ANIMATION);
	AddFloorTile(30+1024*3+50*18,370,RAW_FLOOR1_ANIMATION);
	AddSpikes(30+1024*3+100*0-2*0,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*1-2*1,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*2-2*2,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*3-2*3,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*4-2*4,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*5-2*5,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*6-2*6,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*7-2*7,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*8-2*8,700,RAW_SPIKE1_ANIMATION);
	AddSpikes(30+1024*3+100*9-2*9,700,RAW_SPIKE1_ANIMATION);
	//BK5DATA
	AddEnemyT1(0+1024*4,100,0,0,NULL,false);
	AddEnemyT1(404+1024*4,6,0,0,NULL,false);
	EDoor = AddDoor(775+1024*4,532,RAW_DOOR1_ANIMATION);
	AddEnemyT1(572+1024*4,265,0,0,EDoor,false);
	AddEnemyT1(1000+1024*4,265,0,0,EDoor,false);
	AddEnemyT1(32+1024*4,400,0,0,EDoor,false);
	AddEnemyT1(290+1024*4,471,1024*4,1024+1024*4,EDoor,true);
	//BK6DATA
	Sw1Door = AddDoor(680+1024*5,534,RAW_DOOR1_ANIMATION);
	AddSwitch(570+1024*5,200,Sw1Door,RAW_SWITCH1_ANIMATION);
	AddEnemyT1(300+1024*5,6,280+1024*5,600+1024*5,NULL,true);
	AddEnemyT1(850+1024*5,6,0,0,NULL,false);
	//BK7DATA
	AddEnemyT1(500+1024*6,6,0,0,NULL,false);
	AddEnemyT1(800+1024*6,6,0,0,NULL,false);
	//BK8Data
	AddEnemyT1(26+1024*7,6,0,0,NULL,false);
	AddEnemyT1(290+1024*7,6,0,0,NULL,false);
	AddEnemyT1(786+1024*7,6,620+1024*7,990+1024*7,NULL,true);
	//BK9DATA
	AddEnemyT1(200+1024*8,6,74+1024*8,1000+1024*8,NULL,true);
	AddEnemyT1(700+1024*8,6,590+1024*8,1424+1024*8,NULL,true);
	AddEnemyT1(1020+1024*8,6,590+1024*8,1424+1024*8,NULL,true);
	//BK10Data
	AddEnemyT1(600+1024*9,6,0,0,NULL,false);
	AddBox(860+1024*8,6,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	//BK11Data
	AddEnemyT1(430+1024*10,6,0,0,NULL,false);
	AddBox(700+1024*10,210,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	AddBox(700+1024*10,50,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	//BK12DATA
	AddEnemyT1(10+1024*11,6,0+1024*11,1024+1024*11,NULL,true);
	AddEnemyT1(200+1024*11,6,0+1024*11,1024+1024*11,NULL,true);
	AddEnemyT1(550+1024*11,6,0+1024*11,1024+1024*11,NULL,true);
	AddEnemyT1(800+1024*11,6,0+1024*11,1024+1024*11,NULL,true);
	//BK13DAta
	AddBox(20+1024*12,6,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	AddEnemyT1(450+1024*12,6,0,0,NULL,false);
	AddEnemyT1(886+1024*12,6,0,0,NULL,false);
	Sw2Door = AddDoor(492+1024*13,524,RAW_DOOR1_ANIMATION);
	AddSwitch(650+1024*12,658,Sw2Door,RAW_SWITCH1_ANIMATION);
	//BK14Data
	AddEnemyT1(640+1024*13,6,0,0,NULL,false);
	AddBox(850+1024*13,6,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	AddBox(850+1024*13,166,true,true,iBoxHP,RAW_BOX1_ANIMATION);
	//BK15Data
	AddEnemyT1(200+1024*14,470,0+1024*14,1024+1024*14,NULL,true);
	AddEnemyT1(400+1024*14,470,0+1024*14,1024+1024*14,NULL,true);
	AddEnemyT1(600+1024*14,470,0+1024*14,1024+1024*14,NULL,true);
	AddEnemyT1(800+1024*14,470,0+1024*14,1024+1024*14,NULL,true);
	//BK16Data
	AddTurret(258+1024*15,288,60,60,0.1,RAW_TURRET1_ANIMATION,700,RAW_W2_PRJ,false,190+1024*15,299,408+1024*15,300,772+1024*15,768,1024*15,768,1.0);
	AddTurret(520+1024*15,288,60,60,0.1,RAW_TURRET1_ANIMATION,700,RAW_W2_PRJ,false,480+1024*15,299,568+1024*15,300,800+1024*15,768,176+1024*15,768,1.0);
	AddTurret(724+1024*15,288,60,60,0.1,RAW_TURRET1_ANIMATION,700,RAW_W2_PRJ,false,650+1024*15,299,780+1024*15,300,1024+1024*15,768,400+1024*15,768,1.0);
	AddEnemyT1(200+1024*15,300,0,0,NULL,false);
	AddEnemyT1(300+1024*15,300,0,0,NULL,false);
	AddEnemyT1(700+1024*15,300,0,0,NULL,false);
	AddEnemyT1(800+1024*15,300,0,0,NULL,false);
	AddEnemyT1(900+1024*15,300,0,0,NULL,false);
	//BL17DAta
	//BK18DATA
	AddEnemyT1(340+1024*17,6,0,0,NULL,false);
	AddEnemyT1(540+1024*17,6,0,0,NULL,false);
	AddFinishZone(700+1024*17,0,1000,1000);

	RECT Check; 
	Check.left = 868 * 1024*4;
	Check.top  = 514;
	Check.bottom = 1000;
	Check.right = 1000 * 1024*4;
	AddCheckPoint(Check);
	Check.left = 598 * 1024*13;
	Check.top  = 0;
	Check.bottom = 1000;
	Check.right = 650 * 1024*13;
	AddCheckPoint(Check);

	AddRepairKit(352+1024*14,336+60-10,RAW_REPAIRKIT,300);
	AddRepairKit(452+1024*14,336+60-10,RAW_REPAIRKIT,300);
	AddRepairKit(262+1024*16,271+60-15,RAW_REPAIRKIT,500);
	AddRepairKit(860+1024*5,170+70,RAW_REPAIRKIT,300);

	CSound::Music_SetFocus(RAW_DIGITAL_TYPING);
	RAW_DIGITAL_TYPING->play();
	
	CheckPoint::Clear();

	g_Timer.Tick();
	g_Timer.Tick();
}

void LoadLevel5()
{

}

void LoadLevel4()
{
	g_bGameFinished=false;
	ClearShake();

	bool wasSpeedCheat=g_bCheats[CHEAT_SPEED];

	StartGameMenuButtons[STARTGAME_CONTINUE].State=BUTTON_NORMAL;

	for(register int i=0;i<MAXCHEATS;++i)
		g_bCheats[i]=false;

	g_bLevelIsParallax = false;

	g_Player->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_SHOTGUN]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_BAZOOKA]=WEPSTATE_ENABLED;

	currLevel=5;
	gamePhase=GP_LEVEL5_INTRO;
	introPhase=WAIT_STATE;
	introFrame=0;
	introFrameDelay=0.0185f;
	g_Mouse.Hide=true;
	INTROTEXTOFFSET=0;
	INTROY=300;
	INTROX=75;

	g_pBackground.resize(0);
	DeleteBoxes();
    DeleteStuff();
	CExplosion::DeleteAll();
	CProjectile::DeleteAll();

	g_Player->mFootState=FOOTSTATE_RIGHTSTOP;
	g_Player->PWSPhase=0;
	g_Player->PW_Armor();
	g_Player->bGotToDoAnimation=false;
	g_Player->mHP=g_Player->mMaxHP;
	g_Player->mEP=g_Player->mMaxEP;
	g_Player->getRifle();
	g_PointOfView= Vec2(0,0);
	g_Player->mPosition=Vec2(150,525);
	g_Player->mVelocity=Vec2(0,0);

	if(wasSpeedCheat)
	{
		g_cSpeedModifier/=2;
		SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;
	}

	for(register int i=0;i<WEAPONS;++i)
	{
		g_Player->mHeat[i]=0.0f;
	}

	g_Mouse.Button.Left=false;
	g_Mouse.Button.Middle=false;
	g_Mouse.Button.Right=false;

	g_pBackground.push_back(&RAW_BACKGROUND_LV5BK1);
	g_pBackground.push_back(&RAW_BACKGROUND_LV5BK2);

	LoadRectangleData(std::string("Data\\Levels\\level5.dat"));

	AddBoss();
	AddTurret(859,52,16,45,0.1f,RAW_TURRET1,700,RAW_W2_PRJ,true,10000,10000,10001,10000,10001,10001,10000,10001,0.3f);
	AddTurret(1170,52,16,45,0.1f,RAW_TURRET1,700,RAW_W2_PRJ,true,10000,10000,10001,10000,10001,10001,10000,10001,0.3f);

	CheckPoint::Clear();

	g_Timer.Tick();
	g_Timer.Tick();

	CSound::Music_SetFocus(RAW_DIGITAL_TYPING);
	RAW_DIGITAL_TYPING->play();
}
void LoadFunMode()
{
	for(register int i=0;i<8;++i)
	{
		bFunModePlayer[i]=false;
	}

	funmode_StartAnimFrame=1;
	bFunModeStartAnim=true;

	funmode_Player1Score=0;
	funmode_Player2Score=0;

	gamePhase=GP_FUNMODE;

	bool wasSpeedCheat=g_bCheats[CHEAT_SPEED];

	g_pBackground.resize(0);
	DeleteBoxes();
    DeleteStuff();
	CExplosion::DeleteAll();
	CProjectile::DeleteAll();

	for(register int i=0;i<MAXCHEATS;++i)
		g_bCheats[i]=false;

	if(wasSpeedCheat)
	{
		g_cSpeedModifier/=2;
		SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;
	}

	g_Player->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_SHOTGUN]=WEPSTATE_ENABLED;
	g_Player->mWeaponState[WEP_BAZOOKA]=WEPSTATE_ENABLED;

	g_Player->mFootState=FOOTSTATE_RIGHTSTOP;
	g_Player->PWSPhase=0;
	g_Player->PW_Speed();
	g_Player->bGotToDoAnimation=false;
	g_Player->mHP=g_Player->mMaxHP;
	g_Player->mEP=g_Player->mMaxEP;
	g_Player->getRifle();
	g_Player->mPosition=Vec2(300,266);
	g_Player->mFootState=FOOTSTATE_LEFTSTOP;
	g_Player->mVelocity=Vec2(0,0);

	for(register int i=0;i<WEAPONS;++i)
	{
		g_Player->mHeat[i]=0.0f;
	}

	g_Player2->mWeaponState[WEP_PISTOL]=WEPSTATE_ENABLED;
	g_Player2->mWeaponState[WEP_RIFLE]=WEPSTATE_ENABLED;
	g_Player2->mWeaponState[WEP_SHOTGUN]=WEPSTATE_ENABLED;
	g_Player2->mWeaponState[WEP_BAZOOKA]=WEPSTATE_ENABLED;

	g_Player2->mFootState=FOOTSTATE_RIGHTSTOP;
	g_Player2->PWSPhase=0;
	g_Player2->PW_Speed();
	g_Player2->bGotToDoAnimation=false;
	g_Player2->mHP=g_Player->mMaxHP;
	g_Player2->mEP=g_Player->mMaxEP;
	g_Player2->getRifle_pl2();
	g_Player2->mPosition=Vec2(619,262);
	g_Player2->mFootState=FOOTSTATE_RIGHTSTOP;
	g_Player2->mVelocity=Vec2(0,0);

	for(register int i=0;i<WEAPONS;++i)
	{
		g_Player2->mHeat[i]=0.0f;
	}

	g_Mouse.Button.Left=false;
	g_Mouse.Button.Middle=false;
	g_Mouse.Button.Right=false;

	g_pBackground.push_back(&RAW_BACKGROUND_LV5BK1);

	deathPlayer=0;
	bFunModeGotAnim=false;

	LoadRectangleData(std::string("Data\\Levels\\funmode.dat"));

	CSound::Music_SetFocus(RAW_FUNMODEMUSIC);
	RAW_FUNMODEMUSIC->play();
}
bool FileExists(std::string fileName)
{
	bool retVal=false;
	std::ifstream fin(fileName.c_str());
	if(fin){
		retVal=true;
	}
	fin.close();
	return retVal;
}

void FileCheck(std::string fileName)
{
	if(!FileExists(fileName))
	{
		throw FatalError(std::string("Cannot find file \""+fileName+"\". Maybe reinstalling the application will fix this problem!"));
	}
}