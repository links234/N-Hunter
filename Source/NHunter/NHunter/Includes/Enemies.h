#ifndef ENEMIES_H
#define EMEMIES_H
#include "..\\Includes\\Main.h"
#include "..\\Includes\\Objects.h"

#define	BOSSHP		7500
#define BOSSHALFHP	BOSSHP/2
#define BOSS25HP	BOSSHP/4

class MovingDoor;

void AddEnemyT1(int PozX,int PoxY,int LeftLimit,int RightLimit,MovingDoor *Door,bool mIsMoving);
void AddBoss();
bool SegInRect(Vec2 P1,Vec2 P2,RECT Rect);

#endif