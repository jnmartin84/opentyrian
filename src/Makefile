ROOTDIR			=$(N64_INST)


GCCN64PREFIX = $(ROOTDIR)/bin/mips64-elf-
CHKSUM64PATH = $(ROOTDIR)/bin/chksum64
MKDFSPATH = $(ROOTDIR)/bin/mkdfs
MKSPRITEPATH = $(ROOTDIR)/bin/mksprite
HEADERPATH = $(ROOTDIR)/mips64-elf/lib
N64TOOL = $(ROOTDIR)/bin/n64tool
HEADERNAME = header


LIBS = -ldragon -lc -lm -ldragonsys -lc
LINK_FLAGS = -G4 -L$(ROOTDIR)/lib -L$(ROOTDIR)/mips64-elf/lib $(LIBS) -Tn64.ld
PROG_NAME = opentyrian
CFLAGS = -Wno-format-overflow -Wno-format-truncation -mno-shared -mno-abicalls -mno-branch-likely -mno-llsc -mno-check-zero-division -std=gnu99 -mabi=o64 -std=gnu99 -march=vr4300 -mtune=vr4300 -Wall -g -G4 -O3 -I$(ROOTDIR)/include -I$(ROOTDIR)/mips64-elf/include


CC = $(GCCN64PREFIX)gcc
AS = $(GCCN64PREFIX)as
LD = $(GCCN64PREFIX)ld
OBJCOPY = $(GCCN64PREFIX)objcopy
OBJDUMP = $(GCCN64PREFIX)objdump


O=obj

OBJS=				\
		animlib.o	\
		arg_parse.o	\
		backgrnd.o	\
		config_file.o	\
		config.o	\
		destruct.o	\
		editship.o	\
		episodes.o	\
		file.o		\
		font.o		\
		fonthand.o	\
		game_menu.o	\
		helptext.o	\
		joystick.o	\
		jukebox.o	\
		keyboard.o	\
		lds_play.o	\
		loudness.o	\
		lvllib.o	\
		lvlmast.o	\
		mainint.o	\
		menus.o		\
		mouse.o		\
		mtrand.o	\
		musmast.o	\
		network.o	\
		nortsong.o	\
		nortvars.o	\
		opentyr.o	\
		opl.o		\
		palette.o	\
		params.o	\
		pcxload.o	\
		pcxmast.o	\
		picload.o	\
		player.o	\
		shots.o		\
		sizebuf.o	\
		sndmast.o	\
		sprite.o	\
		starlib.o	\
		tyrian2.o	\
		varz.o		\
		vga_palette.o	\
		vga256d.o	\
		video_scale.o	\
		video.o	\
		xmas.o
		

$(PROG_NAME).z64: $(PROG_NAME).elf rom.dfs
	$(OBJCOPY) $(PROG_NAME).elf $(PROG_NAME).bin -O binary
	rm -f $(PROG_NAME).z64
	$(N64TOOL) -l 16M -t "OPENTYRI" -h $(HEADERPATH)/$(HEADERNAME) -o $(PROG_NAME).z64 $(PROG_NAME).bin -s 1M rom.dfs
	$(CHKSUM64PATH) $(PROG_NAME).z64


$(PROG_NAME).v64: $(PROG_NAME).elf rom.dfs
	$(OBJCOPY) $(PROG_NAME).elf $(PROG_NAME).bin -O binary
	rm -f $(PROG_NAME).v64
	$(N64TOOL) -b -l 16M -t "OPENTYRI" -h $(HEADERPATH)/$(HEADERNAME) -o $(PROG_NAME).v64 $(PROG_NAME).bin -s 1M rom.dfs
	$(CHKSUM64PATH) $(PROG_NAME).v64


$(PROG_NAME).elf : $(OBJS) $(ASM)
	$(LD) -o $(PROG_NAME).elf $(OBJS) $(LINK_FLAGS)
	$(OBJDUMP) -t $(PROG_NAME).elf > $(PROG_NAME)_symbols.txt
	cat $(PROG_NAME)_symbols.txt | grep 'F .text' > $(PROG_NAME)_functions.txt

copy: $(PROG_NAME).z64
	cp $(PROG_NAME).z64 ~/


all: $(PROG_NAME).z64


clean:
	rm -f *.z64 *.v64 *.elf *.bin *.dfs $(PROG_NAME)_symbols.txt $(PROG_NAME)_functions.txt
	rm -f *.o

%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

rom.dfs:
	$(MKDFSPATH) rom.dfs ../tyrian21/

#############################################################
#
#############################################################
