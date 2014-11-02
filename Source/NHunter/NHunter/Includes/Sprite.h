#ifndef SPRITE_H
#define SPRITE_H
#include "Main.h"
#include "Vec2.h"

class Sprite
{
public:
	Sprite(int imageID, int maskID);
	Sprite(const char *szImageFile, const char *szMaskFile);

	virtual ~Sprite();

	virtual int width();
	virtual int height();

	virtual void draw(HDC hBackBufferDC);

	static void setDefaultSpriteDC(HDC hSpriteDC);
	static void deleteAllMemory();
	static void checkForErrors();

public:
	Vec2 mPosition;

private:
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite& rhs);

protected:
	HBITMAP mhImage;
	HBITMAP mhMask; 
	BITMAP 	mImageBM;
	BITMAP 	mMaskBM;
	std::string mImgFile;
	std::string mMaskFile;

 	static HDC sDefault_hSpriteDC;
	static std::vector<Sprite*> v_ptrs;
};

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(const char *szImageFile, const char *szMaskFile, const RECT& rcFirstFrame, int iFrameCount);
	virtual ~AnimatedSprite() { }

public:
	void SetFrame(int iIndex);

	int GetFrameCount() { return miFrameCount; }
	int GetFrameId() { return miCurrFrame; }
	virtual int width(){ return miFrameWidth; }
	virtual int height(){ return miFrameHeight; }

	virtual void draw(HDC hBackBufferDC);
	
	COLORREF GetMaskPixel(int x, int y);

protected:
	POINT mptFrameStartCrop;
	POINT mptFrameCrop;
	int miFrameWidth;
	int miFrameHeight;
	int miFrameCount;
	int miCurrFrame;
	int miMaxColumns;
	int miMaxRows;
};

#endif 
