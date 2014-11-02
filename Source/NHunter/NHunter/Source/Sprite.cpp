#include "..\Includes\Sprite.h"

std::vector<Sprite*> Sprite::v_ptrs;
HDC Sprite::sDefault_hSpriteDC;

extern HINSTANCE g_hInst;

Sprite::Sprite(int imageID, int maskID)
{
	mhImage = LoadBitmap(g_hInst, MAKEINTRESOURCE(imageID));
	mhMask = LoadBitmap(g_hInst, MAKEINTRESOURCE(maskID));

	GetObject(mhImage, sizeof(BITMAP), &mImageBM);
	GetObject(mhMask, sizeof(BITMAP), &mMaskBM);
	v_ptrs.push_back(this);
}

Sprite::Sprite(const char *szImageFile, const char *szMaskFile)
{
	mImgFile=std::string(szImageFile);
	mMaskFile=std::string(szMaskFile);

	mhImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	mhMask = (HBITMAP)LoadImage(g_hInst, szMaskFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	GetObject(mhImage, sizeof(BITMAP), &mImageBM);
	GetObject(mhMask, sizeof(BITMAP), &mMaskBM);
	v_ptrs.push_back(this);
}

Sprite::~Sprite()
{
	DeleteObject(mhImage);
	DeleteObject(mhMask);
}

int Sprite::width()
{
	return mImageBM.bmWidth;
}

int Sprite::height()
{
	return mImageBM.bmHeight;
}

void Sprite::draw(HDC hBackBufferDC)
{
	int w = width();
	int h = height();

	int x = (int)mPosition.x;
	int y = (int)mPosition.y;

	HBITMAP oldObj = (HBITMAP)SelectObject(sDefault_hSpriteDC,mhMask);
	BitBlt(hBackBufferDC,x,y,w,h,sDefault_hSpriteDC,0,0,SRCAND);

	SelectObject(sDefault_hSpriteDC,mhImage);
	BitBlt(hBackBufferDC,x,y,w,h,sDefault_hSpriteDC,0,0,SRCPAINT);

	SelectObject(sDefault_hSpriteDC,oldObj);
}

void Sprite::setDefaultSpriteDC(HDC hSpriteDC)
{
	sDefault_hSpriteDC=hSpriteDC;
}

AnimatedSprite::AnimatedSprite(const char *szImageFile, const char *szMaskFile, const RECT& rcFirstFrame, int iFrameCount) 
			: Sprite (szImageFile, szMaskFile)
{
	mptFrameCrop.x = rcFirstFrame.left;
	mptFrameCrop.y = rcFirstFrame.top;
	mptFrameStartCrop = mptFrameCrop;
	miFrameWidth = rcFirstFrame.right - rcFirstFrame.left;
	miFrameHeight = rcFirstFrame.bottom - rcFirstFrame.top;
	miFrameCount = iFrameCount;
	miMaxColumns = Sprite::width()/miFrameWidth;
	miMaxRows = Sprite::height()/miFrameHeight;
	miCurrFrame = 0;
}

void AnimatedSprite::SetFrame(int iIndex)
{
	if(0<=iIndex || iIndex>miFrameCount)
	{
		miCurrFrame = iIndex;

		int iRow	= iIndex/miMaxColumns+1;
		int iColumn	= iIndex%miMaxColumns;
		if(iColumn==0)
		{
			iColumn=miMaxColumns;
			--iRow;
		}

		mptFrameCrop.x = mptFrameStartCrop.x + miFrameWidth*(iColumn-1);
		mptFrameCrop.y = mptFrameStartCrop.y + miFrameHeight*(iRow-1);
	}
}

void AnimatedSprite::draw(HDC hBackBufferDC)
{
	int w = miFrameWidth;
	int h = miFrameHeight;

	int x = (int)mPosition.x;
	int y = (int)mPosition.y;

	SelectObject(sDefault_hSpriteDC, mhMask);
	BitBlt(hBackBufferDC, x, y, w, h, sDefault_hSpriteDC, mptFrameCrop.x, mptFrameCrop.y, SRCAND);

	SelectObject(sDefault_hSpriteDC, mhImage);
	BitBlt(hBackBufferDC, x, y, w, h, sDefault_hSpriteDC, mptFrameCrop.x, mptFrameCrop.y, SRCPAINT);
}

COLORREF AnimatedSprite::GetMaskPixel(int x, int y)
{
	SelectObject(sDefault_hSpriteDC, mhMask);
	return GetPixel(sDefault_hSpriteDC,mptFrameCrop.x+x,mptFrameCrop.y+y);
}

void Sprite::deleteAllMemory()
{
	for(register int i=0;i<(int)v_ptrs.size();++i)
	{
		delete v_ptrs[i];
		v_ptrs.erase(v_ptrs.begin()+i);
		--i;
	}
}


void Sprite::checkForErrors()
{
	for(register int i=0;i<(int)v_ptrs.size();++i)
	{
		FileCheck(std::string(v_ptrs[i]->mImgFile));
		FileCheck(std::string(v_ptrs[i]->mMaskFile));
	}
}
