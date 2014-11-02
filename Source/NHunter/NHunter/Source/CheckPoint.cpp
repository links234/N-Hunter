#include "..\\Includes\\CheckPoint.h"

extern std::vector<Stuff*>         	g_Stuff;
extern std::vector<MovableObjects*> g_Box;
extern bool							g_bCheats[MAXCHEATS];
extern std::string					g_cheatKeyBuffer;
extern float						SPEED_MOVE_DELAY;
extern float						g_cSpeedModifier;
extern float						NORMAL_MOVE_DELAY;
extern CPlayer						*g_Player;

std::vector< pair<Stuff*,Stuff*> >	g_StuffTable;
std::vector<Stuff*>         	temp_Stuff;
std::vector<MovableObjects*> 	temp_Box;
std::vector<CExplosion*>		temp_Exps;
std::vector<CProjectile*>		temp_Prjs;
bool							temp_bCheats[MAXCHEATS];
std::string						temp_cheatKeyBuffer;
CPlayer							*temp_Player=NULL;

namespace CheckPoint
{
	bool canLoad=false;

	Stuff* new_Stuff(Stuff* ptr)
	{
		pair<Stuff*,Stuff*> p;
		Switch *sw;
		Dummy *dummy;
		MovingDoor *door;

		switch(ptr->FOT)
		{
			case FOT_MOVINGDOOR:
				door=new MovingDoor(*((MovingDoor*)ptr));
				p.first=ptr;
				p.second=door;
				g_StuffTable.push_back(p);
				return (Stuff*)door;
			break;
			case FOT_SWITCH:
				sw=new Switch(*((Switch*)ptr));
				for(register int i=0;i<(int)g_StuffTable.size();++i)
				{
					if(sw->mDoor==g_StuffTable[i].first)
					{
						sw->mDoor=(MovingDoor*)g_StuffTable[i].second;
						break;
					}
				}
				return (Stuff*)sw;
			break;
			case FOT_FINISHZONE:
				return (Stuff*)new FinishZone(*((FinishZone*)ptr));
			break;
			case FOT_TURRET:
				return (Stuff*)new Turret(*((Turret*)ptr));
			break;
			case FOT_FALLINGFLOOR:
				return (Stuff*)new FallingFloor(*((FallingFloor*)ptr));
			break;
			case FOT_DUMMY:
				dummy=new Dummy(*((Dummy*)ptr));
				for(register int i=0;i<(int)g_StuffTable.size();++i)
				{
					if(dummy->mDoor==g_StuffTable[i].first)
					{
						dummy->mDoor=(MovingDoor*)g_StuffTable[i].second;
						break;
					}
				}
				return (Stuff*)dummy;
			break;
			case FOT_PLATFORM:
				return (Stuff*)new Platform(*((Platform*)ptr));
			break;
			case FOT_ELEVATOR:
				return (Stuff*)new Elevator(*((Elevator*)ptr));
			break;
			case FOT_SPIKES:
				return (Stuff*)new Spikes(*((Spikes*)ptr));
			break;
			case FOT_CHECKP:
				return (Stuff*)new CheckP(*((CheckP*)ptr));
			break;
			case FOT_ENEMY:
				return (Stuff*)new Enemy(*((Enemy*)ptr));
			break;
			case FOT_BOSS:
				return (Stuff*)new Boss(*((Boss*)ptr));
			break;
			case FOT_REPAIRKIT:
				return (Stuff*)new RepairKit(*((RepairKit*)ptr));
			break;
		}
		MessageBox(NULL,"EXCEPTION!","",MB_OK);
		return (Stuff*)NULL;
	}

	void Clear()
	{
		canLoad=false;
	}

	void Save()
	{
		temp_cheatKeyBuffer=g_cheatKeyBuffer;

		g_StuffTable.resize(0);
		canLoad=true;
		for(register int i=0;i<(int)temp_Box.size();++i)
		{
			delete temp_Box[i];
		}
		temp_Box.resize(0);
		for(register int i=0;i<(int)g_Box.size();++i)
		{
			temp_Box.push_back(new MovableObjects(*g_Box[i]));
		}

		for(register int i=0;i<(int)temp_Exps.size();++i)
		{
			delete temp_Exps[i];
		}
		temp_Exps.resize(0);
		for(register int i=0;i<(int)CExplosion::sExps.size();++i)
		{
			temp_Exps.push_back(new CExplosion(*CExplosion::sExps[i]));
		}

		for(register int i=0;i<(int)temp_Prjs.size();++i)
		{
			delete temp_Prjs[i];
		}
		temp_Prjs.resize(0);
		for(register int i=0;i<(int)CProjectile::sPrjs.size();++i)
		{
			temp_Prjs.push_back(new CProjectile(*CProjectile::sPrjs[i]));
		}

		for(register int i=0;i<(int)temp_Stuff.size();++i)
		{
			delete temp_Stuff[i];
		}
		temp_Stuff.resize(0);
		for(register int i=0;i<(int)g_Stuff.size();++i)
		{
			temp_Stuff.push_back(new_Stuff(g_Stuff[i]));
		}

		for(register int i=0;i<MAXCHEATS;++i)
		{
			temp_bCheats[i]=g_bCheats[i];
		}
		if(temp_Player!=NULL)
		{
			delete temp_Player;
		}
		temp_Player=new CPlayer(*g_Player);
	}

	void Load()
	{
		if(canLoad)
		{
			if(temp_bCheats[CHEAT_SPEED]!=g_bCheats[CHEAT_SPEED])
			{
				if(!temp_bCheats[CHEAT_SPEED])
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
				else
				{
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
			SPEED_MOVE_DELAY = NORMAL_MOVE_DELAY/g_cSpeedModifier;

			g_cheatKeyBuffer=temp_cheatKeyBuffer;

			g_StuffTable.resize(0);
			for(register int i=0;i<(int)g_Box.size();++i)
			{
				delete g_Box[i];
			}
			g_Box.resize(0);
			for(register int i=0;i<(int)temp_Box.size();++i)
			{
				g_Box.push_back(new MovableObjects(*temp_Box[i]));
			}

			for(register int i=0;i<(int)CExplosion::sExps.size();++i)
			{
				delete CExplosion::sExps[i];
			}
			CExplosion::sExps.resize(0);
			for(register int i=0;i<(int)temp_Exps.size();++i)
			{
				CExplosion::sExps.push_back(new CExplosion(*temp_Exps[i]));
			}

			for(register int i=0;i<(int)CProjectile::sPrjs.size();++i)
			{
				delete CProjectile::sPrjs[i];
			}
			CProjectile::sPrjs.resize(0);
			for(register int i=0;i<(int)temp_Prjs.size();++i)
			{
				CProjectile::sPrjs.push_back(new CProjectile(*temp_Prjs[i]));
			}

			for(register int i=0;i<(int)g_Stuff.size();++i)
			{
				delete g_Stuff[i];
			}
			g_Stuff.resize(0);
			for(register int i=0;i<(int)temp_Stuff.size();++i)
			{
				g_Stuff.push_back(new_Stuff(temp_Stuff[i]));
			}		

			for(register int i=0;i<MAXCHEATS;++i)
			{
				g_bCheats[i]=temp_bCheats[i];
			}
			g_Player=new CPlayer(*temp_Player);
		}
	}

	bool CanLoad()
	{
		return canLoad;
	}
}