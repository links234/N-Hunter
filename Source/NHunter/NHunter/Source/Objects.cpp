#include"..\\Includes\\Objects.h"
#include<cmath>
using namespace std;

extern bool CollideMap(RECT CRect);
extern Vec2                 g_PointOfView;
extern std::vector<Stuff*>  g_Stuff;
extern std::vector<MovableObjects*> g_Box;
extern CPlayer		  *g_Player, *g_Player2;
extern bool g_bGameFinished;
extern bool	g_bCheats[MAXCHEATS];

#define BoxSpeed    0.01f
#define FloorSpeed  0.075f
#define DoorSpeed   0.1f
#define DummySpeed  0.07f
#define SpikeSpeed  0.2f
#define TurretSpeed 0.1f

long long Sarrus(Vec2 P1,Vec2 P2,Vec2 P3){
long long sol = (long long)(P1.x * P2.y + P2.x*P3.y + P1.y*P3.x - 
				P3.x*P2.y - P3.y*P1.x - P1.y*P2.x);
return sol;
}

Object::Object(){
mIsSolid       = false;
bool mIsMoving = false;
mSprite = NULL;
mFrame  = 1;
}



Object::~Object(){
DeleteObject(mSprite); 

}
void Object::Draw(HDC hBackBuffer){
	if(mFrame == mNumberOfFrames)
		return ;
	mSprite->SetFrame(mFrame);
	mSprite->mPosition = mPosition;
	mSprite->mPosition.x -= g_PointOfView.x;
	mSprite->mPosition.y -= g_PointOfView.y;
	mSprite->draw(hBackBuffer);
}
//Movable Objects
MovableObjects::MovableObjects():Object(){}

MovableObjects::~MovableObjects(){}

void MovableObjects::TakeDamage(int Damage){
if(!mIsSolid)
	return ;
if(!mIsDestructible)
	return;
mHP -= Damage;
if(mHP <= 0 ){
	mIsAnimated = true;
	}
}

bool MovableObjects::Move(RECT Player){
	if(!mMovable || (g_Player->CurrPW!=3 && !g_bCheats[CHEAT_STRENGTH]))
	return !RectCollideRect(Player,Rect());

int D;
	RECT check;
	check.top    = int(mPosition.y);
	check.bottom = int( mPosition.y + mSprite->height());
	check.left   = int(mPosition.x);
	check.right  = int(mPosition.x+ mSprite->width())+1;
	if(RectCollideRect(Player, check))
	{
		if(Player.left+1 < check.left && check.left < Player.right+1)
		{D = check.left;
		mPosition.x = Player.right;
		
		check.top    = int(mPosition.y)+1;
		check.bottom = int( mPosition.y + mSprite->height())-1;
		check.left   = int(mPosition.x);
		check.right  = int(mPosition.x + mSprite->width());
		mPosition.x = -1000;
		if(CollideMap(check) || Collide(check) || CollideBox(check))
{
			mPosition.x = D;
			return false;}
		else
			{
			mPosition.x = Player.right+1;
			return true;}
		}
	else
		if(Player.left < check.right && check.right < Player.right )
		{D = check.left;
		mPosition.x = Player.left - mSprite->width()-1;
		
		check.top    = int(mPosition.y)+1;
		check.bottom = int( mPosition.y + mSprite->height())-1;
		check.left   = int(mPosition.x);
		check.right  = int(mPosition.x + mSprite->width())-1;
		mPosition.x = -1000;
		if(CollideMap(check) || Collide(check) || CollideBox(check))
			{
			mPosition.x = D;
			return false;}
		else
			{
			mPosition.x = Player.left - mSprite->width()-1;
			return true;}
		}
	}
	return true;
}


RECT MovableObjects::Rect(){
RECT Sol;
Sol.top    = (int)mPosition.y;
Sol.left   = (int)mPosition.x;
Sol.bottom = Sol.top  + mSprite->height();
Sol.right  = Sol.left + mSprite->width();
return Sol;
}

bool MovableObjects::UpdatePoz(double Delta){
double Dis,Posy;
RECT check;
Dis = (mVelocity.y * Delta);
Posy = mPosition.y;
mPosition.y += Dis;
check.top    = int(mPosition.y);
check.bottom = int( mPosition.y + mSprite->height())+1;
check.left   = int(mPosition.x );
check.right  = int(mPosition.x+ mSprite->width());
mPosition.y = - 1000;

if(Collide(check) || CollideMap(check) || CollideBox(check))
	{mVelocity.y = 0;
	mPosition.y= Posy ;
	mSprite->mPosition = mPosition;
	}
else{
	mPosition.y = Dis + Posy;
	mSprite->mPosition = mPosition;
	}


if(mHP <= 0 )
	mIsAnimated = true;

if(!mIsAnimated)
	return false;

mElapsed += Delta;
if(mElapsed >= mSpeed)
	{mElapsed = 0;
	if(mFrame<=mNumberOfFrames)
		mFrame++;
	else{
		mIsAnimated = false;
		mIsSolid   = false;
		}
	
	}
if(mFrame >= mNumberOfFrames)
	return true;
return false;
}

//other stuff
bool Stuff::CheckCollision(RECT CPlayer){
return false;
}
bool Stuff::Update(double Delta){return false;}
bool Stuff::TakeDamage(int damage,int PosX,int PosY){return false;}
bool Stuff::TakeDamage(int damage){return false;}
void Stuff::Draw(HDC hBackBuffer){}
RECT Stuff::Rect(){
	RECT sol;
	sol.top = sol.bottom = sol.left = sol.right = 0;
	return sol;}
//falling floor
FallingFloor::FallingFloor(){
mSpeed      = 0.f;
mIsAnimated = false;
mSprite     = NULL;
mFrame      = 1;
mIsSolid    = true;
mTimeElapsed = 0.f;
isElevator = false;
FOT=FOT_FALLINGFLOOR;
}

FallingFloor::~FallingFloor(){
}
bool FallingFloor::TakeDamage(int damage,int PosX,int PosY){
return true;
}
bool FallingFloor::TakeDamage(int damage){
return true;
}
RECT FallingFloor::Rect(){
RECT Sol;
Sol.top   = (int)mPosition.y;
Sol.left  = (int)mPosition.x;
Sol.right = (int)Sol.left + mSprite->width();
Sol.bottom= (int)Sol.top  + mSprite->height();
return Sol;
}
bool FallingFloor::CheckCollision(RECT CPlayer){
if(!mIsSolid)
	return false;

RECT check;
check.top   = (int)mPosition.y;
check.left  = (int)mPosition.x;
check.bottom= check.top + mSprite->height();
check.right = check.left+ mSprite->width();

POINT p1,p2;
p1.y = CPlayer.bottom;
p1.x = CPlayer.left;
p2.y = CPlayer.bottom;
p2.x = CPlayer.right;


if(PointInRectangle(check,p1) || PointInRectangle(check,p2))
	{mIsAnimated = true;
	return true;
	}
else
	return false;
}

void FallingFloor::Draw(HDC hBackBuffer){
if(!mIsSolid)
	return ;

	if(mFrame <= mNumberOfFrames){
		mSprite->SetFrame(mFrame);
		mSprite->mPosition = mPosition;
		mSprite->mPosition.x -= g_PointOfView.x;
		mPosition.y -= g_PointOfView.y;
		mSprite->draw(hBackBuffer);
		}
}
bool FallingFloor::Update(double Delta){
if(!mIsAnimated)return false;

mTimeElapsed += Delta;

if(mTimeElapsed > mSpeed)
	{mTimeElapsed = 0;
	mFrame++;
	mSprite->SetFrame(mFrame);
	if(mFrame == mNumberOfFrames){
		mIsAnimated = false;
		}
	}
if(mFrame == mNumberOfFrames)
	return true;
return false;
}
//moving  door
MovingDoor::MovingDoor(){
mSpeed      = 0.f;
mIsAnimated = false;
mIsSolid    = true;
mTimeElapsed= 0.f;
mUpRDown    = 1;
mFrame      = 1;
mActiveSwitches = 0;
mNumberOfSwitches = 0;
isElevator = false;
FOT=FOT_MOVINGDOOR;
}

MovingDoor::~MovingDoor(){
}

bool MovingDoor::TakeDamage(int damage,int PosX,int PosY){
if(mIsSolid)
return true;
else
return false;
}

bool MovingDoor::TakeDamage(int damage){
if(mIsSolid)
return true;
else
return false;
}
RECT MovingDoor::Rect(){
RECT Sol;
Sol.top   = (int)mPosition.y;
Sol.left  = (int)mPosition.x;
Sol.right = (int)Sol.left + mSprite->width();
Sol.bottom= (int)Sol.top  + mSprite->height();
return Sol;
}
bool MovingDoor::Update(double Delta){
	if(mActiveSwitches == mNumberOfSwitches){
		if(mFrame != mNumberOfFrames){
			mIsAnimated = true;
			mUpRDown    = 1;
		}
	}
	else
	if(mFrame != 1){
		mIsAnimated = true;
		mUpRDown = -1;
		mIsSolid = true;
	}

if(!mIsAnimated)return false;

mTimeElapsed += Delta;

if(mTimeElapsed > mSpeed)
	{mTimeElapsed = 0;
	mFrame += mUpRDown;
	if(mFrame == mNumberOfFrames){
		mIsAnimated = false;
		mIsSolid    = false;
		}
	if(mFrame == 1)
		{mIsAnimated = false;
		}
	}
return false;
}

void MovingDoor::Draw(HDC hBackBuffer){
if(!mIsSolid)return ;

if(mFrame <=mNumberOfFrames){
		mSprite->SetFrame(mFrame);
		mSprite->mPosition = mPosition;
		mSprite->mPosition.x -= g_PointOfView.x;
		mPosition.y -= g_PointOfView.y;
		mSprite->draw(hBackBuffer);
		}
}

bool MovingDoor::CheckCollision(RECT CPlayer){
if(!mIsSolid)
	return false;

RECT check;
check.top   = (int)mPosition.y;
check.left  = (int)mPosition.x;
check.bottom= check.top + mSprite->height();
check.right = check.left+ mSprite->width();
return RectCollideRect(check,CPlayer);
}
//switch
Switch::Switch(){
mState = false;
mFrame = 1;
mKeyDown  = false;
isElevator = false;
mIsSolid = false;
FOT=FOT_SWITCH;
}

Switch::~Switch(){
}
RECT Switch::Rect(){
RECT Sol;
Sol.top   = (int)mPosition.y;
Sol.left  = (int)mPosition.x;
Sol.right = Sol.left + mSprite->width();
Sol.bottom= Sol.top  + mSprite->height();
return Sol;
}
void Switch::Draw(HDC hBackBuffer){
	mSprite->SetFrame(mFrame);
	mSprite->mPosition = mPosition;
	mSprite->mPosition.x -= g_PointOfView.x;
	mSprite->mPosition.y -= g_PointOfView.y;
	mSprite->draw(hBackBuffer);
}

bool Switch::Update(double Delta)
{RECT check,CPlayer;
CPlayer = g_Player->CRect();
check.top   = (int)mPosition.y;
check.left  = (int)mPosition.x;
check.bottom= check.top + mSprite->height();
check.right = check.left+ mSprite->width();
if(RectCollideRect(check,CPlayer)){
	static UCHAR pKeyBuffer[ 256 ];
	if ( !GetKeyboardState( pKeyBuffer ) ) throw FatalError("Failed to get keyboard input");
	if( (pKeyBuffer[ 'E'  ] & 0xF0) ){

		if(!mKeyDown){
			mKeyDown = true;
			if(mState == true)
				{mState = false;
				mDoor->mActiveSwitches--;
				mFrame = 1;
				}
			else
				{mState = true;
				mDoor->mActiveSwitches++;
				mFrame = 2;
				}
			}
	}
	else
		mKeyDown = false;

}
return false;
}

//finishzone
FinishZone::FinishZone()
{
	isElevator = false;
	mIsSolid = false;
	FOT=FOT_FINISHZONE;
}
FinishZone::~FinishZone()
{

}
void FinishZone::Draw(HDC hBackBuffer)
{

}
bool FinishZone::Update(double Delta)
{
	if(RectCollideRect(g_Player->CRect(),RC))
		g_bGameFinished=true;
	return false;
}
RECT FinishZone::Rect()
{
	return RC;
}

//turret
Turret::Turret(){
mSprite = NULL;
mFrame = 1;
mElapsed = 0;
isElevator = false;
mIsSolid = false;
FOT=FOT_TURRET;
}

Turret::~Turret(){}

RECT Turret::Rect(){
RECT sol;
sol.top    = (int)mPosition.y;
sol.left   = (int)mPosition.x;
sol.right  = sol.left + mSprite->width();
sol.bottom = sol.top  + mSprite->height();
return sol;
}

bool Turret::Update(double Delta){

mElapsed += Delta;
//mFireElapsed += Delta;

if(mElapsed>=TurretSpeed)
	{mElapsed = 0;
    mFrame++;
	if(mFrame>mSprite->GetFrameCount())
		mFrame =1;
	}

mFireElapsed += Delta;

if(mFireElapsed > mFireDelay)
	{mReadyToFire = true;
	mFireElapsed = 0;
	}

RECT CPlayer = g_Player->CRect();
Vec2 a,b,c,d;
a.x = CPlayer.left;
a.y = CPlayer.top;
b.x = CPlayer.right;
b.y = a.y;
c.x = a.x;
c.y = CPlayer.bottom;
d.x = b.x;
d.y = c.y;



if(mReadyToFire && Check(a,b,c,d))
{
if(!mIsTracking)
	{
	Vec2 Pos;
	Pos.x = mPosition.x + mSprite->width()/2;
	Pos.y = mPosition.y + mSprite->height()/2;
	Pos.x -= mBulletSprite->width()/2;
	Pos.y -= mBulletSprite->height()/2;
	Vec2 Speed;
	Speed.x = 0;
	Speed.y = mBulletSpeed;
	CProjectile *Bullet;
	Bullet = new CProjectile(Pos,Speed,mBulletSprite,mMinDamage,mMaxDamage,PRJ_NOSTUFF);
	CProjectile::Add(Bullet);
	Speed.x = mBulletSpeed/sqrtf(2);
	Speed.y = mBulletSpeed/sqrtf(2);
	Bullet = new CProjectile(Pos,Speed,mBulletSprite,mMinDamage,mMaxDamage,PRJ_NOSTUFF);
	CProjectile::Add(Bullet);
	Speed.x = -mBulletSpeed/sqrtf(2);
	Speed.y = mBulletSpeed/sqrtf(2);
	Bullet = new CProjectile(Pos,Speed,mBulletSprite,mMinDamage,mMaxDamage,PRJ_NOSTUFF);
	CProjectile::Add(Bullet);
	Speed.x = mBulletSpeed/sqrtf(5);
	Speed.y = 2*mBulletSpeed/sqrtf(5);
	Bullet = new CProjectile(Pos,Speed,mBulletSprite,mMinDamage,mMaxDamage,PRJ_NOSTUFF);
	CProjectile::Add(Bullet);
	Speed.x = -mBulletSpeed/sqrtf(5);
	Speed.y = 2*mBulletSpeed/sqrtf(5);
	Bullet = new CProjectile(Pos,Speed,mBulletSprite,mMinDamage,mMaxDamage,PRJ_NOSTUFF);
	CProjectile::Add(Bullet);
	}
else
	{Vec2 Pos;
	Pos.x = mPosition.x + mSprite->width()/2;
	Pos.y = mPosition.y + mSprite->height()/2;
	Vec2 PPos;
	RECT CPlayer = g_Player->CRect();
	PPos.x = (CPlayer.left + CPlayer.right)/2;
	PPos.y = (CPlayer.top  + CPlayer.bottom)/2;
	Vec2 Speed;
	if(Pos.x > PPos.x )
		Speed.x = -1;
	else
		Speed.x = 1;

	if(Pos.y > PPos.y )
		Speed.y = -1;
	else
		Speed.y = 1;

	double a;
	a = (PPos.x - Pos.x)/(PPos.y - Pos.y);
	double b;
	b = (double)(mBulletSpeed/sqrtf((float)(a*a+1)));
	Speed.y *= b;
	Speed.x *= sqrtf((float)(mBulletSpeed * mBulletSpeed - b*b));
	CProjectile *Bullet;
	Bullet = new CProjectile(Pos,Speed,mBulletSprite,mMinDamage,mMaxDamage,PRJ_NOSTUFF);
	CProjectile::Add(Bullet);
	}
mReadyToFire = false;
}

return false;
}
bool Turret::Check(Vec2 a, Vec2 b, Vec2 c, Vec2 d){
long long a1,a2,a3,at;
at = (long long)labs((long)Sarrus(P1,P2,P3));
a1 = (long long)labs((long)Sarrus(P1,P2,a));
a2 = (long long)labs((long)Sarrus(P2,P3,a));
a3 = (long long)labs((long)Sarrus(P3,P1,a));
if(a1 + a2 + a3 == at)return true;

at = (long long)labs((long)Sarrus(P1,P2,P3));
a1 = (long long)labs((long)Sarrus(P1,P2,b));
a2 = (long long)labs((long)Sarrus(P2,P3,b));
a3 = (long long)labs((long)Sarrus(P3,P1,b));
if(a1 + a2 + a3 == at)return true;

at = (long long)labs((long)Sarrus(P1,P2,P3));
a1 = (long long)labs((long)Sarrus(P1,P2,c));
a2 = (long long)labs((long)Sarrus(P2,P3,c));
a3 = (long long)labs((long)Sarrus(P3,P1,c));
if(a1 + a2 + a3 == at)return true;

at = (long long)labs((long)Sarrus(P1,P2,P3));
a1 = (long long)labs((long)Sarrus(P1,P2,d));
a2 = (long long)labs((long)Sarrus(P2,P3,d));
a3 = (long long)labs((long)Sarrus(P3,P1,d));
if(a1 + a2 + a3 == at)return true;

at = (long long)labs((long)Sarrus(P1,P3,P4));
a1 = (long long)labs((long)Sarrus(P1,P3,a));
a2 = (long long)labs((long)Sarrus(P3,P4,a));
a3 = (long long)labs((long)Sarrus(P4,P1,a));
if(a1 + a2 + a3 == at)return true;

at = (long long)labs((long)Sarrus(P1,P3,P4));
a1 = (long long)labs((long)Sarrus(P1,P3,b));
a2 = (long long)labs((long)Sarrus(P3,P4,b));
a3 = (long long)labs((long)Sarrus(P4,P1,b));
if(a1 + a2 + a3 == at)return true;

at = (long long)labs((long)Sarrus(P1,P3,P4));
a1 = (long long)labs((long)Sarrus(P1,P3,c));
a2 = (long long)labs((long)Sarrus(P3,P4,c));
a3 = (long long)labs((long)Sarrus(P4,P1,c));
if(a1 + a2 + a3 == at)return true;

at = (long long)labs((long)Sarrus(P1,P3,P4));
a1 = (long long)labs((long)Sarrus(P1,P3,d));
a2 = (long long)labs((long)Sarrus(P3,P4,d));
a3 = (long long)labs((long)Sarrus(P4,P1,d));
if(a1 + a2 + a3 == at)return true;

return false;
}
bool Turret::CheckCollision(RECT CPlayer){


return false;
}
void Turret::Draw(HDC hBackBuffer){
	mSprite->mPosition = mPosition;
	mSprite->mPosition.x -= g_PointOfView.x;
	mSprite->mPosition.y -= g_PointOfView.y;
	mSprite->SetFrame(mFrame);
	mSprite->draw(hBackBuffer);
}
//Dummy
Dummy::Dummy(){
	mDoor    = NULL;
	mFrame   = 1;
	mElapsed = 0;
	mIsAnimated = false;
	mVelocity.x = mVelocity.y  = 0; 
	isElevator = false;
	mIsSolid = true;
	FOT=FOT_DUMMY;
}

Dummy::~Dummy(){}

void Dummy::Draw(HDC hBackBuffer){
	mSprite->mPosition = mPosition;
	mSprite->mPosition.x -= g_PointOfView.x;
	mSprite->mPosition.y -= g_PointOfView.y-13;
	mSprite->SetFrame(mFrame);
	mSprite->draw(hBackBuffer);
}

bool Dummy::Update(double Delta){
double Dis,Posy;
RECT check;
mVelocity.y += g_cGravity * Delta;
Dis = (mVelocity.y * Delta);
Posy = mPosition.y;
mPosition.y += Dis;
check.top    = int(mPosition.y);
check.bottom = int( mPosition.y + mSprite->height())+1;
check.left   = int(mPosition.x );
check.right  = int(mPosition.x+ mSprite->width());
mPosition.y = - 1000;

if(Collide(check) || CollideMap(check) || CollideBox(check))
	{mVelocity.y = 0;
	mPosition.y= Posy ;
	}
else{
	mPosition.y = Dis + Posy;
	}




mElapsed += Delta;
if(mElapsed >= mSpeed  )
	{
	if(mIsAnimated)
	mFrame ++;
	mElapsed = 0;
	}
if(mFrame > mNumberOfFrames)
	return true;
return false;
}
bool Dummy::TakeDamage(int damage,int PosX,int PosY){
PosX -= (int)mPosition.x;
PosY -= (int)mPosition.y;
if(mSprite->GetMaskPixel(PosX,PosY) != 0)
		return false;
if(mIsAnimated)return false;
mHP -= damage;
if(mHP <= 0){
	mIsAnimated = true;
	mDoor->mActiveSwitches++;
	}
return true;
}
bool Dummy::TakeDamage(int damage){
if(mIsAnimated)return false;
mHP -= damage;
if(mHP <= 0){
	mIsAnimated = true;
	mDoor->mActiveSwitches++;
	}
return true;
}
RECT Dummy::Rect(){
RECT sol;
sol.top    = (int)mPosition.y;
sol.left   = (int)mPosition.x;
sol.right  = sol.left + mSprite->width();
sol.bottom = sol.top  + mSprite->height();
return sol;
}

//Platform
Platform::Platform(){
	mSprite = NULL;
	mFrame = 1;
	isElevator = false;
	mIsSolid = false;
	FOT=FOT_PLATFORM;
}
Platform::~Platform(){}

RECT Platform::Rect(){
RECT sol;
sol.top    = (int)mPosition.y;
sol.left   = (int)mPosition.x;
sol.right  = sol.left + mSprite->width();
sol.bottom = sol.top  + mSprite->height();
return sol;
}
void Platform::Draw(HDC hBackBuffer){
	mSprite->mPosition = mPosition;
	mSprite->mPosition.x -= g_PointOfView.x;
	mSprite->mPosition.y -= g_PointOfView.y;
	//mSprite->SetFrame(mFrame);
	mSprite->draw(hBackBuffer);
}

bool Platform::CheckCollision(RECT CPlayer)
{
	RECT rect = g_Player->CRect();
	if(rect.top == CPlayer.top && rect.bottom == CPlayer.bottom && rect.left == CPlayer.left && rect.right == CPlayer.right && g_Player->mVelocity.y > 0){
		POINT p1,p2;
		p1.y = CPlayer.bottom;
		p1.x = CPlayer.left;
		p2.y = CPlayer.bottom;
		p2.x = CPlayer.right;
	if(PointInRectangle(Rect(),p1) || PointInRectangle(Rect(),p2))
			return true;
	else
		return false;}
	else
	return false;
}

bool Platform::TakeDamage(int damage,int PosX,int PosY){
return true;
}

bool Platform::TakeDamage(int damage){
return true;
}
//Elevator

Elevator::Elevator(){
mVelocity.x = mVelocity.y = 0;
mSprite = NULL;
mFrame = 1;
isElevator = true;
mIsSolid = false;
FOT=FOT_ELEVATOR;
}

Elevator::~Elevator(){}

bool Elevator::Update(double Delta){
mPosition.y += mVelocity.y * Delta;
if(mPosition.y <= mUpLimit){
	mPosition.y = mUpLimit;
	mVelocity.y *= -1;
	}
if(mPosition.y >= mDownLimit){
	mPosition.y = mDownLimit;
	mVelocity.y *= -1;
	}
RECT rect = g_Player->CRect();
POINT p1,p2;
p1.y = rect.bottom;
p1.x = rect.left;
p2.y = rect.bottom;
p2.x = rect.right;
if(PointInRectangle(Rect(),p1) || PointInRectangle(Rect(),p2))
	g_Player->mPosition.y = mPosition.y - 202;
return false;}
//Spikes==================================
Spikes::Spikes(){
	mSprite  = NULL;
	mFrame   = 1;
	mElapsed = 0;
	mDelay   = 0;
	isElevator = false;
	mIsSolid = false;
	FOT=FOT_SPIKES;
}

Spikes::~Spikes(){}
void Spikes::Draw(HDC hBackBuffer){
	mSprite->mPosition = mPosition;
	mSprite->mPosition.x -= g_PointOfView.x;
	mSprite->mPosition.y -= g_PointOfView.y;
	mSprite->SetFrame(mFrame);
	mSprite->draw(hBackBuffer);
}
bool Spikes::Update(double Delta){
	if(RectCollideRect(g_Player->CRect(),Rect()))
		g_Player->takeDamage(10000);
	mElapsed += Delta;
	if(mElapsed >= mDelay){
		mElapsed = 0;
		mFrame++;
		if(mFrame >= mNumberOfFrames)
			mFrame = 1;
	}
return false;}
RECT Spikes::Rect(){
RECT sol;
sol.top    = (int)mPosition.y;
sol.left   = (int)mPosition.x;
sol.right  = sol.left + mSprite->width();
sol.bottom = sol.top  + mSprite->height();
return sol;
}
bool Spikes::CheckCollision(RECT CPlayer){
return false;
}
//CheckPoint===========================
CheckP::CheckP(){
mFrame = 1;
mIsActive = true;
mSprite = NULL;
FOT=FOT_CHECKP;
}

CheckP::~CheckP(){}

void CheckP::Draw(HDC hBackBuffer){
}

RECT CheckP::Rect(){
return RC;
}

bool CheckP::CheckCollision(RECT CPlayer){
	if(mIsActive && RectCollideRect(g_Player->CRect(),Rect()))
		{mFrame = 2;
		mIsActive=false;
		 CheckPoint::Save();
		}
	return false;
}


//RepairKit=====================================
RepairKit::RepairKit()
{
isElevator = false;
mIsSolid = false;
FOT=FOT_REPAIRKIT;
}

RepairKit::~RepairKit()
{
}

bool RepairKit::Update(double Delta)
{
	bool ret=false;
	if(RectCollideRect(g_Player->CRect(),Rect()) && g_Player->mMaxHP!=g_Player->mHP)
	{
		g_Player->mHP+=value;
		if(g_Player->mHP>g_Player->mMaxHP)
		{
			g_Player->mHP=g_Player->mMaxHP;
		}
		ret=true;
	}
	/*if(RectCollideRect(g_Player2->CRect(),Rect()) && g_Player2->mMaxHP!=g_Player2->mHP)
	{
		g_Player2->mHP+=value;
		if(g_Player2->mHP>g_Player2->mMaxHP)
		{
			g_Player2->mHP=g_Player2->mMaxHP;
		}
		ret=true;
	}*/
	return ret;
}

bool RepairKit::CheckCollision(RECT CPlayer)
{
	return false;
}

void RepairKit::Draw(HDC hBackBuffer)
{
	pSpr->mPosition=mPosition-g_PointOfView;
	pSpr->draw(hBackBuffer);
}

RECT RepairKit::Rect()
{
	RECT R;
	R.left=(int)mPosition.x;
	R.top=(int)mPosition.y;
	R.right=R.left+pSpr->width();
	R.bottom=R.top+pSpr->height();
	return R;
}
//usefull functions
bool Collide(RECT CRect){
bool ok = false;
for(register int i = 0 ; i < (int) g_Stuff.size();++i)
	if(g_Stuff[i]->CheckCollision(CRect))
		ok = true;

return ok;
}

void AddFloorTile(int PozX,int PozY,AnimatedSprite *Sprite){
	FallingFloor *Floor;
	
	Floor = new FallingFloor;
	Floor->mNumberOfFrames = Sprite->GetFrameCount();
	Floor->mSprite = Sprite;
	Floor->mSpeed = FloorSpeed;
	Floor->mPosition.x = PozX;
	Floor->mPosition.y = PozY;
	g_Stuff.push_back(Floor);

}

MovingDoor* AddDoor(int PozX,int PozY,AnimatedSprite *Sprite){
MovingDoor *Door;

Door = new MovingDoor;
Door->mNumberOfFrames = Sprite->GetFrameCount();
Door->mSprite = Sprite;
Door->mSpeed = DoorSpeed;
Door->mPosition.x = PozX;
Door->mPosition.y = PozY;
Door->mNumberOfSwitches = 0;
g_Stuff.push_back(Door);

return Door;
}

void AddSwitch(int  PozX,int PozY,MovingDoor* door,AnimatedSprite *Sprite){
RECT Rect;
Rect.top = Rect.left = 0;
Rect.bottom = Rect.right = 30;

Switch *GSwitch;
GSwitch = new Switch;
GSwitch->mSprite = Sprite;
GSwitch->mDoor = door;
GSwitch->mPosition.x = PozX;
GSwitch->mPosition.y = PozY;
door->mNumberOfSwitches++;
g_Stuff.push_back(GSwitch);

}

void AddBox(int PozX,int PozY,bool IsDestructible,bool Movable,int HP,AnimatedSprite *Sprite)
{

MovableObjects* Box;
Box = new MovableObjects;
Box->mSprite = Sprite;
Box->mPosition.x = PozX;
Box->mPosition.y = PozY;
Box->mSpeed  = BoxSpeed;
Box->mNumberOfFrames = Sprite->GetFrameCount();
Box->mMovable = Movable;
Box->mIsDestructible = IsDestructible;
Box->mVelocity.x = 0.f;
Box->mVelocity.y = 0.f;
Box->mHP         = HP;
Box->mElapsed    = 0.f;
Box->mIsAnimated = false;
Box->mIsSolid    = true;
g_Box.push_back(Box);
}
void AddTurret(int PozX,int PozY,int MinDamage,int MaxDamage,double Delay,
			   AnimatedSprite *Spri,double BulletSpeed,Sprite *BulletSprite,bool IsTracking
			   ,int P1X,int P1Y,int P2X,int P2Y,int P3X,int P3Y,int P4X,int P4Y,double FireDelay){
Turret *mTurret;
mTurret = new Turret();
mTurret->mPosition.x  = PozX;
mTurret->mPosition.y  = PozY;
mTurret->mMinDamage   = MinDamage;
mTurret->mMaxDamage   = MaxDamage;
mTurret->mSprite      = Spri;
mTurret->mBulletSpeed = BulletSpeed; 
mTurret->mBulletSprite= BulletSprite;
mTurret->mIsTracking  = IsTracking;
mTurret->P1.x         =P1X;
mTurret->P1.y         =P1Y;
mTurret->P2.x         =P2X;
mTurret->P2.y         =P2Y;
mTurret->P3.x         =P3X;
mTurret->P3.y         =P3Y;
mTurret->P4.x         =P4X;
mTurret->P4.y         =P4Y;
mTurret->mFireDelay   =FireDelay;
mTurret->mFrame       =1;
mTurret->mFireElapsed =0;
g_Stuff.push_back(mTurret);
}

void AddDummy(int PozX,int PozY,int HP,AnimatedSprite *Sprite,MovingDoor *Door){
Dummy *mDummy    = NULL;
mDummy            = new Dummy;
mDummy->mSprite  = Sprite;
mDummy->mDoor    = Door;
mDummy->mHP      = HP;
mDummy->mNumberOfFrames = Sprite->GetFrameCount();
mDummy->mSpeed   = DummySpeed;
mDummy->mPosition.x = PozX;
mDummy->mPosition.y = PozY;
Door->mNumberOfSwitches++;
g_Stuff.push_back(mDummy);
}

void AddPlatform(int PozX,int PozY,AnimatedSprite *Sprite){

Platform *mPlatform = new Platform;
mPlatform->mPosition.x = PozX;
mPlatform->mPosition.y = PozY;
mPlatform->mSprite = Sprite;
g_Stuff.push_back(mPlatform);
}

void AddSpikes(int PozX,int PozY,AnimatedSprite *Sprite){
Spikes *mSpike = new Spikes;
mSpike->mPosition.x = PozX;
mSpike->mPosition.y = PozY;
mSpike->mDelay      = SpikeSpeed;
mSpike->mNumberOfFrames = Sprite->GetFrameCount();
mSpike->mSprite    = Sprite;
mSpike->mElapsed   = 0;
g_Stuff.push_back(mSpike);
}

void AddElevator(int PozX,int PozY,int UpLim,int DownLim,double Speed,AnimatedSprite *Sprite){
Elevator *mElevator = new Elevator;
mElevator->mPosition.x = PozX;
mElevator->mPosition.y = PozY;
mElevator->mSprite     = Sprite;
mElevator->mUpLimit    = UpLim;
mElevator->mDownLimit  = DownLim;
mElevator->mVelocity.y = Speed;
g_Stuff.push_back(mElevator);

}

void AddCheckPoint(RECT Rect){
CheckP* mCheckP = new CheckP;
mCheckP->isElevator = false;
mCheckP->RC=Rect;
g_Stuff.push_back(mCheckP);
}
void AddFinishZone(int PozX,int PozY,int Width,int Height)
{
	FinishZone *pFinishZone = new FinishZone;
	pFinishZone->RC.left=PozX;
	pFinishZone->RC.right=PozX+Width;
	pFinishZone->RC.top=PozY;
	pFinishZone->RC.bottom=PozY+Height;
	g_Stuff.push_back(pFinishZone);
}
void AddRepairKit(int PozX,int PozY, Sprite* spr, int hp)
{
	RepairKit *pRepairKit = new RepairKit;
	pRepairKit->pSpr=spr;
	pRepairKit->mPosition.x=PozX;
	pRepairKit->mPosition.y=PozY;
	pRepairKit->value=hp;
	g_Stuff.push_back(pRepairKit);
}


bool CollideBox(RECT Crect)
{bool ok = false;
for(register int i = 0; i < (int)g_Box.size();++i)
if(g_Box[i]->mIsSolid){
	if(RectCollideRect(Crect,g_Box[i]->Rect()))
		return true;}
return false;
}

bool MoveBox(RECT Crect){
bool ok = false;
for(register int i = 0 ; i < (int)g_Box.size();++i)
if(g_Box[i]->mIsSolid){
	if(!g_Box[i]->Move(Crect))
		ok = true;}
return ok;
}

void UpdateBoxes(double delta){
for(register int i = 0 ; i < (int) g_Box.size();++i)
	if(g_Box[i]->UpdatePoz(delta))
	{
		delete g_Box[i];
		g_Box.erase(g_Box.begin() + i);
		--i;
	}
}

void UpdateStuff(double delta)
{
	for(register int i = 0; i < (int) g_Stuff.size();++i)
		if(g_Stuff[i]->Update(delta))
		{
			delete g_Stuff[i];
			g_Stuff.erase(g_Stuff.begin() + i);
			--i;
		}
}

void ApplyGravityOnBoxes(double delta, double gravity)
{
	for(register int i=0;i<(int)g_Box.size();++i)
	{
		g_Box[i]->mVelocity.y+=gravity*delta;
	}
}

void DeleteBoxes()
{
	for(register int i = 0; i < (int) g_Box.size();++i)
	{
		delete g_Box[i];
		g_Box.erase(g_Box.begin() + i);
		--i;
	}
}

void DeleteStuff()
{
	for(register int i = 0; i < (int) g_Stuff.size();++i)
	{
		delete g_Stuff[i];
		g_Stuff.erase(g_Stuff.begin() + i);
		--i;
	}
}
