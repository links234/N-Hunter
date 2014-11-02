#include "..\\Includes\\Shake.h"

extern std::vector<CImageFile*>		g_pBackground;
extern Vec2 						g_PointOfView;

bool g_bIsShakeing = false;
int maxShakeingFrame=30;
int maxOffset=50;
float shakeingDelay=0.1f;

void ClearShake()
{
	maxShakeingFrame=0;
	maxOffset=0;
}

void UpdateShake(float delta)
{
	if(g_PointOfView.x<0)g_PointOfView.x=0;
	if(g_PointOfView.x>1024*(g_pBackground.size()-1))g_PointOfView.x=1024*(g_pBackground.size()-1);

	static float time=0.0f;
	static int shakeingFrame=0;
	static int sign=1;
	if(g_bIsShakeing)
	{
		time+=delta;
		bool wasIn=false;
		while(time>shakeingDelay)
		{
			time-=shakeingDelay;
			wasIn=true;
			++shakeingFrame;
		}
		if(wasIn)
		{
			sign*=-1;
			int offset=rand()%maxOffset+1;
			offset*=sign;
			g_PointOfView.x+=offset;
		}
		if(shakeingFrame>maxShakeingFrame)
		{
			g_bIsShakeing=false;
			time=0.0f;
			shakeingFrame=0;
		}
	}
}

void MakeShake(int maxFrame, int offset, float delay)
{
	maxShakeingFrame=maxFrame;
    maxOffset=offset;
	shakeingDelay=delay;
	g_bIsShakeing=true;
}
