#include "..\\Includes\\CProjectile.h"

extern CPlayer 							*g_Player, *g_Player2;
extern Vec2 							g_PointOfView;;
extern std::vector<RECT> 				g_CRect;
extern std::vector<Stuff*>         		g_Stuff;
extern std::vector<MovableObjects*> 	g_Box;
extern bool								g_bCheats[MAXCHEATS];
extern std::vector<CImageFile*>			g_pBackground;
extern int								gamePhase;

std::vector<CProjectile*> CProjectile::sPrjs;

int Random(int min, int max)
{
	return min+rand()%(max-min+1);
}

CProjectile::CProjectile(Vec2 pos, Vec2 velocity, Sprite* sprite, int minDmg, int maxDmg, int type, bool explosive, AnimatedSprite *expAnim, float tpf)
	: TPF(tpf), mVelocity(velocity), m_pSprite(sprite), mMinDmg(minDmg), mMaxDmg(maxDmg), mPos(pos), mType(type), mExplosive(explosive), pExpAnim(expAnim)
{
}

bool CProjectile::Update(double delta)
{
	Vec2 displacement=mVelocity*delta;
	POINT AA;
	AA.x=(int)mPos.x;
	AA.y=(int)mPos.y;
	POINT AB;
	AB.x=AA.x+(int)m_pSprite->width();
	AB.y=AA.y;
	POINT AC;
	AC.x=AA.x+(int)m_pSprite->width();
	AC.y=AA.y+(int)m_pSprite->height();
	POINT AD;
	AD.x=AA.x;
	AD.y=AA.y+(int)m_pSprite->height();

	POINT BA=AA;
	BA.x+=(int)displacement.x;
	BA.y+=(int)displacement.y;
	POINT BB=AB;
	BB.x+=(int)displacement.x;
	BB.y+=(int)displacement.y;
	POINT BC=AC;
	BC.x+=(int)displacement.x;
	BC.y+=(int)displacement.y;
	POINT BD=AD;
	BD.x+=(int)displacement.x;
	BD.y+=(int)displacement.y;
	
	RECT NEW;
	NEW.left=BA.x;
	NEW.top=BA.y;
	NEW.right=BC.x;
	NEW.bottom=BC.y;

	if((mType==PRJ_NOSTUFF && !g_bCheats[CHEAT_WATERGIRL]) || gamePhase==GP_FUNMODE)
	{
		if(RectCollideRect(NEW,g_Player->CRect()))
		{
			if(!mExplosive)
			{
				g_Player->takeDamage(Random(mMinDmg,mMaxDmg));
			}
			else
			{
				CExplosion::Make(new CExplosion(pExpAnim,this,TPF),Random(mMinDmg,mMaxDmg));
			}
			return true;
		}
	}

	if(gamePhase==GP_FUNMODE)
	{
		if(RectCollideRect(NEW,g_Player2->CRect()))
		{
			if(!mExplosive)
			{
				g_Player2->takeDamage(Random(mMinDmg,mMaxDmg));
			}
			else
			{
				CExplosion::Make(new CExplosion(pExpAnim,this,TPF),Random(mMinDmg,mMaxDmg));
			}
			return true;
		}
	}

	for(register int i=0;i<(int)g_CRect.size();++i)
	{
		if(!(mType!=PRJ_NOSTUFF && g_bCheats[CHEAT_UBERBULLET]))
		{
			if(RectCollideRect(NEW,g_CRect[i]))
			{
				if(mExplosive)
				{
					CExplosion::Make(new CExplosion(pExpAnim,this,TPF),Random(mMinDmg,mMaxDmg));
				}
				return true;
			}
		}
	}
	
	for(register int i=0;i<(int)g_Stuff.size();++i)
	{
		if(mType!=PRJ_NOSTUFF || g_Stuff[i]->FOT==FOT_MOVINGDOOR)
		{
			if(RectCollideRect(NEW,g_Stuff[i]->Rect()))
			{
				if(g_Stuff[i]->TakeDamage(0,(BA.x + BC.x)/2,(BA.y + BC.y)/2))
				{
					if(mExplosive)
					{
						CExplosion::Make(new CExplosion(pExpAnim,this,TPF),Random(mMinDmg,mMaxDmg));
					}
					else
					{
						g_Stuff[i]->TakeDamage(Random(mMinDmg,mMaxDmg),(BA.x + BC.x)/2,(BA.y + BC.y)/2);
					}
					return true;
				}
			}
		}
	}

	for(register int i=0;i<(int)g_Box.size();++i)
	{
		if(RectCollideRect(NEW,g_Box[i]->Rect()))
		{
			if(!mExplosive)
			{
				g_Box[i]->TakeDamage(Random(mMinDmg,mMaxDmg));
			}
			else
			{
				CExplosion::Make(new CExplosion(pExpAnim,this,TPF),Random(mMinDmg,mMaxDmg));
			}
			return true;
		}
	}

	RECT SCENE;
	SCENE.top=0;
	SCENE.bottom=768;
	SCENE.left=0;
	SCENE.right=g_pBackground.size()*1024;
	if(!RectCollideRect(NEW,SCENE))
	{
		return true;
	}

	mPos+=displacement;
	return false;
}

void CProjectile::Draw(HDC hBackBuffer)
{
	RECT r;
	r.left=(int)(mPos.x-g_PointOfView.x);
	r.right=r.left+m_pSprite->width();
	r.top=(int)(mPos.y-g_PointOfView.y);
	r.bottom=r.top+m_pSprite->height();
	RECT scr;
	scr.left=0;
	scr.right=1024;
	scr.top=0;
	scr.bottom=768;
	if(RectCollideRect(r,scr))
	{
		m_pSprite->mPosition.x=r.left;
		m_pSprite->mPosition.y=r.top;
		m_pSprite->draw(hBackBuffer);
	}
}

void CProjectile::Add(CProjectile *pPrj)
{
	sPrjs.push_back(pPrj);
}

void CProjectile::UpdateAll(double delta)
{
	for(register int i=0;i<(int)sPrjs.size();++i)
	{
		if(sPrjs[i]->Update(delta))
		{
			delete sPrjs[i];
			sPrjs.erase(sPrjs.begin()+i);
			--i;
		}
	}
}

void CProjectile::DrawAll(HDC hBackBuffer)
{
	for(register int i=0;i<(int)sPrjs.size();++i)
	{
		sPrjs[i]->Draw(hBackBuffer);
	}
}

void CProjectile::DeleteAll()
{
	for(register int i=0;i<(int)sPrjs.size();++i)
	{
		delete sPrjs[i];
	}
	sPrjs.resize(0);
}
