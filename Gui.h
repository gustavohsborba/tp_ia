#ifndef AIDEMO14_1_INPUT_H
#define AIDEMO14_1_INPUT_H

#include <cstdio>
#include <GL/glut.h>


#define _WINWIDTH 800
#define _WINHEIGHT 600

void initGui(int argc, char **argv, void (*drawFuncs)(), void (*updateFunc)(int));

bool IsKeyDown(int key);
bool IsKeyDownNow(int key);

#endif
