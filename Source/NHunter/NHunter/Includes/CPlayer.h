#ifndef CPLAYER_H
#define CPLAYER_H

#include "Vec2.h"
#include "Main.h"

class AnimatedSprite;
class Sprite;
class CBox;

const int CRECTS_LENGTH = 128;
const int SHOOTING_POINTS = 16;
const int PRJSPEED_LENGTH = 16;
const int WEAPONS = 8;

const int WEP_PISTOL					= 0;
const int WEP_RIFLE						= 1;
const int WEP_SHOTGUN					= 2;
const int WEP_BAZOOKA					= 3;

const int WEPSTATE_DISABLED				= 0;
const int WEPSTATE_ENABLED				= 1;
const int WEPSTATE_CHOOSED				= 2;

class CPlayer
{
public:
	CPlayer();

	void update(float delta);
	void draw(HDC hBackBuffer);
	void nextAnimatedFrame();
	void updateBodyAndWeaponFrame();
	void jump();
	void leftGroundMove();
	void leftAirMove();
	void rightGroundMove();
	void rightAirMove();
	void stopMoving();
	void attackW();
	void attackWD();
	void attackD();
	void attackDS();
	void attackS();
	void attackSA();
	void attackA();
	void attackAW();
	void attack(bool isMoving);
	void attack(float delta);
	void loadCRects(std::string filename);
	void takeDamage(int dmg);
	void loadWeaponData(std::string filename);
	void update_part2();
	void PW_Speed();
	void PW_Armor();
	void PW_Strength();
	void turnOffPW();
	void getRifle();
	void getRifle_pl2();
	void getPistol();
	void getShotgun();
	void getBazooka();
	void reupdateWep();

	bool isRight();
	bool isLeft();

	int CurrPW;

	RECT CRect();

	Vec2 mPosition;
	Vec2 mVelocity;

	bool mYCollide;
	bool bGotToAttack;
	bool bDead;
	bool bCanSpeed;
	bool bGotToDoAnimation;
	bool bExplosive;

	int PWSPhase;
	int mWeaponId;
	int mWeaponDir;
	int mFootState;
	int mAttackDir;
	int mArmor;
	int mHP;
	int mMaxHP;

	float mEP;
	float mMaxEP;

	float TPF;
	float mTime;
	float mDelay;
	float mGroundSpeed;
	float mAirSpeed;
	float mJumpingSpeed;

	RECT CRects[CRECTS_LENGTH];

	AnimatedSprite *pMovement;
	Sprite *pPrj;
	Vec2 mShootingPoint[SHOOTING_POINTS];
	Vec2 mPrjSpeed[PRJSPEED_LENGTH];
	int mMinDmg;
	int mMaxDmg;
	int mWeaponState[WEAPONS];
	float mCoolingSpeed;
	float mFireingDelay;
	float mHeat[WEAPONS];
	float mHeatLevel;
};

#endif