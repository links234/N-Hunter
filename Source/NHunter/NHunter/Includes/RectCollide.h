#ifndef RECTCOLLIDE_H
#define RECTCOLLIDE_H

#include <windows.h>

bool PointInRectangle(RECT r, POINT p);
bool RectCollideRect(RECT r1, RECT r2);
bool SegIntSeg(POINT A1, POINT A2, POINT B1, POINT B2);

#endif