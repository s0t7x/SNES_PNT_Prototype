#ifndef OAM_H
#define OAM_H
#include <snes.h>
#include "gfxBank.h"

#define OAM_MEMORY_SPACE 0x6000

typedef enum {
    OAM_ENTITY_NUL,
    OAM_ENTITY_WEED
} oamEntityTypes;

typedef struct entity entity;
struct entity {
    u8 moveWithMap;
    u16 oamId;
    int metaX;
    int metaY;
    u8 isSpawned;
    u16 vramPtr;
    oamEntityTypes type;
};

static entity entityList[20];
static int _oam_id_count;
static int _oam_vram_mx;

int oamAddEntity(u8* gfxSprite, u16 gfxSize, u8* tilePalette, u16 paletteSize, u8 paletteId) {
    entity e = { 0 };
    e.oamId = _oam_id_count*4;
    e.vramPtr = OAM_MEMORY_SPACE + 0x0400 * _oam_vram_mx;
    oamInitGfxSet(gfxSprite, gfxSize, tilePalette, paletteSize, paletteId, e.vramPtr, OBJ_SIZE32);
    int entityIndex = _oam_id_count;
    entityList[entityIndex] = e;
    _oam_id_count++;
    _oam_vram_mx++;
    return entityIndex;
}

int oamAddCloneEntity(int x, int y, u8 vramOffset, u8 paletteId){
    entity e = { 0 };
    e.oamId = _oam_id_count*4;
    oamSet(e.oamId, x, y, 3, 0, 0, vramOffset, paletteId);
    int entityIndex = _oam_id_count;
    entityList[entityIndex] = e;
    oamSetEx(e.oamId, OBJ_SMALL, OBJ_SHOW);
    _oam_id_count++;
    return entityIndex;
}

void oamLoadGfxBank() {
    // ID 0 - Pip
    oamAddEntity(&gfxPipSprites, (&gfxPipSprites_end-&gfxPipSprites), &palPipSprites, (&palPipSprites_end-&palPipSprites), 0);
    // ID 4 - Weed
    //oamAddEntity(&gfxWeedSprites, (&gfxWeedSprites_end-&gfxWeedSprites), &palWeedSprites, (&palWeedSprites_end-&palWeedSprites), 1);
}

#endif