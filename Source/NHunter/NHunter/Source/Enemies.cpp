#include"..\\Includes\\Main.h"
#include"..\\Includes\\Enemies.h"
#include<cmath>
#define	BOSSHP		7500
#define BOSSHALFHP	BOSSHP/2
#define BOSS25HP	BOSSHP/4

extern bool					g_bCheats[MAXCHEATS];
extern Sprite *RAW_W2_PRJ;
extern AnimatedSprite 		*RAW_TURRET1;
extern Vec2                 g_PointOfView;
extern bool CollideMap(RECT CRect);
extern std::vector<Stuff*>  g_Stuff;
extern CPlayer		        *g_Player;
extern std::vector<RECT> 	g_CRect;
extern Sprite				*RAW_REPAIRKIT;
//#define BLACK RGB(0,0,0)
//#define WHITE RGB(255,255,255)


Enemy::Enemy(){
mSprite  = NULL;
mElapsed = 0;
mFrame   = 1;
mHeat   = 0;
mVelocity.x  = 150.f;
mVelocity.y  = 0.f;
mIsDying     = false;
isElevator  = false;
mReadyToFire = false;
FOT=FOT_ENEMY;
}

Enemy::~Enemy(){}

void Enemy::Draw(HDC hBackBuffer){

	if(mIsDying)
		{
		if(mVelocity.x > 0){
			mDeathAnimationR->mPosition = mPosition;
			mDeathAnimationR->mPosition.x -=g_PointOfView.x;
			mDeathAnimationR->mPosition.y -=g_PointOfView.y;
			mDeathAnimationR->SetFrame(mFrame);
			mDeathAnimationR->draw(hBackBuffer);}
		else
			{mDeathAnimationL->mPosition  = mPosition;
			mDeathAnimationL->mPosition.x -=g_PointOfView.x;
			mDeathAnimationL->mPosition.y -=g_PointOfView.y;
			mDeathAnimationL->SetFrame(mFrame);
			mDeathAnimationL->draw(hBackBuffer);
			}
		return;
		}

	mSprite->mPosition = mPosition;
	mSprite->mPosition.x -= g_PointOfView.x;
	mSprite->mPosition.y -= g_PointOfView.y;
	mSprite->SetFrame(mFrame);
	mSprite->draw(hBackBuffer);
}
RECT Enemy::Rect(){
RECT Sol;
if(mIsDying){
	Sol.top    = (int)mPosition.y;
	Sol.left   = (int)mPosition.x;
	Sol.bottom = Sol.top  + mDeathAnimationL->height();
	Sol.right  = Sol.left + mDeathAnimationL->width();
	return Sol;}

Sol.top    = (int)mPosition.y;
Sol.left   = (int)mPosition.x;
Sol.bottom = Sol.top  + mSprite->height();
Sol.right  = Sol.left + mSprite->width();
return Sol;
}
bool Enemy::Update(double Delta){
double Dis;
//Acceleration OY
RECT check;
mVelocity.y += g_cGravity * Delta;
Dis = (mVelocity.y * Delta);
mPosition.y  += Dis;
check.top    = int(mPosition.y);
check.bottom = int( mPosition.y + mSprite->height())+1;
check.left   = int(mPosition.x )+50;
check.right  = int(mPosition.x+ mSprite->width())-50;

if(Collide(check) || CollideMap(check) || CollideBox(check))
	{mVelocity.y = 0;
	mPosition.y-= Dis ;
	}
//Death
if(mIsDying)
	{mElapsed += Delta;
	if(mElapsed >= mDelay){
		mFrame ++;
		mElapsed = 0;
	}
	if(mFrame > mDeathAnimationL->GetFrameCount())
		return true;
	return false;
	}
//Firig bullet
mHeat += Delta;
if(mHeat>= mHeatDelay){
	mReadyToFire = true;
	mHeat = 0;
	}
if(Aim(g_Player->CRect()))
	{if(mReadyToFire)
		Fire();
	return false;
	}

//Acceleration OX
if(mIsMoving)
if(mVelocity.y != 0)
{if(mVelocity.x >= 0 )
    mFrame = 1;
else
	mFrame = 21;
}
else{
	Dis = mVelocity.x * Delta;
	mPosition.x += Dis;
	check = Rect();
	if(CollideMap(check) || Collide(check) || CollideBox(check) || mPosition.x <= mLeftLimit || mPosition.x >= mRightLimit)
		{mVelocity.x *= -1.f;
		mPosition.x -= Dis;
		if(mFrame < 21)
			mFrame = 21;
		else
			mFrame = 1;
		}
	if(CollideMap(check) || Collide(check) || CollideBox(check) || mPosition.x <= mLeftLimit || mPosition.x >= mRightLimit){
		if(g_Player->mPosition.x > mPosition.x)
			mFrame = 1;
		else
			mFrame = 21;
		}
	else{
	mElapsed += Delta;
	if(mElapsed >= mDelay){
		mFrame++;
		if((mFrame >= 21 || mFrame < 1)&& mVelocity.x >= 0  )
			mFrame = 1;

		if((mFrame >= 41 || mFrame < 21)&& mVelocity.x <= 0 )
			mFrame = 21;

		mElapsed = 0;
		}
	}
}
return false;
}
bool Enemy::TakeDamage(int Damage,int PosX,int PosY){

PosX -= (int)mPosition.x;
PosY -= (int)mPosition.y;
if(mSprite->GetMaskPixel(PosX,PosY) != 0)
	return false;
if(!mIsDying)
	{
	Damage-=mArmor;
	if(Damage<0)Damage=0;
	mHP -= Damage;
	if(mHP <= 0){
		mFrame = 1;
		mIsDying = true;
		if(mDoor != NULL)
		mDoor->mActiveSwitches++;
		}
	return true;
	}
return false;
}
bool Enemy::TakeDamage(int Damage){
	if(!mIsDying)
	{
		if(g_bCheats[CHEAT_GOD] || g_bCheats[CHEAT_1HIT1KILL])
		{
			mFrame = 1;
			mIsDying=true;
			if(FOT==FOT_BOSS)
				MakeShake(120,30,0.1f);
			if(mDoor != NULL)
				mDoor->mActiveSwitches++;
			return true;
		}
		Damage-=mArmor;
		if(Damage<0)Damage=0;
		mHP -= Damage;
	if(mHP <= 0){
		mFrame = 1;
		mIsDying = true;
		if(FOT==FOT_BOSS)
			MakeShake(120,30,0.1f);
		if(mDoor != NULL)
		mDoor->mActiveSwitches++;
		}
	return true;
	}
	return false;}
bool Enemy::CheckCollision(RECT CPlayer){
return false;
}

bool Enemy::Aim(RECT CPlayer){
Vec2 PPos,EPos;

PPos.x = (CPlayer.left + CPlayer.right)/2;
PPos.y = (CPlayer.top  + CPlayer.bottom)/2;

RECT En;
En = Rect();
EPos.x = (En.left + En.right)/2;
EPos.y = (En.top  + En.bottom)/2;


double dist;
dist = (double)sqrtf((float)((PPos.x - EPos.x) * (PPos.x - EPos.x) + (PPos.y - EPos.y)*(PPos.y - EPos.y)));
if((EPos.y < PPos.y && PPos.y - EPos.y > mSprite->height()/2) || (EPos.y > PPos.y && EPos.y - PPos.y > mSprite->height()/2))return false;
if(dist <= mRange){
	if(PPos.x <= EPos.x)
		{if(mVelocity.x>0)
			mVelocity.x *= -1;
		mFrame = 21;}
	else
	{if(mVelocity.x<0)
			mVelocity.x *= -1;
		mFrame = 1;
	}
	return true;
	}
return false;
}


void Enemy::Fire(){

Vec2 PPos,EPos;
Vec2 Pos,Speed;
RECT En;
RECT CPlayer = g_Player->CRect();
PPos.x = (CPlayer.left + CPlayer.right)/2;
PPos.y = (CPlayer.top  + CPlayer.bottom)/2;


En = Rect();
EPos.x = (En.left + En.right)/2;
EPos.y = (En.top  + En.bottom)/2;
CProjectile *Bullet;
if(mFrame == 1 )
	{Pos.x =210 + mPosition.x;
	Pos.y = 54 + mPosition.y;
	Speed.x = mBulletSpeed;
	Speed.y = 0;}
else
	{Pos.x =  mPosition.x-10;
	Pos.y = 54   + mPosition.y;
	Speed.x = -mBulletSpeed;
	Speed.y = 0;}
mReadyToFire = false;
mHeat = 0;
Bullet = new CProjectile(Pos,Speed,mBulletSprite,mBulletMinDmg,mBulletMaxDmg,PRJ_NOSTUFF);
CProjectile::Add(Bullet);
}
bool SegInRect(Vec2 P1,Vec2 P2,RECT Rect){
	double T1,T2;
	Vec2 S1,S2;
	S1.x = Rect.left;
	S1.y = Rect.top;
	S2.x = Rect.right;
	S2.y = Rect.top;
	if(P1.x == P2.x && S1.x == S2.x )return false;
	
	T1 = (P2.y - P1.y)/(P2.x - P1.x);
	if(T1 < 0)
		T1 = -T1;
	T2 = (S2.y - S1.y)/(S2.x - S1.x);
	if(T1 - T2 <=  0.1 && T1 - T2 >= -0.1) return false;

	if(Sarrus(P2,P1,S1)*Sarrus(P2,P1,S2) <= 0 || Sarrus(S2,S1,P1)*Sarrus(S2,S1,P2) <= 0)
		return true;

	S1.x = Rect.right;
	S1.y = Rect.bottom;
	if(P1.x == P2.x && S1.x == S2.x )return false;
	
	T1 = (P2.y - P1.y)/(P2.x - P1.x);
	if(T1 < 0)
		T1 = -T1;
	T2 = (S2.y - S1.y)/(S2.x - S1.x);
	if(T1 - T2 <=  0.1 && T1 - T2 >= -0.1) return false;

	if(Sarrus(P2,P1,S1)*Sarrus(P2,P1,S2) <= 0 || Sarrus(S2,S1,P1)*Sarrus(S2,S1,P2) <= 0)
		return true;
	S2.x = Rect.left;
	S2.y = Rect.bottom;
	if(P1.x == P2.x && S1.x == S2.x )return false;
	T1 = (P2.y - P1.y)/(P2.x - P1.x);
	if(T1 < 0)
		T1 = -T1;
	T2 = (S2.y - S1.y)/(S2.x - S1.x);
	if(T1 - T2 <=  0.1 && T1 - T2 >= -0.1) return false;

	if(Sarrus(P2,P1,S1)*Sarrus(P2,P1,S2) <= 0 || Sarrus(S2,S1,P1)*Sarrus(S2,S1,P2) <= 0)
		return true;
	return false;
}


Boss::Boss():Enemy(){
	mIsMoving = true;
	FOT = FOT_BOSS;
	mPowerUp = false;
	mTurret  = false;
	mDoor    = NULL;
}

Boss::~Boss(){
}

bool Boss::Update(double Delta){
double Dis;

bool notAnymoreKit=true;
for(register int i=0;i<(int)g_Stuff.size();++i)
{
	if(g_Stuff[i]->FOT==FOT_REPAIRKIT)
	{
		notAnymoreKit=false;
		break;
	}
}

if(notAnymoreKit)
{
	repairKitTime+=Delta;
	if(repairKitTime>20.0f && notAnymoreKit)
	{
		repairKitTime=0.0f;
		AddRepairKit(93,702,RAW_REPAIRKIT,500);
		AddRepairKit(2048-93-30,702,RAW_REPAIRKIT,500);
	}
}

static double accdmg=0.0f;
const double DPS=100;
if(RectCollideRect(g_Player->CRect(),Rect()))
{
	accdmg+=Delta*DPS;
	if(accdmg>1)
	{
		g_Player->takeDamage((int)accdmg+30);
		accdmg=0;
	}
}




//turret update
if(mHP<=BOSSHALFHP && !mTurret)
		{mTurret = true;
		for(register int i = 0 ; i < (int)g_Stuff.size();++i)
			if(g_Stuff[i]->FOT == FOT_TURRET)
				{Turret *mTurr = (Turret*) (g_Stuff[i]);
				mTurr->P1.x = mTurr->P1.y = 0;
				mTurr->P2.x = 2048;
				mTurr->P2.y = 0;
				mTurr->P2.x = 2048;
				mTurr->P3.x = 2048;
				mTurr->P3.y = 768;
				mTurr->P4.x = 0;
				mTurr->P4.y = 768;
				}
		CheckPoint::Save();
}
//PowerUp
if(mHP <= BOSS25HP && !mPowerUp)
	{mPowerUp = true;
	mVelocity.x*= 1.25f;
	mDelay -= mDelay*0.25f;
	mBulletMinDmg += mBulletMinDmg/4;
	mBulletMaxDmg += mBulletMinDmg/4;
	for(register int i = 0 ; i < (int)g_Stuff.size();++i)
			if(g_Stuff[i]->FOT == FOT_TURRET)
			{Turret *mTurr = (Turret*) (g_Stuff[i]);
			mTurr->mFireDelay=(mTurr->mFireDelay*1.25f);
			mTurr->mBulletSpeed=(mTurr->mBulletSpeed*1.25f);
			mTurr->mMinDamage=(int)(mTurr->mMinDamage*1.25f);
			mTurr->mMaxDamage=(int)(mTurr->mMaxDamage*1.25f);
			}
	CheckPoint::Save();
	}

//Acceleration OY
RECT check;
mVelocity.y += g_cGravity * Delta;
Dis = (mVelocity.y * Delta);
mPosition.y  += Dis;
check.top    = int(mPosition.y);
check.bottom = int( mPosition.y + mSprite->height())+1;
check.left   = int(mPosition.x )+50;
check.right  = int(mPosition.x+ mSprite->width())-50;

if(Collide(check) || CollideMap(check) || CollideBox(check))
	{mVelocity.y = 0;
	mPosition.y-= Dis ;
	}

//Death
if(mIsDying)
	{mElapsed += Delta;
	if(mElapsed >= mDelay){
		mFrame ++;
		mElapsed = 0;
	}
	if(mFrame > mDeathAnimationL->GetFrameCount())
		return true;
	return false;
	}

//moving

if(mIsMoving){
	Dis = mVelocity.x * Delta;
	mPosition.x += Dis;
	check = Rect();
	check.left+=50;
	check.right-=50;
	if(CollideMap(check) || CollideBox(check) || Collide(check))
		{mPosition.x -= Dis;
		if(mVelocity.y == 0)
			mVelocity.y = -1000;
		
		if(mVelocity.x > 0)
			mPosition.x-=1;
		else
			mPosition.x+=1;
		}
	else{
	mElapsed += Delta;
	if(mElapsed >= mDelay){
		mFrame++;
		if((mFrame >= 21 || mFrame < 1)&& mVelocity.x >= 0  )
			mFrame = 1;

		if((mFrame >= 41 || mFrame < 21)&& mVelocity.x <= 0 )
			mFrame = 21;

		mElapsed = 0;
		}

	if(mPosition.x >= mPoz3 )
	{
		mIsMoving = false;
		mListIndex = 0;
		mNumberOfShots = 3;
		mLastPoz = 3;
		mSameShots = 3;
		mCurrShot = 0;
		mVelocity.x *= -1.f;
		mClockWise = true;
		mHeat = mHeatDelay;
	}
	else
	if(mPosition.x <= mPoz1){
		mClockWise = false;
		mListIndex = 5;
		mNumberOfShots = 3;
		mSameShots = 3;
		mCurrShot = 0;
		mVelocity.x *= -1.f;
		mLastPoz = 1;
		mIsMoving = false;
		mHeat = mHeatDelay;
		}
	else
		if(mPosition.x <= mPoz2 && mPoz2-mPosition.x <= mSprite->width()&& mLastPoz!=2){
			mIsMoving = false;
			mLastPoz = 2;
			mNumberOfShots = 12;
			mSameShots     = 2;
			mCurrShot = 0;
			mHeat = mHeatDelay;
		if(mVelocity.x > 0 )
			{mListIndex = 0;
			mClockWise = true;
			}
		else
			{mListIndex = 5;
			mClockWise = false;
			}
		}
	}
}
//firing
else{
	mHeat += Delta;
	if(mHeat >= mHeatDelay){
		if(!Aim()){
			mIsMoving = true;
			if(mVelocity.x>0)
				mFrame = 1;
			else
				mFrame = 21;
			}
		mHeat = 0;
		}
	else
		Fire();
	}

if(mVelocity.y < 0 && mVelocity.x > 0)
	mFrame = 41;
else
if(mVelocity.y < 0 && mVelocity.x < 0)
	mFrame = 61;
else
if(mVelocity.y > 0 && mVelocity.x > 0)
	mFrame = 42;
else
if(mVelocity.y > 0 && mVelocity.x < 0)
	mFrame = 62;

return false;
}
void Boss::Fire(){

mBulletPos.x += mPosition.x;
mBulletPos.y += mPosition.y;
CProjectile *Bullet;
Bullet = new CProjectile(mBulletPos,mBulletDir,mBulletSprite,mBulletMinDmg,mBulletMaxDmg,PRJ_NOSTUFF);
CProjectile::Add(Bullet);
}
bool Boss::CheckCollision(RECT CPlayer){
return false;}

bool Boss::Aim(){
	mCurrShot++;
	mNumberOfShots--;
	if(mNumberOfShots<0)
		return false;
	
	


	
	mFrame =  mFrameList[mListIndex];
	mBulletDir.x = mBulletSpeed*cosf((float)(PI*mBulletAngList[mListIndex])/180);
	mBulletDir.y = -1*mBulletSpeed*sinf(((float)PI*mBulletAngList[mListIndex])/180);
	mBulletPos = mBulletPosList[mListIndex];
	if(mCurrShot >= mSameShots){
	mCurrShot = 0;
	if(mClockWise)
			mListIndex++;
	else
		mListIndex--;}
	
	return true;
	
}

void AddEnemyT1(int PozX,int PozY,int LeftLimit,int RightLimit,MovingDoor *Door,bool IsMoving)
{
static RECT Rect;
Rect.top = Rect.left = 0;
Rect.bottom = Rect.right = 200;
static AnimatedSprite *BSprite = new AnimatedSprite("Data\\Sprites\\E1.bmp","Data\\Sprites\\E1M.bmp",Rect,80);
Rect.bottom = Rect.right = 200;
static AnimatedSprite *Death  = new AnimatedSprite("Data\\Sprites\\E1DR.bmp","Data\\Sprites\\E1DRM.bmp",Rect,24);
static AnimatedSprite *Death2  = new AnimatedSprite("Data\\Sprites\\E1DL.bmp","Data\\Sprites\\E1DLM.bmp",Rect,24);
static Sprite *Bullet = new Sprite("Data\\Sprites\\PRJ3.bmp","Data\\Sprites\\PRJ3M.bmp");

Enemy *mEnemy;
mEnemy = new Enemy;
mEnemy->mSprite = BSprite;
mEnemy->mPosition.x = PozX;
mEnemy->mPosition.y = PozY;
mEnemy->mLeftLimit  = LeftLimit;
mEnemy->mRightLimit = RightLimit;
mEnemy->mHP         = 700;
mEnemy->mDelay      = 0.07f;
mEnemy->mHeatDelay  = 0.5f;
mEnemy->mRange      = 500.f;
mEnemy->mBulletMaxDmg  = 100;
mEnemy->mBulletMinDmg  = 50;
mEnemy->mBulletSpeed=500.f;
mEnemy->mBulletSprite= Bullet; 
mEnemy->mDeathAnimationR = Death;
mEnemy->mDeathAnimationL = Death2;
mEnemy->mDoor            = Door;
mEnemy->mArmor			= 0;
if(Door != NULL)
Door->mNumberOfSwitches++;
mEnemy->mIsMoving = IsMoving;

g_Stuff.push_back(mEnemy);
}

void AddBoss(){
static RECT Rect;
Rect.top = Rect.left = 0;
Rect.bottom = Rect.right = 200;
static AnimatedSprite *BSprite = new AnimatedSprite("Data\\Sprites\\BOSS.bmp","Data\\Sprites\\BOSSM.bmp",Rect,80);
Rect.bottom = Rect.right = 200;
static AnimatedSprite *Death  = new AnimatedSprite("Data\\Sprites\\BOSSDR.bmp","Data\\Sprites\\BOSSDRM.bmp",Rect,24);
static AnimatedSprite *Death2  = new AnimatedSprite("Data\\Sprites\\BOSSDL.bmp","Data\\Sprites\\BOSSDLM.bmp",Rect,24);
static Sprite *Bullet = new Sprite("Data\\Sprites\\PRJ3.bmp","Data\\Sprites\\PRJ3M.bmp");
static int List[] = {21,63,64,44,43,1};
static float Ang[]  = {180.f,139.f,90.f,90.f,41.f,0.f};
static Vec2 Pos[6] ;
Pos[0].x = 38;
Pos[0].y = 61;
Pos[1].x = 32;
Pos[1].y = 4;
Pos[2].x = 90;
Pos[2].y = 0;
Pos[3].x = 110;
Pos[3].y = 1;
Pos[4].x = 166;
Pos[4].y = 3;
Pos[5].x = 162;
Pos[5].y = 63;
Boss *mBoss = new Boss();
mBoss->mSprite = BSprite;
mBoss->mDeathAnimationR = Death;
mBoss->mDeathAnimationL = Death2;
mBoss->mBulletMaxDmg    = 250;
mBoss->mBulletMinDmg    = 200;
mBoss->mBulletSpeed     = 700;
mBoss->mDelay           = 0.15f;
mBoss->mHeatDelay       = 0.5f;
mBoss->mHP              = BOSSHP;
mBoss->mBulletSprite    = Bullet;
mBoss->mPoz1            = 100;
mBoss->mPoz3            = 1700;
mBoss->mPoz2            = 1024;
mBoss->mPosition.x      = 2048-500;
mBoss->mPosition.y      = 538;
mBoss->mFrameList       = List;
mBoss->mBulletAngList   = Ang;
mBoss->mBulletPosList   = Pos;
mBoss->mArmor			= 150;
mBoss->repairKitTime	= 0.0f;
g_Stuff.push_back(mBoss);
}
