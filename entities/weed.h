#ifndef WEED_H
#define WEED_H
#include <snes.h>

#include "../system/system.h"

#define WEED_MAX 8
#define WEED_RESPAWN_TIME 4
#define WEED_SPAWN_AREA 512

int gWeedActiveCount;

entity * gWeedArray[WEED_MAX];
int gWeedRespawnQueue[WEED_MAX];
u8 _weed_array_cursor;
u8 _weed_respawn_queue_cursor;

timer gWeedRespawnTimer;

int weedRespawnQueuePop(){
    if(_weed_respawn_queue_cursor > 0){
        _weed_respawn_queue_cursor--;
        return gWeedRespawnQueue[_weed_respawn_queue_cursor];
    }
    return -1;
}

int weedRespawnQueuePush(int index){
    if(_weed_respawn_queue_cursor < WEED_MAX){
        gWeedRespawnQueue[_weed_respawn_queue_cursor] = index;
        _weed_respawn_queue_cursor++;
        return 1;
    }
    return 0;
}

void weedClearRespawnQueue(){
    int i;
    for(i = 0; i < WEED_MAX; i++)
        gWeedRespawnQueue[i] = -1;
}

void weedClearArray(){
    int i;
    for(i = 0; i < WEED_MAX; i++)
        gWeedArray[i] = 0;
}

void weedAdd(u8 index, entity * e){
    if(index < WEED_MAX)
        gWeedArray[index] = e;
}

void weedDeactivate(u8 index){
    if(gWeedArray[index]==0) return;
    gWeedArray[index]->type = OAM_ENTITY_NUL;
    oamSetEx(gWeedArray[index]->oamId,OBJ_SMALL, OBJ_HIDE);
}

void weedActivate(u8 index){
    if(gWeedArray[index]==0) return;
    gWeedArray[index]->type = OAM_ENTITY_WEED;
    oamSetEx(gWeedArray[index]->oamId,OBJ_SMALL, OBJ_SHOW);
}

void weedRespawn(){
    int w = weedRespawnQueuePop();
    if(w > -1){
        if(gWeedArray[w]==0) return;
        gWeedArray[w]->metaX = rand()%WEED_SPAWN_AREA;
        gWeedArray[w]->metaY = rand()%WEED_SPAWN_AREA;
        gWeedArray[w]->isSpawned = 1;
        gWeedArray[w]->moveWithMap = 1;
    }
}

int weedGetFromPosition(int x, int y, int w) {
    int i;
    for(i = 0; i < WEED_MAX; i++){
        if(gWeedArray[i]==0) continue;
        if(gWeedArray[i]->isSpawned){
            int weedTopLeftX = (int)oamGetX(gWeedArray[i]->oamId);
            int weedTopLeftY = (int)oamGetY(gWeedArray[i]->oamId);
            if(
                (weedTopLeftX < (x + w)) &&
                ((weedTopLeftX + w) > x) &&
                (weedTopLeftY < (y + w)) &&
                ((weedTopLeftY + w) > y)
            ){
                return i;
            }
        }
    }
    return -1;
}

void weedMoveWithMap(int bgX, int bgY){
    int i;
    //consoleDrawText(1,4,"bgX:%03d, bgY:%03d", bgX, bgY);
    //consoleDrawText(1,6,"respawn queue:%d", (int)_weed_respawn_queue_cursor);
    for( i = 0; i < WEED_MAX; i++ ){
         if(gWeedArray[i]->moveWithMap && gWeedArray[i]->isSpawned){
            int trueX, trueY;
            trueX = (gWeedArray[i]->metaX-bgX)%512;
            trueY = (gWeedArray[i]->metaY-bgY)%512;
            if(
                (trueX < 256) &&
                (trueX > 0) &&
                (trueY < 224) &&
                (trueY > 0)
                ){
                oamSetXY(gWeedArray[i]->oamId, trueX, trueY);
                weedActivate(i);
                // consoleDrawText(1,3,"mtX:%03d, mtY:%03d", gWeedArray[i]->metaX, gWeedArray[i]->metaY);
                //consoleDrawText(1,5,"trX:%03d, trY:%03d", trueX, trueY);
            } else {
                weedDeactivate(i);
            }
         }
    }
}

void weedInit(){
    _weed_array_cursor = 0;
    _weed_respawn_queue_cursor = 0;

    weedClearRespawnQueue();
    
    int weedEntityId = oamAddEntity(&gfxWeedSprites, (&gfxWeedSprites_end-&gfxWeedSprites), &palWeedSprites, (&palWeedSprites_end-&palWeedSprites), 2);
    u16 spriteOffset = entityList[weedEntityId].oamId * 0x10;
    int wX, wY;
    wX = rand()%512;
    wY = rand()%512;
    entityList[weedEntityId].metaX = wX;
    entityList[weedEntityId].metaY = wY;
    oamSet(entityList[weedEntityId].oamId, wX, wY, 3, 0, 0, spriteOffset, 0);
    entityList[weedEntityId].moveWithMap = 1;
    entityList[weedEntityId].isSpawned = 1;
    weedAdd(0, &entityList[weedEntityId]);

    int i;
    for(i = 1; i < 3; i++){
        wX = rand()%512;
        wY = rand()%512;
        entityList[weedEntityId].metaX = wX;
        entityList[weedEntityId].metaY = wY;
        int weed2Id = oamAddCloneEntity(wX,wY, spriteOffset, 0);
        entityList[weed2Id].moveWithMap = 1;
        entityList[weed2Id].isSpawned = 1;
        weedAdd(i, &entityList[weed2Id]);
    }
    for(i = 3; i < WEED_MAX; i++){
        int weed2Id = oamAddCloneEntity(0,0, spriteOffset, 0);
        entityList[weed2Id].moveWithMap = 1;
        entityList[weed2Id].isSpawned = 0;
        weedAdd(i, &entityList[weed2Id]);
        weedDeactivate(i);
        weedRespawnQueuePush(i);
    }

    gWeedRespawnTimer.iTime = 1;
    timerStart(&gWeedRespawnTimer);
    weedMoveWithMap(0,0);
}

void weedProcess(){
    if(gWeedRespawnTimer.bEnded){
        weedRespawn();
        gWeedRespawnTimer.iTime = 1+(rand()%(WEED_RESPAWN_TIME-1));
        timerStart(&gWeedRespawnTimer);
    }
    timerProcess(&gWeedRespawnTimer);
}


#endif