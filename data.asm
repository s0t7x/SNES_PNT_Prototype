.include "hdr.asm"

.section ".rodata1" superfree

gfxDefaultFont:
.incbin "sprites/pvsneslibfont.pic"

gfxPipSprites:
.incbin "sprites/pipSprites.pic"
gfxPipSprites_end:

palPipSprites:
.incbin "sprites/pipSprites.pal"
palPipSprites_end:

gfxWeedSprites:
.incbin "sprites/weedSprites.pic"
gfxWeedSprites_end:

palWeedSprites:
.incbin "sprites/weedSprites.pal"
palWeedSprites_end:

bgMap:
.incbin "backgrounds/map.pic"
bgMap_end:

palMap:
.incbin "backgrounds/map.pal"
palMap_end:

mapMap:
.incbin "backgrounds/map.map"
mapMap_end:

.ends

.section ".rodata2" superfree

bgLogo:
.incbin "backgrounds/logo.pic"
bgLogo_end:

palLogo:
.incbin "backgrounds/logo.pal"
palLogo_end:

mapLogo:
.incbin "backgrounds/logo.map"
mapLogo_end:

gfxCopSprites:
.incbin "sprites/copSprites.pic"
gfxCopSprites_end:

palCopSprites:
.incbin "sprites/copSprites.pal"
palCopSprites_end:

.ends