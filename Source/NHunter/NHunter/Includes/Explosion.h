#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Main.h"

class CProjectile;

class CExplosion
{
public:
	CExplosion(AnimatedSprite *exp, CProjectile *prj, float TPF=0.05f);
	CExplosion(AnimatedSprite *exp, Vec2 pos, float TPF=0.05f);
	
	bool Update(float delta);
	void Draw(HDC hdc);

	static void Make(CExplosion* pExp, int damage);
	static void UpdateAll(float delta);
	static void DrawAll(HDC hdc);
	static void DeleteAll();

	static std::vector<CExplosion*> sExps;
private:
	AnimatedSprite* pAnim;
	Vec2 mPos;
	int mFrame;
	float mTimeElapsed;
	float mTimePerFrame;
};

#endif
