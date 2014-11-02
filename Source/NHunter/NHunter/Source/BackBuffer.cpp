#include "..\Includes\BackBuffer.h"


BackBuffer::BackBuffer(HWND hWnd, int width, int height)
{
	mhWnd = hWnd;

	HDC hWndDC = GetDC(hWnd);

	mWidth = width;
	mHeight = height;

	mhDC = CreateCompatibleDC(hWndDC);

	mhSurface = CreateCompatibleBitmap(hWndDC, width, height);

	ReleaseDC(hWnd, hWndDC);

	reset();
}

void BackBuffer::reset()
{
	mhOldObject = (HBITMAP)SelectObject(mhDC, mhSurface);

	HPEN white_pen = (HPEN)GetStockObject(WHITE_PEN);
	HPEN oldPen = (HPEN)SelectObject(mhDC, white_pen);
	HBRUSH white = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mhDC, white);
	Rectangle(mhDC, 0, 0, mWidth, mHeight);
	SelectObject(mhDC, oldBrush);
	SelectObject(mhDC, oldPen);
}

BackBuffer::~BackBuffer()
{
	SelectObject(mhDC, mhOldObject);
	DeleteObject(mhSurface);
	DeleteDC(mhDC);
}

HDC BackBuffer::getDC()
{
	return mhDC;
}

int BackBuffer::width()
{
	return mWidth;
}

int BackBuffer::height()
{
	return mHeight;
}

void BackBuffer::present()
{
	HDC hWndDC = GetDC(mhWnd);

	BitBlt(hWndDC, 0, 0, mWidth, mHeight, mhDC, 0, 0, SRCCOPY);

	ReleaseDC(mhWnd, hWndDC);
}