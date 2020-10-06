#ifndef SCENE_CHARACTER_TEST_H
#define SCENE_CHARACTER_TEST_H
#include <snes.h>

#include "system/system.h"
#include "entities/weed.h"
#include "entities/pip.h"

#include "soundbank.h"
extern char __SOUNDBANK__;

void sceneCharacterTest() {
    bgSetEnable(0);
    bgSetEnable(1);
    // Copy tiles to VRAM
    WaitForVBlank();
	bgInitTileSet(1, &bgMap, &palMap, 2, (&bgMap_end - &bgMap), (&palMap_end - &palMap), BG_16COLORS, 0x4000);
	bgInitMapSet(1, &mapMap, (&mapMap_end - &mapMap), SC_64x64, 0x0c00);

    spcSetBank(&__SOUNDBANK__);

	spcLoad(MOD_POLLEN8);
	
	spcPlay(1);
	spcSetModuleVolume(150);

    consoleDrawText(1,1, "Pip");
    consoleDrawText(26,1, "Taper");
    consoleDrawText(26,2, "-n/a-");
    pipInit();
    weedInit();

    consoleSetTextCol(RGB15(0,0,0), RGB15(31,31,31));

    timer beginTimer;
    beginTimer.iTime = 3;
    
    timer roundTimer;
    roundTimer.iTime = 260;

    setScreenOn();

    timerStart(&beginTimer);
    while(beginTimer.bActive){
        if(beginTimer.iTime>2)
            consoleDrawText(1, 14, "          AUFROLLEN           ");
        else if(beginTimer.iTime>1)
            consoleDrawText(1, 14, "           ANHAUEN            ");
        else
            consoleDrawText(1, 14, "            SPLIFF            ");
        WaitForVBlank();
        timerProcess(&beginTimer);
        spcProcess();
    }
    consoleDrawText(1, 14, "                              ");
    WaitForVBlank();

    timerStart(&roundTimer);

    while (1) {
        // if(entityList[weedEntityId].type == OAM_ENTITY_WEED)
        //     oamSetXY(weedOamId, 50-gPip.bgX, 50-gPip.bgY);
        
        consoleDrawText(14, 1, "%02d:%02d", roundTimer.iTime / 60, roundTimer.iTime % 60);
        consoleDrawText(1, 2, "%05d", gPip.score);
        pipUpdate();
        weedProcess();
        timerProcess(&roundTimer);
        spcProcess();
        WaitForVBlank();
    }
}


#endif