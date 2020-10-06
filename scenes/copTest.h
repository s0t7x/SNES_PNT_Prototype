#ifndef SCENE_COP_TEST_H
#define SCENE_COP_TEST_H
#include <snes.h>

#include "system/system.h"
#include "entities/weed.h"
#include "entities/pip.h"
#include "entities/cop.h"

void sceneCopTest() {
    bgSetEnable(0);
    bgSetEnable(1);
    // Copy tiles to VRAM
    WaitForVBlank();
	bgInitTileSet(1, &bgMap, &palMap, 2, (&bgMap_end - &bgMap), (&palMap_end - &palMap), BG_16COLORS, 0x4000);
	bgInitMapSet(1, &mapMap, (&mapMap_end - &mapMap), SC_64x64, 0x0c00);

    consoleDrawText(1,1, "Pip");
    consoleDrawText(26,1, "Taper");

    pipInit();
    weedInit();
    copInit();

    consoleSetTextCol(RGB15(0,0,0), RGB15(31,31,31));

    setScreenOn();

    WaitForVBlank();

    while (1) {
        // if(entityList[weedEntityId].type == OAM_ENTITY_WEED)
        //     oamSetXY(weedOamId, 50-gPip.bgX, 50-gPip.bgY);
        
        consoleDrawText(1, 2, "%05d", gPip.score);

        pipUpdate();
        copProcess();
        WaitForVBlank();

        weedProcess();
        
        WaitForVBlank();
    }
}


#endif