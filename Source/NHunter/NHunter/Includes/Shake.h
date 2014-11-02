#ifndef SHAKE_H
#define SHAKE_H

#include "Main.h"

void UpdateShake(float delta);
void MakeShake(int maxFrame, int offset, float delay);
void ClearShake();

#endif
