TARGET		:= KETM
SOURCES		:= src
			
INCLUDES	:= include

LIBS =   -lSceLibKernel_stub -lSceAppMgr_stub -lm -lSceNet_stub -lSceNetCtl_stub \
    -lSceAppUtil_stub -lScePower_stub -lmad -lSDL_mixer -lvorbisfile -lvorbis -logg \
	-lSDL -lSDL_image -lSceAudio_stub -lc -lvita2d -lScePvf_stub -lScePgf_stub -ljpeg -lpng16 \
	-lz -lSceDisplay_stub -lfreetype -lSceGxm_stub -lSceCtrl_stub -lSceSysmodule_stub \
	-lSceCommonDialog_stub -lSceHid_stub

CFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.cpp))
BINFILES := $(foreach dir,$(DATA), $(wildcard $(dir)/*.bin))
OBJS     := $(addsuffix .o,$(BINFILES)) $(CFILES:.c=.o) $(CPPFILES:.cpp=.o) 

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CXX      = $(PREFIX)-g++
CFLAGS  = -fno-lto -g -Wl,-q -O3
CXXFLAGS  = $(CFLAGS) -fno-exceptions -std=gnu++11 -fpermissive
ASFLAGS = $(CFLAGS)

all: $(TARGET).vpk

$(TARGET).vpk: $(TARGET).velf
	vita-mksfoex -s TITLE_ID=KETM00001 "K.E.T.M." param.sfo
	cp -f param.sfo sce_sys/param.sfo
	
	#------------ Comment this if you don't have 7zip ------------------
	7z a -tzip $(TARGET).vpk -r sce_sys/* eboot.bin 
	#-------------------------------------------------------------------

%.velf: %.elf
	cp $< $<.unstripped.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@
	vita-make-fself -s $@ eboot.bin

$(TARGET).elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf $(TARGET).velf $(TARGET).elf $(OBJS)
