#include "..\\Includes\\RectCollide.h"

bool SegIntSeg(POINT A1, POINT A2, POINT B1, POINT B2)
{
	int a1=A1.y-A2.y;
	int b1=A2.x-A1.x;
	int c1=A2.x*A1.y-A1.x*A2.y;
	int a2=B1.y-B2.y;
	int b2=B2.x-B1.x;
	int c2=B2.x*B1.y-B1.x*B2.y;
	POINT I;
	I.x=-(b1*c2-b2*c1)/(a1*b2-a2*b1);
	I.y=-(c1*a2-c2*a1)/(a1*b2-a2*b1);
	RECT r1,r2;
	r1.left=A1.x;
	r1.right=A2.x;
	r1.top=A1.y;
	r1.bottom=B2.y;
	r2.left=B1.x;
	r2.right=B2.x;
	r2.top=B1.y;
	r2.bottom=B2.y;
	return PointInRectangle(r1,I) && PointInRectangle(r2,I);
}

bool PointInRectangle(RECT r, POINT p)
{
	if(r.top<=p.y && r.bottom>=p.y)
	{
		if(r.left<=p.x && r.right>=p.x)
		{
			return true;
		}
		return false;
	}
	return false;
}

bool RectCollideRect(RECT r1, RECT r2)
{
	if(r1.bottom>=r2.bottom && r1.top<=r2.top && r1.left>=r2.left && r1.right<=r2.right)
		return true;

	if(r2.bottom>=r1.bottom && r2.top<=r1.top && r2.left>=r1.left && r2.right<=r1.right)
		return true;
	
	POINT r1_a={r1.left,r1.top};
	POINT r1_b={r1.right,r1.top};
	POINT r1_c={r1.right,r1.bottom};
	POINT r1_d={r1.left,r1.bottom};

	POINT r2_a={r2.left,r2.top};
	POINT r2_b={r2.right,r2.top};
	POINT r2_c={r2.right,r2.bottom};
	POINT r2_d={r2.left,r2.bottom};

	return PointInRectangle(r1,r2_a)||PointInRectangle(r1,r2_b)||PointInRectangle(r1,r2_c)||PointInRectangle(r1,r2_d)||
		   PointInRectangle(r2,r1_a)||PointInRectangle(r2,r1_b)||PointInRectangle(r2,r1_c)||PointInRectangle(r2,r1_d);
}
