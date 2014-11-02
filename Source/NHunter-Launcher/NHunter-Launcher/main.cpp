#include <windows.h>
#include <Windowsx.h>
#include <string>
#include <fstream>
#include "resource.h"

using namespace std;

HWND ghDlg = 0;
HWND ghEB_Volume = 0;
HWND ghCK_Fullscreen = 0;
HWND ghCK_Cheats = 0;
HWND ghCK_Sound = 0;
HINSTANCE g_hInst = 0;
int sound;

string IntToString(int nr)
{
	string str("");
	while(nr)
	{
		str=std::string("")+(char)(nr%10+'0')+str;
		nr/=10;
	}
	return str;
}

int StringToInt(string str)
{
	int nr=0;
	for(register int i=0;i<(int)str.size();++i)
	{
		nr*=10;
		nr+=str[i]-'0';
		if(!(str[i]>='0' && str[i]>='0'))
		{
			return sound;
		}
	}
	if(nr>1000)
	{
		nr=sound;
	}
	return nr;
}

void LoadFromFile()
{
	int x;
	ifstream fin("Data\\SAVE.ini");
	fin>>x;fin>>x;fin>>x;fin>>x;fin>>x;fin>>sound;
	Edit_SetText(ghEB_Volume,IntToString(sound).c_str());
	fin>>x;
	if(x==0)
	{
		Button_SetCheck(ghCK_Sound,BST_CHECKED);
	}
	else
	{
		Button_SetCheck(ghCK_Sound,BST_UNCHECKED);
	}
	fin.close();

	ifstream fin2("Data\\START.ini");
	fin2>>x;
	if(x==1)
	{
		Button_SetCheck(ghCK_Fullscreen,BST_CHECKED);
	}
	else
	{
		Button_SetCheck(ghCK_Fullscreen,BST_UNCHECKED);
	}
	fin2>>x;
	if(x==1)
	{
		Button_SetCheck(ghCK_Cheats,BST_CHECKED);
	}
	else
	{
		Button_SetCheck(ghCK_Cheats,BST_UNCHECKED);
	}
	fin2.close();
}

void SaveToFile()
{
	ifstream fin;
	ofstream fout;

	int levels[5];
	fin.open("Data\\SAVE.ini");
	for(register int i=0;i<5;++i)
	{
		fin>>levels[i];
	}
	fin.close();

	fout.open("Data\\SAVE.ini");
	for(register int i=0;i<5;++i)
	{
		fout<<levels[i]<<" ";
	}
	char str[256];
	memset(str,0,256);
	Edit_GetText(ghEB_Volume,str,256);
	sound=StringToInt(std::string(str));
	fout<<sound;
	if(Button_GetCheck(ghCK_Sound)==BST_CHECKED)
	{
		fout<<" 0";
	}
	else
	{
		fout<<" 1";
	}
	fout.close();

	fout.open("Data\\START.ini");
	if(Button_GetCheck(ghCK_Fullscreen)==BST_UNCHECKED)
	{
		fout<<"0 ";
	}
	else
	{
		fout<<"1 ";
	}
	if(Button_GetCheck(ghCK_Cheats)==BST_UNCHECKED)
	{
		fout<<"0";
	}
	else
	{
		fout<<"1";
	}
	fout.close();
}

BOOL CALLBACK MsgDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ofstream fout;
	ifstream fin;
	int x,levels[5],nr;
	char str[256];
	bool gotError;

	switch( msg )
	{
		case WM_INITDIALOG:
			ghEB_Volume = GetDlgItem(hDlg,IDC_EDIT_VOLUME);
			ghCK_Fullscreen = GetDlgItem(hDlg,IDC_FULLSCREEN);
			ghCK_Cheats = GetDlgItem(hDlg,IDC_ENABLECHEATS);
			ghCK_Sound = GetDlgItem(hDlg,IDC_SOUND);
			
			SetClassLong(hDlg, GCL_HICON, (LONG)LoadIcon(g_hInst, (LPCTSTR)IDI_ICON1));
			SetClassLong(hDlg, GCL_HICONSM, (LONG)LoadIcon(g_hInst, (LPCTSTR)IDI_ICON1));

			LoadFromFile();
		return true;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_START:
					SaveToFile();

					memset(str,0,256);
					Edit_GetText(ghEB_Volume,str,256);
					nr=0;
					gotError=false;
					for(register int i=0;i<(int)strlen(str);++i)
					{
						nr*=10;
						nr+=str[i]-'0';
						if(!(str[i]>='0' && str[i]>='0'))
						{
							gotError=true;
							break;
						}
					}
					if(nr>1000)
					{
						gotError=true;
					}
					if(gotError)
					{
						MessageBox(ghDlg,"Please enter a number in range [1,1000]","Input error!",MB_OK|MB_ICONERROR);
						return true;
					}

					STARTUPINFO         siStartupInfo;
					PROCESS_INFORMATION piProcessInfo;

					ZeroMemory(&siStartupInfo, sizeof(siStartupInfo));
					ZeroMemory(&piProcessInfo, sizeof(piProcessInfo));

					siStartupInfo.cb = sizeof(siStartupInfo);

					if(CreateProcess("NHunter.exe","",0,0,FALSE,CREATE_DEFAULT_ERROR_MODE,0,0,&siStartupInfo,&piProcessInfo) == FALSE)
					{
						MessageBox(ghDlg,"Could not find \"NHunter.exe\". Maybe reinstalling the application will fix this problem!","Fatal error!",MB_OK|MB_ICONERROR);
					}

					PostQuitMessage(0);
				return true;
				case IDC_EXIT:
					PostQuitMessage(0);
				return true;
				case IDC_DEFAULT:
					fin.open("Data\\SAVE.ini");
					for(register int i=0;i<5;++i)
					{
						fin>>levels[i];
					}
					fin.close();

					fout.open("Data\\SAVE.ini");
					for(register int i=0;i<5;++i)
					{
						fout<<levels[i]<<" ";
					}
					fout<<"1000 0";
					fout.close();

					fout.open("Data\\START.ini");
					fout<<"1 0";
					fout.close();

					LoadFromFile();
				return true;
				case IDC_RESET:
					fin.open("Data\\SAVE.ini");
					fin>>x;fin>>x;fin>>x;fin>>x;fin>>x;fin>>sound;fin>>x;
					fin.close();

					fout.open("Data\\SAVE.ini");
					fout<<"1 0 0 0 0 "<<sound<<" "<<x;
					fout.close();

					MessageBox(NULL,"Reseting levels done successfully!","Done!",MB_OK);
				return true;
			}
		return true;
		case WM_CLOSE:
			DestroyWindow(hDlg);
		return true;
		case WM_DESTROY:
			PostQuitMessage(0);
		return true;
	}
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	g_hInst = hInstance;

	ghDlg = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),0,MsgDlgProc);

	ShowWindow(ghDlg, SW_SHOW);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while( GetMessage( &msg, 0, 0, 0 ) )
	{
		if( ghDlg == 0 || !IsDialogMessage(ghDlg, &msg ) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}