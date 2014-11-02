#include "..\\Includes\\CPlayer.h"

extern Vec2 							g_PointOfView;
extern std::vector<RECT> 				g_CRect;
extern std::vector<Stuff*>         		g_Stuff;
extern std::vector<MovableObjects*> 	g_Box;
extern AnimatedSprite					*RAW_PLAYER_MOVEMENT_ANIMATION_RIFLE,*RAW_PLAYER2_MOVEMENT_ANIMATION_RIFLE,*RAW_PLAYER_MOVEMENT_ANIMATION_SHOTGUN,*RAW_PLAYER_MOVEMENT_ANIMATION_PISTOL,*RAW_PLAYER_MOVEMENT_ANIMATION_BAZOOKA;
extern Sprite							*RAW_W1_PRJ, *RAW_W2_PRJ, *RAW_W3_PRJ;
extern bool								g_bCheats[MAXCHEATS];
extern AnimatedSprite					*RAW_EXP1, *RAW_EXP2;

bool CollideMap(RECT CRect);

#define g_cNormalGroundSpeed	250.0f
#define g_cNormalAirSpeed		200.0f
#define g_cNormalJumpingSpeed	1000.0f
#define g_cArmorModifier		2.0f
#define g_cStrengthModifier		1.3f
#define g_cArmorPWEC			50.0f
#define g_cSpeedPWEC			100.0f
#define g_cStrengthPWEC			200.0f
#define g_cEPRegen				50.0f
#define g_cArmorValue			60

float NORMAL_MOVE_DELAY					= 0.15f;
float g_cSpeedModifier					= 2.0f;
float SPEED_MOVE_DELAY					= NORMAL_MOVE_DELAY/g_cSpeedModifier;

const int FOOTSTATE_LEFTSTOP 			= 1;
const int FOOTSTATE_LEFTWALK 			= 2;
const int FOOTSTATE_RIGHTSTOP 			= 3;
const int FOOTSTATE_RIGHTWALK 			= 4;
const int FOOTSTATE_JUMPINGRIGHT	    = 5;
const int FOOTSTATE_JUMPINGLEFT			= 6;
const int FOOTSTATE_REVERSELEFT			= 7;
const int FOOTSTATE_REVERSERIGHT		= 8;
const int FOOTSTATE_REVERSELEFTJUMPING 	= 9;
const int FOOTSTATE_REVERSERIGHTJUMPING	= 10;

const int WEAPON_DIR_WD					= 1;
const int WEAPON_DIR_WA					= 2;
const int WEAPON_DIR_E					= 3;
const int WEAPON_DIR_Q					= 4;
const int WEAPON_DIR_D					= 5;
const int WEAPON_DIR_A					= 6;
const int WEAPON_DIR_SD					= 7;
const int WEAPON_DIR_SA					= 8;
const int WEAPON_DIR_Z					= 9;
const int WEAPON_DIR_C					= 10;
const int WEAPON_NO_DIR					= 11;

const int PW_NONE						= 0;
const int PW_SPEED						= 1;
const int PW_ARMOR						= 2;
const int PW_STRENGTH					= 3;

const int NONE							= 0;
const int MULTIPLY						= 1;
const int MULTIPLIED					= 2;

CPlayer::CPlayer()
	: mFootState(FOOTSTATE_RIGHTSTOP), mPosition(250,250), mVelocity(0,0),
	mTime(0.0f), mDelay(NORMAL_MOVE_DELAY), mGroundSpeed(g_cNormalGroundSpeed), mAirSpeed(g_cNormalAirSpeed),
	mJumpingSpeed(g_cNormalJumpingSpeed), mWeaponDir(WEAPON_NO_DIR), bGotToAttack(false), mMaxHP(1000), mHP(1000), 
	mArmor(g_cArmorValue), bDead(false), mMaxEP(1000), mEP(1000), mWeaponId(1), CurrPW(PW_ARMOR), bGotToDoAnimation(false), pMovement(NULL)
{
}


void CPlayer::draw(HDC hBackBuffer)
{
	pMovement->mPosition.x=mPosition.x-g_PointOfView.x-50;
	pMovement->mPosition.y=mPosition.y-g_PointOfView.y+5;
	pMovement->draw(hBackBuffer);
}


void CPlayer::update_part2()
{
	RECT CR=CRect();
	
	std::vector<RECT> g_CRect2;
	for(register int i=0;i<(int)g_Box.size();++i)
	{
		if(g_Box[i]->mIsSolid)
		{
			g_CRect2.push_back(g_Box[i]->Rect());
		}
	}

	int sign;
	for(register int i=0;i<(int)g_CRect2.size();++i)
	{
		if(RectCollideRect(CR,g_CRect2[i]))
		{
			if(g_CRect2[i].left>CR.left)
			{
				sign=-1;
			}
			else
			{
				sign=1;
			}
			while(RectCollideRect(CR,g_CRect2[i]))
			{
				mPosition.x+=sign*0.5;
				CR=CRect();
			}
		}
	}

	for(register int i=0;i<(int)g_Stuff.size();++i)
	{
		if(g_Stuff[i]->CheckCollision(CR))
		{
			RECT R=g_Stuff[i]->Rect();
			if(R.left>CR.left)
			{
				sign=-1;
			}
			else
			{
				sign=1;
			}
			while(RectCollideRect(CR,R))
			{
				mPosition.x+=sign*0.5;
				CR=CRect();
			}
		}
	}
}


void CPlayer::update(float delta)
{
	if(mHP==0)
	{
		bDead=true;
		bGotToDoAnimation=true;
		if(CurrPW==PW_SPEED && PWSPhase==MULTIPLIED)
		{
			for(register int i=0;i<PRJSPEED_LENGTH;++i)
			{
				mPrjSpeed[i].x/=g_cSpeedModifier;
				mPrjSpeed[i].y/=g_cSpeedModifier;
			}
		}
		PWSPhase=NONE;
		return;
	}

	mEP+=delta*g_cEPRegen;
	if(mEP>mMaxEP)mEP=mMaxEP;

	if(mFootState!=FOOTSTATE_LEFTSTOP && mFootState!=FOOTSTATE_RIGHTSTOP)
	{
		if(CurrPW==PW_SPEED)
		{
			if(PWSPhase!=MULTIPLIED)
			{
				PWSPhase=MULTIPLIED;
				for(register int i=0;i<PRJSPEED_LENGTH;++i)
				{
					mPrjSpeed[i].x*=g_cSpeedModifier;
					mPrjSpeed[i].y*=g_cSpeedModifier;
				}
			}
			float cost=delta*g_cSpeedPWEC;
			if(cost<mEP)
			{
				mDelay=SPEED_MOVE_DELAY;
				mEP-=cost;
			}
			else
			{
				PW_Armor();
			}
		}
	}
	else
	{
		if(CurrPW==PW_SPEED && PWSPhase==MULTIPLIED)
		{
			PWSPhase=MULTIPLY;
			for(register int i=0;i<PRJSPEED_LENGTH;++i)
			{
				mPrjSpeed[i].x/=g_cSpeedModifier;
				mPrjSpeed[i].y/=g_cSpeedModifier;
			}
		}
	}

	for(register int i=0;i<WEAPONS;++i)
	{
		mHeat[i]-=delta*mCoolingSpeed;
		if(mHeat[i]<0)mHeat[i]=0;
	}

	Vec2 displacement=mVelocity*delta;

	RECT CR=CRect();
	
	int sign;
	for(register int i=0;i<(int)g_CRect.size();++i)
	{
		if(RectCollideRect(CR,g_CRect[i]))
		{
			if(g_CRect[i].left>CR.left)
			{
				sign=-1;
			}
			else
			{
				sign=1;
			}
			while(RectCollideRect(CR,g_CRect[i]))
			{
				mPosition.x+=sign*0.5;
				CR=CRect();
			}
		}
	}

	mPosition.x+=displacement.x;
	CR=CRect();

	if(CollideMap(CR) || Collide(CR) || MoveBox(CR))
	{
		mPosition.x-=displacement.x;
		mVelocity.x=0.0f;
	}

	mPosition.y+=displacement.y;
	CR=CRect();

	if(CollideMap(CR) || Collide(CR) || CollideBox(CR))
	{
		mPosition.y-=displacement.y;
		mVelocity.y=0.0f;
		if(displacement.y>0.0f)
			mYCollide=true;
	}
	else
	{
		mYCollide=false;
	}

	CR=CRect();
	for(register int i=0;i<(int)g_Stuff.size();++i)
	{
		if(g_Stuff[i]->isElevator)
		{
			RECT R=g_Stuff[i]->Rect();
			int gap=(R.bottom-R.top+1)/2;
			R.bottom-=gap;
			R.top-=gap;
			if(RectCollideRect(CR,R))
			{
				if(R.top<=CR.bottom && CR.bottom<=R.bottom)
				{
					mYCollide=true;
					mPosition.y=((Elevator*)g_Stuff[i])->mPosition.y-201;
					if(mVelocity.y>0)mVelocity.y=0.0f;
				}
			}
		}
	}

	if(mYCollide)
	{
		if(mFootState==FOOTSTATE_JUMPINGRIGHT || mFootState==FOOTSTATE_REVERSELEFTJUMPING)
		{
			mFootState=FOOTSTATE_RIGHTSTOP;
		}
		else if(mFootState==FOOTSTATE_JUMPINGLEFT || mFootState==FOOTSTATE_REVERSERIGHTJUMPING)
		{
			mFootState=FOOTSTATE_LEFTSTOP;
		}
	}

	g_PointOfView.x=mPosition.x-512+50;

	mTime+=delta;
	while(mTime>mDelay)
	{
		nextAnimatedFrame();
		mTime-=mDelay;
	}

	if(mWeaponDir==WEAPON_DIR_WD)
			pMovement->SetFrame(44);
	else if(mWeaponDir==WEAPON_DIR_WA)
			pMovement->SetFrame(64);
	else if(mWeaponDir==WEAPON_DIR_E)
			pMovement->SetFrame(45);
	else if(mWeaponDir==WEAPON_DIR_Q)
			pMovement->SetFrame(65);
	else if(mWeaponDir==WEAPON_DIR_D)
			pMovement->SetFrame(1);
	else if(mWeaponDir==WEAPON_DIR_A)
			pMovement->SetFrame(21);
	else if(mWeaponDir==WEAPON_DIR_SD)
			pMovement->SetFrame(43);
	else if(mWeaponDir==WEAPON_DIR_SA)
			pMovement->SetFrame(63);
	else if(mWeaponDir==WEAPON_DIR_Z)
			pMovement->SetFrame(62);
	else if(mWeaponDir==WEAPON_DIR_C)
			pMovement->SetFrame(42);
	mWeaponDir=WEAPON_NO_DIR;

	if(bGotToAttack)
		attack(delta);
}


void CPlayer::nextAnimatedFrame()
{
	int frame;
	switch(mFootState)
	{
		case FOOTSTATE_LEFTSTOP:
			pMovement->SetFrame(21);
		break;

		case FOOTSTATE_LEFTWALK:
			frame=pMovement->GetFrameId();
			if(frame<21)frame=20;
			++frame;
			if(frame>40)frame=21;
			pMovement->SetFrame(frame);
		break;

		case FOOTSTATE_RIGHTSTOP:
			pMovement->SetFrame(1);
		break;

		case FOOTSTATE_RIGHTWALK:
			frame=pMovement->GetFrameId();
			if(frame<1)frame=0;
			++frame;
			if(frame>20)frame=1;
			pMovement->SetFrame(frame);
		break;

		case FOOTSTATE_JUMPINGRIGHT:
			if(mVelocity.y<0)
			{
				pMovement->SetFrame(41);
			}
			else
			{
				pMovement->SetFrame(46);
			}
		break;

		case FOOTSTATE_JUMPINGLEFT:
			if(mVelocity.y<0)
			{
				pMovement->SetFrame(61);
			}
			else
			{
				pMovement->SetFrame(66);
			}
		break;

		case FOOTSTATE_REVERSELEFT:
			frame=pMovement->GetFrameId();
			if(frame>20)frame=20;
			--frame;
			if(frame<1)frame=20;
			pMovement->SetFrame(frame);
		break;

		case FOOTSTATE_REVERSERIGHT:
			frame=pMovement->GetFrameId();
			if(frame>40)frame=40;
			--frame;
			if(frame<21)frame=40;
			pMovement->SetFrame(frame);
		break;

		case FOOTSTATE_REVERSERIGHTJUMPING:
			if(mVelocity.y>=0)
			{
				pMovement->SetFrame(61);
			}
			else
			{
				pMovement->SetFrame(66);
			}
		break;

		case FOOTSTATE_REVERSELEFTJUMPING:
			if(mVelocity.y>=0)
			{
				pMovement->SetFrame(41);
			}
			else
			{
				pMovement->SetFrame(46);
			}
		break;
	}
}


void CPlayer::jump()
{
	if(CurrPW==PW_STRENGTH)
	{
		bool a=false;
		if(mEP>=g_cStrengthPWEC)
		{
			a=true;
			mEP-=g_cStrengthPWEC;
		}
		else
		{
			mJumpingSpeed/=g_cStrengthModifier;
		}
		mVelocity.y=-mJumpingSpeed;
		if(mFootState==FOOTSTATE_RIGHTWALK || mFootState==FOOTSTATE_RIGHTSTOP || mFootState==FOOTSTATE_JUMPINGRIGHT)
		{
			mFootState=FOOTSTATE_JUMPINGRIGHT;
		}
		if(mFootState==FOOTSTATE_LEFTWALK || mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_JUMPINGLEFT)
		{
			mFootState=FOOTSTATE_JUMPINGLEFT;
		}
		if(!a)
		{
			mJumpingSpeed*=g_cStrengthModifier;
		}
	}
	else
	{
		mVelocity.y=-mJumpingSpeed;
		if(mFootState==FOOTSTATE_RIGHTWALK || mFootState==FOOTSTATE_RIGHTSTOP || mFootState==FOOTSTATE_JUMPINGRIGHT)
		{
			mFootState=FOOTSTATE_JUMPINGRIGHT;
		}
		if(mFootState==FOOTSTATE_LEFTWALK || mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_JUMPINGLEFT)
		{
			mFootState=FOOTSTATE_JUMPINGLEFT;
		}
	}
}


void CPlayer::leftGroundMove()
{
	mVelocity.x=-mGroundSpeed;
	mFootState=FOOTSTATE_LEFTWALK;
}


void CPlayer::leftAirMove()
{
	mVelocity.x=-mAirSpeed;
	mFootState=FOOTSTATE_JUMPINGLEFT;
}	

void CPlayer::rightGroundMove()
{
	mVelocity.x=mGroundSpeed;
	mFootState=FOOTSTATE_RIGHTWALK;
}	

void CPlayer::rightAirMove()
{
	mVelocity.x=mAirSpeed;
	mFootState=FOOTSTATE_JUMPINGRIGHT;
}


void CPlayer::stopMoving()
{
	mVelocity.x=0.0f;
	if(mFootState==FOOTSTATE_RIGHTWALK || mFootState==FOOTSTATE_RIGHTSTOP || mFootState==FOOTSTATE_REVERSERIGHT)
	{
		mFootState=FOOTSTATE_RIGHTSTOP;
	}
	if(mFootState==FOOTSTATE_LEFTWALK || mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_REVERSELEFT)
	{
		mFootState=FOOTSTATE_LEFTSTOP;
	}
}


void CPlayer::attackW()
{
	if(mFootState==FOOTSTATE_LEFTSTOP)
		mWeaponDir=WEAPON_DIR_Q;
	else if(mFootState==FOOTSTATE_RIGHTSTOP)
		mWeaponDir=WEAPON_DIR_E;
	else 
		mWeaponDir=WEAPON_NO_DIR;
	if(mWeaponDir!=WEAPON_NO_DIR)
		attack(false);
}


void CPlayer::attackWD()
{
	if(mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_RIGHTSTOP)
		mWeaponDir=WEAPON_DIR_WD;
	else 
		mWeaponDir=WEAPON_NO_DIR;
	if(mWeaponDir!=WEAPON_NO_DIR)
		attack(false);
}	

void CPlayer::attackD()
{
	if(mFootState==FOOTSTATE_RIGHTWALK || mFootState==FOOTSTATE_RIGHTSTOP || mFootState==FOOTSTATE_JUMPINGRIGHT)
		mWeaponDir=WEAPON_DIR_D;
	else if(mFootState==FOOTSTATE_LEFTWALK)
	{
		mFootState=FOOTSTATE_REVERSELEFT;
		mWeaponDir=WEAPON_DIR_D;
	}
	else if(mFootState==FOOTSTATE_JUMPINGLEFT)
	{
		mFootState=FOOTSTATE_REVERSELEFTJUMPING;
		mWeaponDir=WEAPON_DIR_D;
	}
	else if(mFootState==FOOTSTATE_LEFTSTOP)
	{
		mFootState=FOOTSTATE_RIGHTSTOP;
		mWeaponDir=WEAPON_DIR_D;
	}
	else 
		mWeaponDir=WEAPON_NO_DIR;
	if(mWeaponDir!=WEAPON_NO_DIR)
		attack(true);
}
void CPlayer::attackDS()
{
	if(mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_RIGHTSTOP)
		mWeaponDir=WEAPON_DIR_SD;
	else 
		mWeaponDir=WEAPON_NO_DIR;
	if(mWeaponDir!=WEAPON_NO_DIR)
		attack(false);
}
void CPlayer::attackS()
{
	if(mFootState==FOOTSTATE_LEFTSTOP)
		mWeaponDir=WEAPON_DIR_Z;
	else if(mFootState==FOOTSTATE_RIGHTSTOP)
		mWeaponDir=WEAPON_DIR_C;
	else 
		mWeaponDir=WEAPON_NO_DIR;
	if(mWeaponDir!=WEAPON_NO_DIR)
		attack(false);
}
void CPlayer::attackSA()
{
	if(mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_RIGHTSTOP)
		mWeaponDir=WEAPON_DIR_SA;
	else 
		mWeaponDir=WEAPON_NO_DIR;
	if(mWeaponDir!=WEAPON_NO_DIR)
		attack(false);
}
void CPlayer::attackA()
{
	if(mFootState==FOOTSTATE_LEFTWALK || mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_JUMPINGLEFT)
		mWeaponDir=WEAPON_DIR_A;
	else if(mFootState==FOOTSTATE_RIGHTWALK)
	{
		mFootState=FOOTSTATE_REVERSERIGHT;
		mWeaponDir=WEAPON_DIR_A;
	}
	else if(mFootState==FOOTSTATE_JUMPINGRIGHT)
	{
		mFootState=FOOTSTATE_REVERSERIGHTJUMPING;
		mWeaponDir=WEAPON_DIR_A;
	}
	else if(mFootState==FOOTSTATE_RIGHTSTOP)
	{
		mFootState=FOOTSTATE_LEFTSTOP;
		mWeaponDir=WEAPON_DIR_D;
	}
	else 
		mWeaponDir=WEAPON_NO_DIR;
	if(mWeaponDir!=WEAPON_NO_DIR)
		attack(true);
}
void CPlayer::attackAW()
{
	if(mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_RIGHTSTOP)
		mWeaponDir=WEAPON_DIR_WA;
	else
		mWeaponDir=WEAPON_NO_DIR;
	if(mWeaponDir!=WEAPON_NO_DIR)
		attack(false);
}


void CPlayer::attack(bool isMoving)
{
	bGotToAttack=true;
	mAttackDir=mWeaponDir;
	if(isMoving)
	{
		mWeaponDir=WEAPON_NO_DIR;
	}
}


void CPlayer::attack(float delta)
{
	bGotToAttack=false;
	
	static float time=0.0f;
	time+=delta;

	if(time>mFireingDelay)
	{
		if(mHeat[mWeaponId]<100)
		{
			mHeat[mWeaponId]+=mHeatLevel;
			Vec2 pos(mPosition.x+mShootingPoint[mAttackDir].x-(pPrj->width()/2),mPosition.y+mShootingPoint[mAttackDir].y-(pPrj->height()/2));
			int mMiDmg=mMinDmg;
			int mMaDmg=mMaxDmg;
			if(g_bCheats[CHEAT_1HIT1KILL] || g_bCheats[CHEAT_GOD])
			{
				mMiDmg=2<<30;
				mMaDmg=2<<30;
			}
			if(mWeaponId==WEP_SHOTGUN)
			{
				mMiDmg/=3;
				mMaDmg/=3;
			}
			AnimatedSprite *EXPLOSION=RAW_EXP1;
			if(mWeaponId==WEP_BAZOOKA)
			{
				EXPLOSION=RAW_EXP2;
			}
			if(g_bCheats[CHEAT_WALLSHOT])
			{
				int gap=30;
				if(isRight())
				{
					pos.x=mPosition.x+mShootingPoint[WEAPON_DIR_D].x;
					pos.y=0;
					while(pos.y<=768)
					{
						pos.y+=gap;
						if(bExplosive)
						{
							CProjectile::Add(new CProjectile(pos,mPrjSpeed[WEAPON_DIR_D],pPrj,mMiDmg,mMaDmg,PRJ_NORMAL,true,EXPLOSION,TPF));
						}
						else
						{
							CProjectile::Add(new CProjectile(pos,mPrjSpeed[WEAPON_DIR_D],pPrj,mMiDmg,mMaDmg));
						}
					}
				}
				else
				{
					pos.x=mPosition.x+mShootingPoint[WEAPON_DIR_A].x;
					pos.y=0;
					while(pos.y<=768)
					{
						pos.y+=gap;
						if(bExplosive)
						{
							CProjectile::Add(new CProjectile(pos,mPrjSpeed[WEAPON_DIR_A],pPrj,mMiDmg,mMaDmg,PRJ_NORMAL,true,EXPLOSION,TPF));
						}
						else
						{
							CProjectile::Add(new CProjectile(pos,mPrjSpeed[WEAPON_DIR_A],pPrj,mMiDmg,mMaDmg));
						}
					}
				}
			}
			else
			{
				if(mWeaponId==WEP_SHOTGUN)
				{
					if(bExplosive)
					{
						CProjectile::Add(new CProjectile(pos,mPrjSpeed[mAttackDir].vRotate(DEG2RAD(15)),pPrj,mMiDmg,mMaDmg,PRJ_NORMAL,true,EXPLOSION,TPF));
						CProjectile::Add(new CProjectile(pos,mPrjSpeed[mAttackDir],pPrj,mMiDmg,mMaDmg,PRJ_NORMAL,true,EXPLOSION,TPF));
						CProjectile::Add(new CProjectile(pos,mPrjSpeed[mAttackDir].vRotate(DEG2RAD(-15)),pPrj,mMiDmg,mMaDmg,PRJ_NORMAL,true,EXPLOSION,TPF));
					}
					else
					{
						CProjectile::Add(new CProjectile(pos,mPrjSpeed[mAttackDir].vRotate(DEG2RAD(15)),pPrj,mMiDmg,mMaDmg));
						CProjectile::Add(new CProjectile(pos,mPrjSpeed[mAttackDir],pPrj,mMiDmg,mMaDmg));
						CProjectile::Add(new CProjectile(pos,mPrjSpeed[mAttackDir].vRotate(DEG2RAD(-15)),pPrj,mMiDmg,mMaDmg));
					}
				}
				else
				{
					if(bExplosive)
					{
						CProjectile::Add(new CProjectile(pos,mPrjSpeed[mAttackDir],pPrj,mMiDmg,mMaDmg,PRJ_NORMAL,true,EXPLOSION,TPF));
					}
					else
					{
						CProjectile::Add(new CProjectile(pos,mPrjSpeed[mAttackDir],pPrj,mMiDmg,mMaDmg));
					}
				}

			}
			time=0.0f;
		}
	}

	mAttackDir=WEAPON_NO_DIR;
}


void CPlayer::loadCRects(std::string filename)
{
	std::ifstream fin(filename.c_str());
	int start,finish;
	fin>>start>>finish;
	for(register int i=start;i<=finish;++i)
	{
		fin>>CRects[i].left>>CRects[i].top>>CRects[i].right>>CRects[i].bottom;
	}
	fin.close();
}


RECT CPlayer::CRect()
{
	RECT rect;
	rect.bottom=(int)mPosition.y+CRects[pMovement->GetFrameId()].bottom;
	rect.top=(int)mPosition.y+CRects[pMovement->GetFrameId()].top;
	rect.left=(int)mPosition.x+CRects[pMovement->GetFrameId()].left;
	rect.right=(int)mPosition.x+CRects[pMovement->GetFrameId()].right;
	return rect;
}


void CPlayer::takeDamage(int dmg)
{
	if(g_bCheats[CHEAT_IMMORTAL] || g_bCheats[CHEAT_GOD])
		return;
	if(CurrPW==PW_ARMOR)
	{
		bool a=false;
		if(mEP>g_cArmorPWEC)
		{
			mEP-=g_cArmorPWEC;
			a=true;
		}
		else
		{
			float fArmor=float(mArmor)/g_cArmorModifier;
			mArmor=(int)fArmor;
		}
		dmg-=mArmor;
		if(dmg<0)dmg=0;
		if(dmg>mHP)mHP=0;
		mHP-=dmg;
		if(mHP<0)mHP=0;

		if(!a)
		{
			float fArmor=float(mArmor)*g_cArmorModifier;
			mArmor=(int)fArmor;
		}
	}
	else
	{
		dmg-=mArmor;
		if(dmg<0)dmg=0;
		if(dmg>mHP)mHP=0;
		mHP-=dmg;
		if(mHP<0)mHP=0;
	}
}


void CPlayer::loadWeaponData(std::string filename)
{
	const int GAP=-52;
	std::ifstream fin(filename.c_str());

	for(register int i=WEAPON_DIR_WD;i<WEAPON_NO_DIR;++i)
	{
		fin>>mPrjSpeed[i].x>>mPrjSpeed[i].y;
	}

	fin>>mMinDmg>>mMaxDmg;
	fin>>mCoolingSpeed;
	fin>>mFireingDelay;
	fin>>mHeatLevel;

	for(register int i=1;i<11;++i)
	{
		fin>>mShootingPoint[i].x>>mShootingPoint[i].y;
		mShootingPoint[i].x+=GAP;
	}
	fin.close();
}


bool CollideMap(RECT CRect)
{
	for(register int i=0;i<(int)g_CRect.size();++i)
	{
		if(RectCollideRect(CRect,g_CRect[i]))
		{
			return true;
		}
	}
	return false;
}


void CPlayer::PW_Speed()
{
	if(CurrPW!=PW_SPEED)
	{
		turnOffPW();
		mGroundSpeed*=g_cSpeedModifier;
		mAirSpeed*=g_cSpeedModifier;
		CurrPW=PW_SPEED;
		if(mFootState!=FOOTSTATE_LEFTSTOP && mFootState!=FOOTSTATE_RIGHTSTOP)
		{
			PWSPhase=MULTIPLIED;
			for(register int i=0;i<PRJSPEED_LENGTH;++i)
			{
				mPrjSpeed[i].x*=g_cSpeedModifier;
				mPrjSpeed[i].y*=g_cSpeedModifier;
			}
		}
		else
		{
			PWSPhase=NONE;
		}
		bGotToDoAnimation=true;
	}
}

void CPlayer::PW_Armor()
{
	if(CurrPW!=PW_ARMOR)
	{
		turnOffPW();
		float fArmor=float(mArmor)*g_cArmorModifier;
		mArmor=(int)fArmor;

		CurrPW=PW_ARMOR;
		bGotToDoAnimation=true;
	}
}

void CPlayer::PW_Strength()
{
	if(CurrPW!=PW_STRENGTH)
	{
		turnOffPW();
		mJumpingSpeed*=g_cStrengthModifier;
		CurrPW=PW_STRENGTH;
		bGotToDoAnimation=true;
	}
}


void CPlayer::turnOffPW()
{
	float fArmor;
	switch(CurrPW)
	{
		case PW_ARMOR:
			fArmor=float(mArmor)/g_cArmorModifier;
			mArmor=(int)fArmor;
		break;
		case PW_SPEED:
			mGroundSpeed/=g_cSpeedModifier;
			mAirSpeed/=g_cSpeedModifier;
			mDelay=NORMAL_MOVE_DELAY;
			bCanSpeed=true;
			if(PWSPhase==MULTIPLIED)
			{
				for(register int i=0;i<PRJSPEED_LENGTH;++i)
				{
					mPrjSpeed[i].x/=g_cSpeedModifier;
					mPrjSpeed[i].y/=g_cSpeedModifier;
				}
			}
			PWSPhase=NONE;
		break;
		case PW_STRENGTH:
			mJumpingSpeed/=g_cStrengthModifier;
		break;
	}
}


bool CPlayer::isRight()
{
	return !isLeft();
}

bool CPlayer::isLeft()
{
	if(mFootState==FOOTSTATE_LEFTSTOP || mFootState==FOOTSTATE_LEFTWALK  || mFootState==FOOTSTATE_JUMPINGLEFT || mFootState==FOOTSTATE_REVERSERIGHT || mFootState==FOOTSTATE_REVERSERIGHTJUMPING)return true;
	return false;
}


void CPlayer::getRifle()
{
	if(mWeaponState[WEP_RIFLE]==WEPSTATE_ENABLED)
	{
		if(pMovement!=NULL)
		{
			RAW_PLAYER_MOVEMENT_ANIMATION_RIFLE->SetFrame(pMovement->GetFrameId());
		}
		for(register int i=0;i<WEAPONS;++i)
		{
			if(mWeaponState[i]!=WEPSTATE_DISABLED)
			{
				mWeaponState[i]=WEPSTATE_ENABLED;
			}
		}
		bool wasInThere=false;
		if(CurrPW==PW_SPEED)
		{
			wasInThere=true;
			PW_Armor();
		}
		mWeaponState[WEP_RIFLE]=WEPSTATE_CHOOSED;
		mWeaponId=WEP_RIFLE;
		FileCheck(std::string("Data\\WEPDATARIFLE.ini"));
		loadWeaponData(std::string("Data\\WEPDATARIFLE.ini"));
		pMovement=RAW_PLAYER_MOVEMENT_ANIMATION_RIFLE;
		FileCheck(std::string("Data\\PMACRects.ini"));
		loadCRects(std::string("Data\\PMACRects.ini"));
		pPrj=RAW_W1_PRJ;
		bExplosive=true;
		TPF=0.05f;
		if(wasInThere)
		{
			PW_Speed();
			bGotToDoAnimation=false;
		}
	}
}	

void CPlayer::getRifle_pl2()
{
	if(mWeaponState[WEP_RIFLE]==WEPSTATE_ENABLED)
	{
		for(register int i=0;i<WEAPONS;++i)
		{
			if(mWeaponState[i]!=WEPSTATE_DISABLED)
			{
				mWeaponState[i]=WEPSTATE_ENABLED;
			}
		}
		mWeaponState[WEP_RIFLE]=WEPSTATE_CHOOSED;
		mWeaponId=WEP_RIFLE;
		FileCheck(std::string("Data\\WEPDATARIFLE.ini"));
		loadWeaponData(std::string("Data\\WEPDATARIFLE.ini"));
		pMovement=RAW_PLAYER2_MOVEMENT_ANIMATION_RIFLE;
		FileCheck(std::string("Data\\PMACRects.ini"));
		loadCRects(std::string("Data\\PMACRects.ini"));
		pPrj=RAW_W1_PRJ;
		bExplosive=true;
		TPF=0.05f;
	}
}	

void CPlayer::getPistol()
{
	if(mWeaponState[WEP_PISTOL]==WEPSTATE_ENABLED)
	{
		if(pMovement!=NULL)
		{
			RAW_PLAYER_MOVEMENT_ANIMATION_PISTOL->SetFrame(pMovement->GetFrameId());
		}
		for(register int i=0;i<WEAPONS;++i)
		{
			if(mWeaponState[i]!=WEPSTATE_DISABLED)
			{
				mWeaponState[i]=WEPSTATE_ENABLED;
			}
		}
		bool wasInThere=false;
		if(CurrPW==PW_SPEED)
		{
			wasInThere=true;
			PW_Armor();
		}
		mWeaponState[WEP_PISTOL]=WEPSTATE_CHOOSED;
		mWeaponId=WEP_PISTOL;
		FileCheck(std::string("Data\\WEPDATAPISTOL.ini"));
		loadWeaponData(std::string("Data\\WEPDATAPISTOL.ini"));
		pMovement=RAW_PLAYER_MOVEMENT_ANIMATION_PISTOL;
		FileCheck(std::string("Data\\PMACRects.ini"));
		loadCRects(std::string("Data\\PMACRects.ini"));
		pPrj=RAW_W1_PRJ;
		bExplosive=true;
		TPF=0.05f;
		if(wasInThere)
		{
			PW_Speed();
			bGotToDoAnimation=false;
		}
	}
}
void CPlayer::getShotgun()
{
	if(mWeaponState[WEP_SHOTGUN]==WEPSTATE_ENABLED)
	{
		if(pMovement!=NULL)
		{
			RAW_PLAYER_MOVEMENT_ANIMATION_SHOTGUN->SetFrame(pMovement->GetFrameId());
		}
		for(register int i=0;i<WEAPONS;++i)
		{
			if(mWeaponState[i]!=WEPSTATE_DISABLED)
			{
				mWeaponState[i]=WEPSTATE_ENABLED;
			}
		}
		bool wasInThere=false;
		if(CurrPW==PW_SPEED)
		{
			wasInThere=true;
			PW_Armor();
		}
		mWeaponState[WEP_SHOTGUN]=WEPSTATE_CHOOSED;
		mWeaponId=WEP_SHOTGUN;
		FileCheck(std::string("Data\\WEPDATASHOTGUN.ini"));
		loadWeaponData(std::string("Data\\WEPDATASHOTGUN.ini"));
		pMovement=RAW_PLAYER_MOVEMENT_ANIMATION_SHOTGUN;
		FileCheck(std::string("Data\\PMACRects.ini"));
		loadCRects(std::string("Data\\PMACRects.ini"));
		pPrj=RAW_W1_PRJ;
		bExplosive=true;
		TPF=0.05f;
		if(wasInThere)
		{
			PW_Speed();
			bGotToDoAnimation=false;
		}
	}
}	
void CPlayer::getBazooka()
{
	if(mWeaponState[WEP_BAZOOKA]==WEPSTATE_ENABLED)
	{
		if(pMovement!=NULL)
		{
			RAW_PLAYER_MOVEMENT_ANIMATION_BAZOOKA->SetFrame(pMovement->GetFrameId());
		}
		for(register int i=0;i<WEAPONS;++i)
		{
			if(mWeaponState[i]!=WEPSTATE_DISABLED)
			{
				mWeaponState[i]=WEPSTATE_ENABLED;
			}
		}
		bool wasInThere=false;
		if(CurrPW==PW_SPEED)
		{
			wasInThere=true;
			PW_Armor();
		}
		mWeaponState[WEP_BAZOOKA]=WEPSTATE_CHOOSED;
		mWeaponId=WEP_BAZOOKA;
		FileCheck(std::string("Data\\WEPDATABAZOOKA.ini"));
		loadWeaponData(std::string("Data\\WEPDATABAZOOKA.ini"));
		pMovement=RAW_PLAYER_MOVEMENT_ANIMATION_BAZOOKA;
		FileCheck(std::string("Data\\PMACRects.ini"));
		loadCRects(std::string("Data\\PMACRects.ini"));
		pPrj=RAW_W3_PRJ;
		bExplosive=true;
		TPF=0.05f;
		if(wasInThere)
		{
			PW_Speed();
			bGotToDoAnimation=false;
		}
	}
}
void CPlayer::reupdateWep()
{
	switch(mWeaponId)
	{
		case WEP_PISTOL:
			getPistol();
		break;
		case WEP_RIFLE:
			getRifle();
		break;
		case WEP_SHOTGUN:
			getShotgun();
		break;
		case WEP_BAZOOKA:
			getBazooka();
		break;
	}
}
