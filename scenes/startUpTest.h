#ifndef SCENE_STARTUP_TEST_H
#define SCENE_STARTUP_TEST_H
#include <snes.h>

#include "../system/system.h"
#include "menuTest.h"

void sceneStartUpTest() {
    bgSetEnable(0);
	consoleDrawText(6,14,"(c) s0t7x   1996-2020");
	systemSleep(5);
	setFadeEffect(FADE_OUT);
	sceneMenuTest();
}

#endif