#include "..\\Includes\\ImageFile.h"
#include <math.h>

extern HINSTANCE g_hInst;
#define EPS 1e-3

CImageFile::CImageFile() : height(m_biInfo.biHeight), width(m_biInfo.biWidth)
{
	m_hBMP = 0;
	m_pRGB = NULL;
	ZeroMemory(&m_biInfo, sizeof(BITMAPINFOHEADER));
}

bool CImageFile::LoadBitmapFromFile(const char *szFileName, HDC hdc)
{
	FileCheck(std::string(szFileName));

	BYTE *pData;
	HDC mdc = CreateCompatibleDC(hdc);

	strcpy_s(m_szFileName, MAX_PATH, szFileName);

	if(m_pRGB)
	{
		delete[] m_pRGB;
		m_pRGB = NULL;
	}

	if(m_hBMP)
	{
		DeleteObject(m_hBMP);
		m_hBMP = 0;
	}

	m_hBMP = (HBITMAP)LoadImage(g_hInst, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);	

	if(!m_hBMP)
		return false;

	ZeroMemory(&m_biInfo, sizeof(BITMAPINFO));
	m_biInfo.biSize=sizeof(BITMAPINFOHEADER);

	GetDIBits(mdc, m_hBMP, 0, 0, NULL, (BITMAPINFO*)&m_biInfo, DIB_RGB_COLORS);

	pData = new BYTE[m_biInfo.biSizeImage];

	GetDIBits(mdc, m_hBMP, 0, height, pData, (BITMAPINFO*)&m_biInfo, DIB_RGB_COLORS);

	m_pRGB = new RGBQUAD[width * height];

	if(m_biInfo.biBitCount == 24)
	{
		int size = width * height;
		unsigned char *data = pData;
		RGBQUAD *c = m_pRGB;

		for(int i=0;i<size;i++)
		{
			c->rgbBlue = *data++;
			c->rgbGreen = *data++;
			c->rgbRed = *data++;
			c->rgbReserved = 0;
			c++;
		}
	}

	m_biInfo.biBitCount = 32;

	DeleteObject(m_hBMP);
	m_hBMP = 0;

	DeleteDC(mdc);

	delete[] pData;

	return true;
}

void CImageFile::Reload(HDC hdc)
{
	LoadBitmapFromFile(m_szFileName, hdc);
}

void CImageFile::Paint(HDC hdc, int x, int y)
{
	if(!m_pRGB)
		return;

	if(!m_hBMP)
		m_hBMP = CreateCompatibleBitmap(hdc, width, height);

	HDC mdc = CreateCompatibleDC(hdc);

	SelectObject(mdc, m_hBMP);

	SetDIBits(mdc, m_hBMP, 0, height, m_pRGB, (BITMAPINFO*)&m_biInfo, DIB_RGB_COLORS);

	BitBlt(hdc, x, y, width, height, mdc, 0, 0, SRCCOPY);

	DeleteDC(mdc);
}

void CImageFile::CaptureImage(HDC hImgDC, int x, int y, int width, int height)
{
		delete m_pRGB;
		m_pRGB = new RGBQUAD[width * height];

		for(register int i=x;i<=x+width;++i)
		{
			for(register int j=y;j<=y+height;++j)
			{
				int pos=i*width+j;
				COLORREF col=GetPixel(hImgDC,i,j);
				m_pRGB[pos].rgbRed=GetRValue(col);
				m_pRGB[pos].rgbGreen=GetGValue(col);
				m_pRGB[pos].rgbBlue=GetBValue(col);
			}
		}
}

void CImageFile::ApplyGausianBlur(GaussFilter gauss)
{
	int gaussSum=gauss.Sum;

	int sumR=0;
	int sumG=0;
	int sumB=0;

	int w=Width();
	int h=Width();

	RGBQUAD *p_TempBuffer=new RGBQUAD[w*h];
	RGBQUAD color; 

	int pos;

	for(int i=1;i<w;i++)
	{
	  	for(int j=1;j<h;j++)
		{
		    sumR=0;
		    sumG=0;
		    sumB=0;
			gaussSum=gauss.Sum;
			for(int k=0;k<(int)gauss.Fact.size();k++)
			{
				int ii=(i-(((int)gauss.Fact.size()-1)/2)+k);
				if(ii<w && ii>=1)
				{
					color=m_pRGB[ii*w+j];
				    sumR+=color.rgbRed*gauss.Fact[k];
					sumG+=color.rgbGreen*gauss.Fact[k];
					sumB+=color.rgbBlue*gauss.Fact[k];
				}
				else
				{
					gaussSum-=gauss.Fact[k];
				}
		    }
			pos=i*w+j;
			p_TempBuffer[pos].rgbRed=sumR/gaussSum;
			p_TempBuffer[pos].rgbGreen=sumG/gaussSum;
			p_TempBuffer[pos].rgbBlue=sumB/gaussSum;
		} 
	}

	for(int i=1;i<w;i++)
	{
	  	for(int j=1;j<h;j++)
		{
		    sumR=0;
		    sumG=0;
		    sumB=0;
			gaussSum=gauss.Sum;
		    for(int k=0;k<(int)gauss.Fact.size();k++)
			{
				int jj=(j-((gauss.Fact.size()-1)/1)+k);
				if(jj<h && jj>=1)
				{
					color=p_TempBuffer[i*w+jj];
				    sumR+=color.rgbRed*gauss.Fact[k];
					sumG+=color.rgbGreen*gauss.Fact[k];
					sumB+=color.rgbBlue*gauss.Fact[k];
				}
				else
				{
					gaussSum-=gauss.Fact[k];
				}
		    }
			pos=i*w+j;
			m_pRGB[pos].rgbRed=sumR/gaussSum;
			m_pRGB[pos].rgbGreen=sumG/gaussSum;
			m_pRGB[pos].rgbBlue=sumB/gaussSum;
		} 
	}
}

CImageFile::~CImageFile(void)
{
	if(m_pRGB)
		delete[] m_pRGB;

	DeleteObject(m_hBMP);
}

BYTE* CImageFile::CopyMonoImage(EColorChannel chn, const RECT* rc)
{
	int imgHeight = rc? rc->bottom - rc->top + 1 : height;
	int imgWidth = rc? rc->right - rc->left + 1 : width;
	int x = rc? rc->left : 0;
	int y = rc? rc->top : 0;

	BYTE *img = new BYTE[imgHeight * imgWidth];

	switch(chn)
	{

	case ECC_RED:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				img[i*imgWidth + j] = m_pRGB[(i+y)*width + j + x].rgbRed;
		break;

	case ECC_GREEN:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				img[i*imgWidth + j] = m_pRGB[(i+y)*width + j + x].rgbGreen;
		break;

	case ECC_BLUE:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				img[i*imgWidth + j] = m_pRGB[(i+y)*width + j + x].rgbBlue;
		break;

	case ECC_HUE:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
			{
				RGBQUAD &q = m_pRGB[(i+y)*width + j + x];
				float r = q.rgbRed/255.0f;
				float g = q.rgbGreen/255.0f;
				float b = q.rgbBlue/255.0f;

				float u = max(r, g);
				u = max(b, u);
				float d = min(r, g);
				d = min(b, d);

				BYTE v;

				if(fabsf(u-d)<EPS)
					v=0;
				else
				{
					float f = 1/(u-d);

					if(fabsf(u-r)<EPS)
					{
						f *= (g-b)*60.f;
					}
					else
					if(fabsf(u-g)<EPS)
					{
						f *= (b-r)*60.f;
						f += 120;
					}
					else
					if(fabsf(u-b)<EPS)
					{
						f *= (r-g)*60.f;
						f += 240;
					}

					v = (BYTE)(f*255.f/360.f);
				}

				img[i*imgWidth + j] = v;
			}

	case ECC_SATURATION:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
			{
				RGBQUAD &q = m_pRGB[(i+y)*width + j + x];
				float r = q.rgbRed/255.0f;
				float g = q.rgbGreen/255.0f;
				float b = q.rgbBlue/255.0f;

				float u = max(r, g);
				u = max(b, u);
				float d = min(r, g);
				d = min(b, d);

				BYTE v;

				if(fabsf(u-d)<EPS)
					v=0;
				else
				{
					float l = (u+d)/2;
					float f = (u-d);

					if(l<=0.5f)
						f /= u+d;
					else
						f /= 2-u-d;

					v = (BYTE)(f*255.f);
				}

				img[i*imgWidth + j] = v;
			}
		break;

	case ECC_LUMINOSITY:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
			{
				RGBQUAD &q = m_pRGB[(i+y)*width + j + x];
				float r = q.rgbRed/255.0f;
				float g = q.rgbGreen/255.0f;
				float b = q.rgbBlue/255.0f;

				float u = max(r, g);
				u = max(b, u);
				float d = min(r, g);
				d = min(b, d);

				BYTE v;

				if(fabsf(u-d)<EPS)
					v=0;
				else
				{
					float f = (u+d)/2;
					v = (BYTE)(f*255.f);
				}

				img[i*imgWidth + j] = v;
			}
		break;
	}

	return img;
}

void CImageFile::PasteMonoImage(const BYTE *img, EColorChannel chn, const RECT* rc)
{
	int imgHeight = rc? rc->bottom - rc->top + 1 : height;
	int imgWidth = rc? rc->right - rc->left + 1 : width;
	int x = rc? rc->left : 0;
	int y = rc? rc->top : 0;

	if(chn >= ECC_EXCLUSIVERED)
		Clear();

	switch(chn)
	{

	case ECC_EXCLUSIVERED:
	case ECC_RED:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				m_pRGB[(i+y)*width + j + x].rgbRed = img[i*imgWidth + j];
		break;

	case ECC_EXCLUSIVEGREEN:
	case ECC_GREEN:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				m_pRGB[(i+y)*width + j + x].rgbGreen = img[i*imgWidth + j];
		break;

	case ECC_EXCLUSIVEBLUE:
	case ECC_BLUE:
		for(int i=0;i<imgHeight;i++)
			for(int j=0;j<imgWidth;j++)
				m_pRGB[(i+y)*width + j + x].rgbBlue = img[i*imgWidth + j];
		break;
	}

}

GaussFilter CreateGaussFilter(int level)
{
	GaussFilter g;
	g.Fact.push_back(1);
	std::vector<int> temp;
	for(register int i=0;i<level;++i)
	{
		temp.resize(0);
		temp.push_back(1);
		for(register int j=1;j<(int)g.Fact.size();++j)
		{
			temp.push_back(g.Fact[j]+g.Fact[j-1]);	
		}
		temp.push_back(1);
		g.Fact=temp;
	}

	g.Sum=0;
	for(register int i=0;i<(int)g.Fact.size();++i)
	{
		g.Sum+=g.Fact[i];
	}
	return g;
}
