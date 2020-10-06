#ifndef SYSTEM_H
#define SYSTEM_H
#include <snes.h>
#include <stdio.h>
#include "timer.h"
#include "oam.h"

extern void consoleVblank(void);

void systemClearAllText(){
    int i;
    for(i = 0; i < 32; i++){
        consoleDrawText(0, i,"                                 ");
    }
    WaitForVBlank();
}

void systemVBLANKinterrupt(){
    setBrightness(0);
    WaitForVBlank();
}

void systemVBLANK(void) {
	// u8 *pgfx;
	// u16 padrgfx;
	
	// // Refresh pad values
	// scanPads();

	// if tile sprite queued 
	// if (spr_queue != 0xff) {
	// 	if (spr_mutex == 0) { // if we have finished adding things 
	// 		// copy memory to vram (2 tiles of the 16x16 sprites)
	// 		while (spr_queue != 0xff) {
	// 			pgfx = sprqueue[spr_queue].gfxoffset;
	// 			padrgfx = sprqueue[spr_queue].adrgfxvram;
	// 			dmaCopyVram(pgfx, padrgfx, 8*4*2); dmaCopyVram(pgfx+8*4*16, padrgfx+8*4*8, 8*4*2);
	// 			spr_queue--;
	// 		}
	// 	}
	// }

    consoleVblank();

	// // Put oam to screen if needed
	//dmaCopyOAram((unsigned char *) &oamMemory,0x2000,0x220);
	
	// // count vbls
	//snes_vblank_count++;
}

void systemInit() {
    //general
    consoleInit();

    //boot SPC-700 Audio Chip
    spcBoot();
    spcAllocateSoundRegion(39);

    //vfx
    setMode(BG_MODE1, BG3_MODE1_PRORITY_HIGH);
    bgSetDisable(0);
    bgSetDisable(1);
    bgSetDisable(2);

    //gfx
    oamInit();
    _oam_id_count = 0;
    _oam_vram_mx = 0;
    oamLoadGfxBank();

    //we want text everywhere
    consoleInitText(0, 0, &gfxDefaultFont);

    //set VBLANK function
    __nmi_handler = systemVBLANK;

    // done
    setScreenOn();
}

void systemSleep(int s) {
    int framesToSleep = s * (snes_50hz ? 50 : 60);
    while(framesToSleep){
        WaitForVBlank();
        framesToSleep--;
    }
}

void systemMemMove(void *dest, void *src, u8 n) 
{ 
   // Typecast src and dest addresses to (char *) 
   char *csrc = (char *)src; 
   char *cdest = (char *)dest; 
  
   // Create a temporary array to hold data of src 
   char temp[128]; 
  
   int i;
   // Copy data from csrc[] to temp[] 
   for (i=0; i<n; i++) 
       temp[i] = csrc[i]; 
  
   // Copy data from temp[] to cdest[] 
   for (i=0; i<n; i++) 
       cdest[i] = temp[i];
} 

#endif