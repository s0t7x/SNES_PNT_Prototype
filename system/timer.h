#ifndef TIMER_H
#define TIMER_H
#include <snes.h>

// Simple Timer
typedef struct {
    int bActive;
    int bEnded;
    int iTime;
    int iFrameCount;
} timer;

// Timer must be processed each frame
void timerProcess(timer * t){
    if(t->bActive){
        if(t->iTime <= 0){
            t->bEnded = 1;
            t->bActive = 0;
        } else {
            t->iFrameCount++;
            if(t->iFrameCount > (snes_50hz ? 50 : 60)){
                t->iFrameCount = 0;
                t->iTime--;
            }
        }
    }
}

// Starts a timer
void timerStart(timer * t){
    t->bEnded = 0;
    t->iFrameCount = 0;
    t->bActive = 1;
}

// Stops a timer
void timerStop(timer * t){
    t->bActive = 0;
}

#endif