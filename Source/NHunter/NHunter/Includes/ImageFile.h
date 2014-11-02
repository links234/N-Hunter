#pragma once

#include "Main.h"

struct GaussFilter
{
	std::vector<int> Fact;
	int Sum;
};

GaussFilter CreateGaussFilter(int level);

typedef BYTE (*RGBQUAD_TO_BYTE)(const RGBQUAD &q);

enum EColorChannel
{
	ECC_RED,
	ECC_GREEN,
	ECC_BLUE,
	ECC_HUE,
	ECC_SATURATION,
	ECC_LUMINOSITY,
	ECC_EXCLUSIVERED,
	ECC_EXCLUSIVEGREEN,
	ECC_EXCLUSIVEBLUE
};


class CImageFile
{
protected:
	BITMAPINFOHEADER m_biInfo;
	RGBQUAD *m_pRGB;
	HBITMAP m_hBMP;

	LONG &height;
	LONG &width;
	char m_szFileName[MAX_PATH];

public:
	CImageFile(void);
	virtual ~CImageFile(void);

	bool LoadBitmapFromFile(const char* szFileName, HDC hdc);
	virtual void Paint(HDC hdc, int x, int y);

	LONG Height() const { return height; }
	LONG Width() const { return width; }

	void Clear() { ZeroMemory(m_pRGB, sizeof(RGBQUAD) * width * height); }
	void Reload(HDC hdc);

	void CaptureImage(HDC hImgDC, int x, int y, int width, int height);
	void ApplyGausianBlur(GaussFilter gauss);

	BYTE* CopyMonoImage(EColorChannel chn, const RECT* rc = NULL);
	void PasteMonoImage(const BYTE *img, EColorChannel chn, const RECT* rc = NULL);
};
