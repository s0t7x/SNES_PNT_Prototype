# path to snesdev root directory (for emulators, devkitsnes, libsnes)
export DEVKITSNES := ./../../

# path to devkitsnes root directory for compiler
export DEVKIT65XX := $(DEVKITSNES)devkitsnes

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
SOURCES		:=	.

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	+=	$(INCLUDE) 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(PVSNESLIB)
LIBOBJS +:=	
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))

export AUDIOFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.it)))

ifneq ($(AUDIOFILES),)
ifeq ($(wildcard soundbank.asm),)
	SFILES		:=	soundbank.asm $(SFILES)
endif
endif
 
#---------------------------------------------------------------------------------
export OFILES	:=	$(BINFILES:.bin=.obj) $(CFILES:.c=.obj) $(SFILES:.asm=.obj)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)

GTITLE 		:= -ht"$(TARGET)"
 
.PHONY: bitmaps all

#---------------------------------------------------------------------------------
all	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc
	@cp -f $(TARGET).sfc $(CURDIR)/ROM/$(TARGET).sfc
	@rm -f -R $(OFILES) $(TARGET).sfc $(TARGET).sym */*.pic */*.map */*.pal

clean:
	@echo clean ...
	@rm -f -R $(OFILES) $(TARGET).sfc $(TARGET).sym */*.pic */*.map */*.pal *.xcf

#---------------------------------------------------------------------------------
soundbank.asm : $(AUDIOFILES)
	@echo Compiling Soundbank ... 
	@$(SMCONV) -l -s -o soundbank -v -b 5 $^

#---------------------------------------------------------------------------------
pvsneslibfont.pic: sprites/pvsneslibfont.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs8 -po2 -pc16 -pe0 -mR! -m! -p! $<

pipSprites.pic: sprites/pipSprites.bmp
	@echo convert Sprite ... $(notdir $@)
	$(GFXCONV) -gs32 -pc16 -po16 -n $<

map.pic: backgrounds/map.bmp
	@echo convert bg with tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs16 -po16 -pc16 -pe2 -m $<

weedSprites.pic: sprites/weedSprites.bmp
	@echo convert Sprite ... $(notdir $@)
	$(GFXCONV) -gs32 -pc16 -po4 -pe1 -n $<

copSprites.pic: sprites/copSprites.bmp
	@echo convert Sprite ... $(notdir $@)
	$(GFXCONV) -gs32 -pc16 -po11 -pe2 -n $<

logo.pic: backgrounds/logo.bmp
	@echo convert bg ... $(notdir $@)
	$(GFXCONV) -n -gs8 -po16 -pc16 -pe2 -m -p $<

bitmaps : soundbank.asm pvsneslibfont.pic pipSprites.pic map.pic weedSprites.pic logo.pic copSprites.pic
#---------------------------------------------------------------------------------
$(OUTPUT).sfc	: $(OFILES)