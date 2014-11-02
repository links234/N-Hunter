#ifndef OBJECTS_H
#define OBJECTS_H
#include<windows.h>
#include<string>
#include"Main.h"
using namespace std;

#define FOT_MOVINGDOOR		0
#define FOT_SWITCH			1
#define FOT_FINISHZONE		2
#define FOT_TURRET			3
#define FOT_FALLINGFLOOR	4
#define FOT_DUMMY			5
#define FOT_PLATFORM		6
#define FOT_ELEVATOR		7
#define FOT_SPIKES			8
#define FOT_CHECKP			9
#define FOT_ENEMY			10
#define FOT_BOSS			11
#define FOT_REPAIRKIT		12

class Object{

public:
	   Object();
	   ~Object();
	   void Draw(HDC hBackBuffer);

bool mIsSolid;
int mFrame;
Vec2 mPosition;
AnimatedSprite *mSprite;
int mNumberOfFrames;
};


class MovableObjects:public Object{
public:
	MovableObjects();
	~MovableObjects();
	bool Move(RECT Player);
	bool UpdatePoz(double Delta);
    void TakeDamage(int damage);
	RECT Rect();
	bool mMovable;
	double mSpeed;
	double mElapsed;
	bool   mIsAnimated;
	int mHP;
	int mIsDestructible;
	Vec2 mVelocity;
};

class Stuff{
public:
virtual bool Update(double Delta);
virtual bool CheckCollision(RECT CPlayer);
virtual	void Draw(HDC hBackBuffer);
virtual bool TakeDamage(int damage,int PosX,int PosY);
virtual bool TakeDamage(int damage);
virtual RECT Rect();
int mFrame;
int FOT;
AnimatedSprite *mSprite;
bool isElevator;
bool mIsSolid;
};




class FallingFloor:public Stuff{
public:
	
	FallingFloor();
	~FallingFloor();
	bool CheckCollision(RECT CPlayer);
	bool Update(double Delta);
	void Draw(HDC hBackBuffer);
	bool TakeDamage(int damage,int PosX,int PosY);
	bool TakeDamage(int damage);
	RECT Rect();
	Vec2 mPosition;
	double mSpeed;
	int mNumberOfFrames;
	bool mIsAnimated;
protected:
	double mTimeElapsed;
};

class MovingDoor:public Stuff{
public:
	MovingDoor();
	~MovingDoor();
	void Draw(HDC hBackBuffer);
	bool Update(double Delta);
	RECT Rect();
	bool TakeDamage(int damage,int PosX,int PosY);
	bool TakeDamage(int damage);
	double mSpeed;
	bool   CheckCollision(RECT CPlayer);
	int    mUpRDown;
	bool   mIsAnimated;
	int    mNumberOfSwitches;
	int    mActiveSwitches;
	double mTimeElapsed;
	int mNumberOfFrames;
	Vec2 mPosition;
};

class Switch:public Stuff{
public:
	MovingDoor     *mDoor;
	Switch();
	~Switch();
	void Draw(HDC hBackBuffer);
	bool Update(double Delta);
	RECT Rect();
	Vec2 mPosition;
private:
	bool mKeyDown;
	bool mState;


};

class FinishZone:public Stuff
{
public:
	FinishZone();
	~FinishZone();
	void Draw(HDC hBackBuffer);
	bool Update(double Delta);
	RECT Rect();
	RECT RC;
};

class Turret:public Stuff{
public:
	Turret();
	~Turret();
	bool CheckCollision(RECT CPlayer);
	bool Update(double Delta);
	void Draw(HDC hBackBuffer);
	bool Check(Vec2 a,Vec2 b,Vec2 c,Vec2 d);
	RECT Rect();
	RECT     mRect;
	Vec2     mPosition;
	Vec2     P1;
	Vec2     P2;
	Vec2     P3;
	Vec2     P4;
	double   mElapsed;
	double   mFireElapsed;
	double   mFireDelay;
	double   mBulletSpeed;
	bool     mReadyToFire;
	bool     mIsTracking;
	int      mMinDamage;
	int      mMaxDamage;
	Sprite   *mBulletSprite;
};

class Dummy:public Stuff{
public:
	Dummy();
	~Dummy();
	bool Update(double Delta);
	void Draw(HDC hBackBuffer);
	bool TakeDamage(int damage,int PosX,int PosY);
	bool TakeDamage(int damage);
	RECT Rect();
	MovingDoor *mDoor;
	Vec2 mPosition;
	Vec2 mVelocity;
	int mHP;
	double mSpeed;
	double mElapsed;
	bool   mIsAnimated;
	int    mNumberOfFrames;

};

class Platform:public Stuff{
public:
	Platform();
	~Platform();
	void Draw(HDC hBackBuffer);
	bool TakeDamage(int damage,int PosX,int PosY);
	bool TakeDamage(int damage);
	bool CheckCollision(RECT CPlayer);
	RECT Rect();
	Vec2 mPosition;
};

class Elevator:public Platform{
public:
	Elevator();
	~Elevator();
	bool Update(double Delta);
	Vec2 mVelocity;
	int mUpLimit;
	int mDownLimit;
};

class Spikes:public Stuff{
	public:
	Spikes();
	~Spikes();
	void Draw(HDC hBackBuffer);
	bool CheckCollision(RECT CPlayer);
	RECT Rect();
	Vec2 mPosition;
	bool Update(double Delta);
	double mElapsed ;
	double mDelay;
	int mNumberOfFrames;
};
class CheckP:public Stuff{
public:
	CheckP();
	~CheckP();
	void Draw(HDC hBackBuffer);
	bool CheckCollision(RECT CPlayer);
	RECT Rect();
	RECT RC;
	bool mIsActive;
};
class Enemy:public Stuff{
public:
	Enemy();
	~Enemy();
	virtual bool Update(double Delta);
	bool TakeDamage();
	void Draw(HDC hBackBuffer);
	bool TakeDamage(int damage,int PosX,int PosY);
	bool TakeDamage(int damage);
	virtual bool CheckCollision(RECT CPlayer);
	virtual bool Aim(RECT CPlayer);
	virtual void Fire();
	RECT Rect();

	AnimatedSprite *mDeathAnimationR;
	AnimatedSprite *mDeathAnimationL;
	Sprite *mBulletSprite;
	Vec2 mPosition;
	Vec2 mVelocity;
	MovingDoor *mDoor;
	double mHeat;
	double mHeatDelay;
	double mElapsed;
	double mDelay;
	double mRange;
	double mBulletSpeed;
	int mHP;
	bool mIsMoving;
	int mLeftLimit;
	int mBulletMinDmg;
	int mBulletMaxDmg;	
	int mRightLimit;
	int mArmor;
	bool mIsDying;
	bool mReadyToFire;

};
class Boss:public Enemy{
public:
	Boss();
	~Boss();
	bool Update(double Delta);
	bool Aim();
	bool CheckCollision(RECT CPlayer);
	void Fire();


	Vec2 mBulletDir;
	Vec2 mBulletPos;
	Turret* T1;
	Turret* T2;
	int *mFrameList;
	float *mBulletAngList;
	Vec2*mBulletPosList;
	int mNumberOfShots;
	int mSameShots;
	int mCurrShot;
	int mListIndex;
	int mLastPoz;
	bool mIsMoving;
	int mPoz1;
	int mPoz2;
	int mPoz3;
	bool mClockWise;
	bool mTurret;
	bool mPowerUp;
	double repairKitTime;
};
class RepairKit:public Stuff
{
public:
	RepairKit();
	~RepairKit();
	bool Update(double Delta);
	bool CheckCollision(RECT CPlayer);
	void Draw(HDC hBackBuffer);
	RECT Rect();
	Sprite *pSpr;
	Vec2 mPosition;
	int value;
};
long long Sarrus(Vec2 P1,Vec2 P2,Vec2 P3);
bool Collide(RECT CRect);
bool CollideBox(RECT CRect);
bool MoveBox(RECT Crect);
void AddFloorTile(int PozX,int PozY,AnimatedSprite *Sprite);
MovingDoor* AddDoor(int PozX,int PozY,AnimatedSprite *Sprite);
void AddSwitch(int PozX,int PozY,MovingDoor* Door,AnimatedSprite *Sprite);
void AddBox(int PozX,int PozY,bool IsDestructible,bool Movable,int HP,AnimatedSprite *Sprite);
bool CollideBox(RECT CRect);
bool MoveBox(RECT Crect);
void UpdateBoxes(double delta);
void UpdateStuff(double delta);
void ApplyGravityOnBoxes(double delta, double gravity);
void AddTurret(int PozX,int PozY,int MinDamage,int MaxDamage,double Delay,
			   AnimatedSprite *Spri,double BulletSpeed,Sprite *BulletSprite,bool IsTracking
			   ,int P1X,int P1Y,int P2X,int P2Y,int P3X,int P3Y,int P4X,int P4Y,double FireDelay);
void AddDummy(int PozX,int PozY,int HP,AnimatedSprite *Sprite,MovingDoor *Door);
void AddPlatform(int PozX,int PozY,AnimatedSprite *Sprite);
void AddSpikes(int PozX,int PozY,AnimatedSprite *Sprite);
void AddElevator(int PozX,int PozY,int UpLim,int DownLim,double Speed,AnimatedSprite *Sprite);
void AddFinishZone(int PozX,int PozY,int Width,int Height);
void AddCheckPoint(RECT Rect);
void AddEnemyT1(int PozX,int PoxY,int LeftLimit,int RightLimit,MovingDoor *Door = NULL,bool mIsMoving = true);
void AddBoss();
void AddRepairKit(int PozX,int PozY, Sprite* spr, int hp);

void UpdateBoxes(double delta);
void UpdateStuff(double delta);

void DeleteBoxes();
void DeleteStuff();

void ApplyGravityOnBoxes(double delta, double gravity);
#endif