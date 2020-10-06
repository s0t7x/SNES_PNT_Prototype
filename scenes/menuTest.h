#ifndef SCENE_MENU_TEST_H
#define SCENE_MENU_TEST_H
#include <snes.h>

#include "../system/system.h"
#include "characterTest.h"

#include "soundbank.h"
extern char __SOUNDBANK__;


void sceneMenuTest() {
    bgSetEnable(0);
	bgSetEnable(1);

	int bMainMenuActive = 1;

	
    spcSetBank(&__SOUNDBANK__);

	spcLoad(MOD_PNTMENU);

	
	spcPlay(0);
	spcSetModuleVolume(150);

	systemClearAllText();
	bgInitTileSet(1, &bgLogo, &palLogo, 2, (&bgLogo_end - &bgLogo), (&palLogo_end - &palLogo), BG_16COLORS, 0x4000);
	bgInitMapSet(1, &mapLogo, (&mapLogo_end - &mapLogo), SC_32x32, 0x0c00);

	consoleSetTextCol(RGB15(31,31,31), RGB15(0,0,0));

	int cursorPosition = 0;

	setFadeEffect(FADE_IN);

	while(bMainMenuActive){
		if(cursorPosition < 0) cursorPosition = 0;
		if(cursorPosition > 1) cursorPosition = 1;

		consoleDrawText(12,20,"%c1 Player", cursorPosition ? ' ' : '>');
		consoleDrawText(12,21,"%c2 Player", cursorPosition ? '>' : ' ');

		switch (padsCurrent(0)) {
			case KEY_DOWN	: cursorPosition++; break;
			case KEY_UP		: cursorPosition--; break;
			case KEY_START  :
				bMainMenuActive = 0;
		}

		spcProcess();
		WaitForVBlank();
	}
	spcStop();
	spcProcess();
	spcStop();
	
	WaitForVBlank();
	if(cursorPosition == 0){
		systemClearAllText();
		sceneCharacterTest();    
	}
}

#endif