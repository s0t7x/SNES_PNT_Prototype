#ifndef SCENE_HIGHSCORE_TEST_H
#define SCENE_HIGHSCORE_TEST_H
#include <snes.h>

#include "../system/system.h"
#include "menuTest.h"

char _sram_[2];

void sceneHighscoreTest(int score) {
    bgSetEnable(0);
    bgSetDisable(1);
    setFadeEffect(FADE_IN);
    systemClearAllText();
    WaitForVBlank();
    consoleLoadSram((u8*)_sram_, 4);
	consoleDrawText(1,1,"Highscore in SRAM: %d", *(int*)_sram_);
    consoleDrawText(1,3,"YOUR SCORE: %d", score);

    if(score>*(int*)_sram_){
        consoleDrawText(1,4,"Yea you have beaten the highscore!");
        consoleCopySram((u8*)(&score), 2);
    }

    WaitForVBlank();
	systemSleep(2);
	setFadeEffect(FADE_OUT);
	sceneHighscoreTest(score);
}

#endif