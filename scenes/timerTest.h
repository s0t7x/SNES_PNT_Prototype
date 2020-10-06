#ifndef SCENE_TIMER_TEST_H
#define SCENE_TIMER_TEST_H
#include <snes.h>

#include "../system/timer.h"
#include "../system/gfxBank.h"

void sceneTimerTest() {
    bgSetEnable(0);

    timer t = {0};
	t.iTime = 5;

	timerStart(&t);
	
	while(1) {
		consoleDrawText(1,1,"Timer Active:   %2d", t.bActive);
		consoleDrawText(1,2,"Timer Ended :   %2d", t.bEnded);
		consoleDrawText(1,3,"Timer Time  :   %2d", t.iTime);
		consoleDrawText(1,4,"Timer Frames:   %2d", t.iFrameCount);

		if(t.bEnded){
			consoleDrawText(1,14,"Timer ended...");
			consoleDrawText(1,15,"Press START to restart Timer");
		} else {
			consoleDrawText(1,14,"              ");
			consoleDrawText(1,15,"                            ");
		}

		timerProcess(&t);
		WaitForVBlank();

		if(padsCurrent(0) == KEY_START) {
			t.iTime = 5;
			timerStart(&t);
		}
	}
}

#endif