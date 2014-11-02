#include "..\\Includes\\Explosion.h"

extern std::vector<Stuff*>         	g_Stuff;
extern std::vector<MovableObjects*> g_Box;
extern Vec2 						g_PointOfView;
extern CPlayer						*g_Player, *g_Player2;
extern int							gamePhase;

std::vector<CExplosion*> CExplosion::sExps;

CExplosion::CExplosion(AnimatedSprite *exp, CProjectile *prj, float TPF)
	: mFrame(1), mTimeElapsed(0), mTimePerFrame(TPF), pAnim(exp)
{
	mPos.x=prj->mPos.x-exp->width()/2+prj->m_pSprite->width()/2;
	mPos.y=prj->mPos.y-exp->height()/2+prj->m_pSprite->height()/2;
}

CExplosion::CExplosion(AnimatedSprite *exp, Vec2 pos, float TPF)
	: mFrame(1), mTimeElapsed(0), mTimePerFrame(TPF), pAnim(exp), mPos(pos)
{

}

bool CExplosion::Update(float delta)
{
	mTimeElapsed+=delta;
	while(mTimeElapsed>mTimePerFrame)
	{
		mTimeElapsed-=mTimePerFrame;
		++mFrame;
	}

	if(mFrame>pAnim->GetFrameCount())
	{
		return true;
	}
	return false;
}

void CExplosion::Draw(HDC hdc)
{
	pAnim->SetFrame(mFrame);
	pAnim->mPosition=mPos-g_PointOfView;
	pAnim->draw(hdc);
}

void CExplosion::Make(CExplosion* pExp, int damage)
{
	RECT EXPR;
	EXPR.top=(int)pExp->mPos.y;
	EXPR.left=(int)pExp->mPos.x;
	EXPR.bottom=(int)pExp->mPos.y+pExp->pAnim->height();
	EXPR.right=(int)pExp->mPos.x+pExp->pAnim->width();

	if(RectCollideRect(g_Player->CRect(),EXPR))
	{
		g_Player->takeDamage(damage);
	}

	if(gamePhase==GP_FUNMODE)
	{
		if(RectCollideRect(g_Player2->CRect(),EXPR))
		{
			g_Player2->takeDamage(damage);
		}
	}

	for(register int i=0;i<(int)g_Stuff.size();++i)
	{
		if(g_Stuff[i]->mIsSolid)
		{
			if(RectCollideRect(EXPR,g_Stuff[i]->Rect()))
			{
				g_Stuff[i]->TakeDamage(damage);
			}
		}
	}

	for(register int i=0;i<(int)g_Box.size();++i)
	{
		if(RectCollideRect(EXPR,g_Box[i]->Rect()))
		{
			g_Box[i]->TakeDamage(damage);
		}
	}

	sExps.push_back(pExp);
}

void CExplosion::UpdateAll(float delta)
{
	for(register int i=0;i<(int)sExps.size();++i)
	{
		if(sExps[i]->Update(delta))
		{
			delete sExps[i];
			sExps.erase(sExps.begin()+i);
			--i;
		}
	}
}

void CExplosion::DrawAll(HDC hdc)
{
	for(register int i=0;i<(int)sExps.size();++i)
	{
		sExps[i]->Draw(hdc);
	}
}

void CExplosion::DeleteAll()
{
	for(register int i=0;i<(int)sExps.size();++i)
	{
		delete sExps[i];
	}
	sExps.resize(0);
}