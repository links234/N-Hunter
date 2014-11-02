#ifndef CPROJECTILE_H
#define CPROJECTILE_H

#include "Objects.h"
#include "Main.h"
#include <time.h>
#include <stdlib.h>

class Sprite;

#define PRJ_NORMAL 	1
#define PRJ_NOSTUFF 2

class CProjectile
{
public:
	CProjectile(Vec2 pos, Vec2 velocity, Sprite* sprite, int minDmg, int maxDmg, int type = PRJ_NORMAL, bool explosive=false, AnimatedSprite *expAnim=NULL, float tpf=0.01f);
	
	bool Update(double delta);
	void Draw(HDC hBackBuffer);

	static void Add(CProjectile *pPrj);
	static void UpdateAll(double delta);
	static void DrawAll(HDC hBackBuffer);
	static void DeleteAll();

	Vec2 	mVelocity;
	Vec2	mPos;
	Sprite*	m_pSprite;
	int mMinDmg, mMaxDmg;
	int mType;
	bool mExplosive;
	AnimatedSprite *pExpAnim;
	float TPF;

	static std::vector<CProjectile*> sPrjs;
};

#endif