#ifndef PIP_H
#define PIP_H
#include <snes.h>

#include "../system/system.h"
#include "weed.h"

#define PIP_ENTITY_ID      0
#define PIP_PAD_ID      0
#define PIP_MAP_BG_ID   1

#define PIP_CAMERA_MARGIN_X     64
#define PIP_CAMERA_MARGIN_Y     64
#define PIP_CAMERA_MARGIN_X_LEFT_BORDER     0   + PIP_CAMERA_MARGIN_X
#define PIP_CAMERA_MARGIN_X_RIGHT_BORDER    254 - 32 - PIP_CAMERA_MARGIN_X
#define PIP_CAMERA_MARGIN_Y_TOP_BORDER      0   + PIP_CAMERA_MARGIN_Y
#define PIP_CAMERA_MARGIN_Y_BOTTOM_BORDER   224 - 32 - PIP_CAMERA_MARGIN_Y

#define PIP_MOVEMENT_SPEED 2

// Global Pip
struct PlayerPip_s {
    int positionX;
    int positionY;
    int bgX;
    int bgY;
    int hFlip;
    int score;
} gPip;

// void pipMoveEntitiesOnMap(int bgX, int bgY){
//     int i;
//     if(bgX==0 && bgY==0) return;
//     for( i = 0; i < _oam_id_count; i++ ){
//          if(entityList[i].moveWithMap){}
//             oamSetXY(entityList[i].oamId, (int)oamGetX(entityList[i].oamId)-bgX, (int)oamGetY(entityList[i].oamId)-bgY);
//     }
// }

// return 1 if colliding else 0
int pipCheckForCollision(int relativeX, int relativeY){
    // int i;
    // for(i = 0; i < _oam_id_count; i++){
    //     if(entityList[i].type == OAM_ENTITY_WEED){
    //         int weedTopLeftX = (int)oamGetX(entityList[i].oamId);
    //         int weedTopLeftY = (int)oamGetY(entityList[i].oamId);
    //         if(
    //             weedTopLeftX < gPip.positionX+relativeX + 32 &&
    //             weedTopLeftX + 32 > gPip.positionX+relativeX &&
    //             weedTopLeftY < gPip.positionY+relativeY + 32 &&
    //             weedTopLeftY + 32 > gPip.positionY+relativeY
    //         ){
    //             oamSetEx(entityList[i].oamId, OBJ_SMALL, OBJ_HIDE);
    //             entityList[i].type = OAM_ENTITY_NUL;
    //             characterTestWeedCount--;
    //             characterTestMaybeSpawnNewWeed();
    //             gPip.score += 500;
    //             return 1;
    //         }
    //     }
    // }

    int w = weedGetFromPosition(gPip.positionX+relativeX, gPip.positionY+relativeY, 32);
    
    if(w>-1){
        weedDeactivate(w);
        gWeedArray[w]->isSpawned = 0;
        weedRespawnQueuePush(w);
        gPip.score += 5;
        return 1;
    }

    return 0;
}

// Pip must be updated each frame!
void pipUpdate(){
    unsigned short pad = padsCurrent(PIP_PAD_ID);

    int moveX, moveY, bgX, bgY;
    moveX = moveY = bgX = bgY = 0;

    if (pad & KEY_UP){
        if(gPip.positionY > PIP_CAMERA_MARGIN_Y_TOP_BORDER){
            moveY-=PIP_MOVEMENT_SPEED;
        }
        else
        {
            bgY-=PIP_MOVEMENT_SPEED;
        }
        
    }
    if (pad & KEY_DOWN){
        if(gPip.positionY < PIP_CAMERA_MARGIN_Y_BOTTOM_BORDER)
            moveY+=PIP_MOVEMENT_SPEED;
        else
        {
            bgY+=PIP_MOVEMENT_SPEED;
        }
    }
    if (pad & KEY_LEFT){
        if(gPip.positionX > PIP_CAMERA_MARGIN_X_LEFT_BORDER) {
            moveX-=PIP_MOVEMENT_SPEED;
            gPip.hFlip = 1;
        } else {
            bgX-=PIP_MOVEMENT_SPEED;
        }
    }
    if (pad & KEY_RIGHT){
        if(gPip.positionX < PIP_CAMERA_MARGIN_X_RIGHT_BORDER) {
            moveX+=PIP_MOVEMENT_SPEED;
            gPip.hFlip = 0;
        } else {
            bgX+=PIP_MOVEMENT_SPEED;
        }
    }
    if(pipCheckForCollision(moveX, moveY)){
        // collision detected
    } else {
        gPip.positionX += moveX;
        gPip.positionY += moveY;
        gPip.bgX += bgX;
        gPip.bgY += bgY;
        if(gPip.bgX<-256) gPip.bgX += 512;
        if(gPip.bgX>256) gPip.bgX -= 512;
        if(gPip.bgY<-256) gPip.bgY += 512;
        if(gPip.bgY>256) gPip.bgY -= 512;
        // gPip.bgX += bgX;
        // gPip.bgY += bgY;
        bgSetScroll(PIP_MAP_BG_ID, gPip.bgX, gPip.bgY);
        //pipMoveEntitiesOnMap(bgX, bgY);
        weedMoveWithMap(gPip.bgX, gPip.bgY);
    }
    oamSet(PIP_ENTITY_ID, gPip.positionX, gPip.positionY, 3, gPip.hFlip, 0, 0, 0);
}

void pipInit() {
    gPip.positionX = 112;
    gPip.positionY = 96;
    gPip.hFlip = 0;
    oamSetEx(PIP_ENTITY_ID, OBJ_SMALL, OBJ_SHOW);
}

void pipHide() {
    oamSetEx(PIP_ENTITY_ID, OBJ_SMALL, OBJ_HIDE);
}

void pipShow() {
    oamSetEx(PIP_ENTITY_ID, OBJ_SMALL, OBJ_SHOW);
}

#endif