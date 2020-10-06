#ifndef COP_H
#define COP_H
#include <snes.h>

#include "../system/system.h"
#include "weed.h"
#include "pip.h"

#define COP_COLLISION_SIZE 32

#define COP_MOVEMENT_SPEED 2

#define COP_PLAYER_AFFINITY 1
#define COP_DECISION_COOLDOWN 1

#define COP_DECISION_COUNT 7

typedef enum {
    COP_DECISION_IDLE,
    COP_DECISION_WALK_UP,
    COP_DECISION_WALK_DOWN,
    COP_DECISION_WALK_LEFT,
    COP_DECISION_WALK_RIGHT,
    COP_DECISION_CHASING_PIP,
    COP_DECISION_WALK_AROUND
} copDecision;

// Global Cop
int gCopEntityId;
int gCopOldPlayerScore;

int copTrueX, copTrueY;

copDecision copCurrentTask;

timer copDecisionTimer;

void copGoOffscreen(){
    int nX, nY;
    nX = rand()%512;
    nY = rand()%512;    

    entityList[gCopEntityId].metaX = nX;
    entityList[gCopEntityId].metaY = nY;

    copCurrentTask = COP_DECISION_IDLE;
}

int copCollidesWithPlayer(){
    int copTrueX, copTrueY;
    copTrueX = (entityList[gCopEntityId].metaX-gPip.bgX)%512;
    copTrueY = (entityList[gCopEntityId].metaY-gPip.bgY)%512;
    // collides with pip?
    if(
                (copTrueX < (gPip.positionX+32)) &&
                ((copTrueX + COP_COLLISION_SIZE) > gPip.positionX+32) &&
                (copTrueY < (gPip.positionY + 32)) &&
                ((copTrueY + COP_COLLISION_SIZE) > gPip.positionY)
            ){
                return 1;
            }

    return 0;
}

// return 1 if colliding else 0
int copCheckForCollision(){
    if(copCollidesWithPlayer()){
        gPip.score = 0;
        copGoOffscreen();
    }

    return 0;
}

void copHide() {
    oamSetEx(entityList[gCopEntityId].oamId, OBJ_SMALL, OBJ_HIDE);
}

void copShow() {
    oamSetEx(entityList[gCopEntityId].oamId, OBJ_SMALL, OBJ_SHOW);
}

void copDrawWithMap(int bgX, int bgY){
    copTrueX = (entityList[gCopEntityId].metaX-bgX)%512;
    copTrueY = (entityList[gCopEntityId].metaY-bgY)%512;
    if(
        (copTrueX < 256) &&
        (copTrueX > 0) &&
        (copTrueY < 224) &&
        (copTrueY > 0)
        ){
        oamSetXY(entityList[gCopEntityId].oamId, copTrueX, copTrueY);
        consoleDrawText(1, 4, "DRAW");
    } else {
        consoleDrawText(1, 4, "    ");
        copHide();
    }
}

void copMakeDecision(){
    copCurrentTask = (copDecision)(rand()%COP_DECISION_COUNT);
}

// Cop must be updated each frame!
void copProcess(){
    int moveX, moveY;
    moveX = moveY = 0;

    int gPipMetaX = gPip.positionX + gPip.bgX;
    int gPipMetaY = gPip.positionY + gPip.bgY;

    if(copDecisionTimer.bEnded){
        if(rand()%10<COP_PLAYER_AFFINITY) copCurrentTask = COP_DECISION_CHASING_PIP;
        else copMakeDecision();
        copDecisionTimer.iTime = COP_DECISION_COOLDOWN;
        timerStart(&copDecisionTimer);
    }

    switch (copCurrentTask)
    {
    case COP_DECISION_WALK_UP:
        moveY-=COP_MOVEMENT_SPEED;
        break;
    case COP_DECISION_WALK_DOWN:
        moveY+=COP_MOVEMENT_SPEED;
        break;
    case COP_DECISION_WALK_LEFT:
        moveX-=COP_MOVEMENT_SPEED;
        break;
    case COP_DECISION_WALK_RIGHT:
        moveX+=COP_MOVEMENT_SPEED;
        break;
    case COP_DECISION_CHASING_PIP:
        if(gPipMetaX>entityList[gCopEntityId].metaX+33){
            moveX+=COP_MOVEMENT_SPEED;
        } else
        if(gPipMetaX+33<entityList[gCopEntityId].metaX){
            moveX-=COP_MOVEMENT_SPEED;
        }
        if(gPipMetaY>entityList[gCopEntityId].metaY+33){
            moveY+=COP_MOVEMENT_SPEED;
        } else
        if(gPipMetaY+33<entityList[gCopEntityId].metaY){
            moveY-=COP_MOVEMENT_SPEED;
        }
        break;
    case COP_DECISION_IDLE:
    default:
        break;
    }
    if(copCheckForCollision()){
        // collision detected
    } else {
        entityList[gCopEntityId].metaX = (entityList[gCopEntityId].metaX+moveX)%512;
        entityList[gCopEntityId].metaY = (entityList[gCopEntityId].metaY+moveY)%512;
    }
    if(COP_DECISION_CHASING_PIP == copCurrentTask){
        if(
        (copTrueX < 256) &&
        (copTrueX > 0) &&
        (copTrueY < 224) &&
        (copTrueY > 0)
        ){
            if(gPip.score > gCopOldPlayerScore){
                gPip.score = 0;
                copGoOffscreen();
            }
            consoleDrawText(2,5, "s");
        } else {
            consoleDrawText(2,5, " ");
        }
        consoleDrawText(1,5, "c");
        
    } else {
        consoleDrawText(1,5, "  ");
        gCopOldPlayerScore = gPip.score;
    }

    copDrawWithMap(gPip.bgX, gPip.bgY);
    timerProcess(&copDecisionTimer);
}

void copInit() {
    gCopEntityId = (int)oamAddEntity(&gfxCopSprites, (&gfxCopSprites_end-&gfxCopSprites), &palCopSprites, (&palCopSprites_end-&palCopSprites), 3);
    oamSet(entityList[gCopEntityId].oamId, 0, 0, 3, 0, 0, 0x80, 3);
    entityList[gCopEntityId].metaX = rand()%512;
    entityList[gCopEntityId].metaY = rand()%512;
    oamSetEx(entityList[gCopEntityId].oamId, OBJ_SMALL, OBJ_SHOW);
    copDecisionTimer.iTime = COP_DECISION_COOLDOWN;
    timerStart(&copDecisionTimer);
}


#endif