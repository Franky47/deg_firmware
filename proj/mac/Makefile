
#########  AVR Project Makefile Template   #########
######                                        ######
######    Copyright (C) 2003-2005,Pat Deegan, ######
######            Psychogenic Inc             ######
######          All Rights Reserved           ######
######                                        ######
###### You are free to use this code as part  ######
###### of your own applications provided      ######
###### you keep this copyright notice intact  ######
###### and acknowledge its authorship with    ######
###### the words:                             ######
######                                        ######
###### "Contains software by Pat Deegan of    ######
###### Psychogenic Inc (www.psychogenic.com)" ######
######                                        ######
###### If you use it as part of a web site    ######
###### please include a link to our site,     ######
###### http://electrons.psychogenic.com  or   ######
###### http://www.psychogenic.com             ######
######                                        ######
####################################################


##### This Makefile will make compiling Atmel AVR 
##### micro controller projects simple with Linux 
##### or other Unix workstations and the AVR-GCC 
##### tools.
#####
##### It supports C, C++ and Assembly source files.
#####
##### Customize the values as indicated below and :
##### make
##### make disasm 
##### make stats 
##### make hex
##### make upload
##### make gdbinit
##### or make clean
#####
##### See the http://electrons.psychogenic.com/ 
##### website for detailed instructions


####################################################
#####                                          #####
#####              Configuration               #####
#####                                          #####
##### Customize the values in this section for #####
##### your project. MCU, PROJECTNAME and       #####
##### SRC must be setup for all projects,      #####
##### the remaining variables are only         #####
##### relevant to those needing additional     #####
##### include dirs or libraries and those      #####
##### who wish to use the avrdude programmer   #####
#####                                          #####
##### See http://electrons.psychogenic.com/    #####
##### for further details.                     #####
#####                                          #####
####################################################


#####         Target Specific Details          #####
#####     Customize these for your project     #####

# Name of target controller 
# (e.g. 'at90s8515', see the available avr-gcc mmcu 
# options for possible values)
MCU=attiny84

# CPU Speed
F_CPU=16000000

# id to use with programmer
# default: PROGRAMMER_MCU=$(MCU)
# In case the programer used, e.g avrdude, doesn't
# accept the same MCU name as avr-gcc (for example
# for ATmega8s, avr-gcc expects 'atmega8' and 
# avrdude requires 'm8')
PROGRAMMER_MCU=t84

# Name of our project
# (use a single word, e.g. 'myproject')
# PROJECTPATH=$(shell pwd)		# This version took the root name
# PROJECTNAME=$(notdir $(PROJECTPATH))	# as the project name

PROJECTNAME=DEG


# Directories

# Project Root dir contains src, build ect.. Path from Makefile
PROJ_ROOT=../../

BUILD_DIR= $(PROJ_ROOT)build/
OBJ_DIR= $(BUILD_DIR)Intermediates/obj/
LST_DIR= $(BUILD_DIR)Intermediates/lst/
HEX_DIR= $(BUILD_DIR)Products/hex/
ELF_DIR= $(BUILD_DIR)Intermediates/elf/

# Automatic list all dirs containing source files (.asm .s .S .c .cc .cpp .C .h .hpp .H)

CORE_DIR= $(PROJ_ROOT)src/core/ # Final version (light core)
LIBS_DIR= $(PROJ_ROOT)src/config $(PROJ_ROOT)src/envelope $(PROJ_ROOT)src/clock $(PROJ_ROOT)src/output $(PROJ_ROOT)src/control

# List source directories
SRC_DIR=$(PROJ_ROOT)src/core/ \
	$(PROJ_ROOT)src/config/ \
	$(PROJ_ROOT)src/envelope/ \
	$(PROJ_ROOT)src/clock/ \
	$(PROJ_ROOT)src/output/ \
	$(PROJ_ROOT)src/control/


# Source files
# List C/C++/Assembly source files:
# (list all files to compile, e.g. 'a.c b.cpp as.S'):
# Use .cc, .cpp or .C suffix for C++ files, use .S 
# (NOT .s !!!) for assembly source code files.
SRC=	$(wildcard $(addsuffix *.c,$(SRC_DIR)))\
	$(wildcard $(addsuffix *.cpp,$(SRC_DIR)))
	
VPATH= $(SRC_DIR)


# additional includes (e.g. -I/path/to/mydir)

INC=	$(addprefix -I,$(SRC_DIR))


# libraries to link in (e.g. -lmylib)
LIBS=

# Optimization level, 
# use s (size opt), 1, 2, 3 or 0 (off)
OPTLEVEL=3


#####      AVR Dude 'upload' options       #####
#####  If you are using the avrdude program
#####  (http://www.bsdhome.com/avrdude/) to write
#####  to the MCU, you can set the following config
#####  options and use 'make upload' to program
#####  the device.


# programmer id--check the avrdude for complete list
# of available opts.  These should include stk500,
# avr910, avrisp, bsd, pony and more.  Set this to
# one of the valid "-c PROGRAMMER-ID" values 
# described in the avrdude info page.
# 
AVRDUDE_PROGRAMMERID=usbtiny

# port--serial or parallel port to which your 
# hardware programmer is attached
#
AVRDUDE_PORT=/dev/tty.usbserial-FTE2ZPXN
AVRDUDE_BAUD=38400

####################################################
#####                Config Done               #####
#####                                          #####
##### You shouldn't need to edit anything      #####
##### below to use the makefile but may wish   #####
##### to override a few of the flags           #####
##### nonetheless                              #####
#####                                          #####
####################################################


##### Flags ####

# HEXFORMAT -- format for .hex file output
HEXFORMAT=ihex

# compiler
CFLAGS= $(INC) -g -mmcu=$(MCU) -DF_CPU=$(F_CPU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall                \
	-Werror	#\
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

# c++ specific flags
CPPFLAGS=-fno-exceptions               \
	-ffunction-sections -fdata-sections #\
	-Wa,-ahlms=$(firstword         \
	$(filter %.lst, $(<:.cpp=.lst))\
	$(filter %.lst, $(<:.cc=.lst)) \
	$(filter %.lst, $(<:.C=.lst)))

# assembler
ASMFLAGS = $(INC) -mmcu=$(MCU)        \
	-x assembler-with-cpp            #\
	-Wa,-gstabs,-ahlms=$(firstword   \
		$(<:.S=.lst) $(<.s=.lst))


# linker
LDFLAGS=-Wl,-relax,--cref,-gc-sections,-Map,$(ELFTARGET).map -mmcu=$(MCU) \
	-lm $(LIBS)



##### executables ####
BINPATH=/usr/local/avr/bin/
CC=$(BINPATH)avr-gcc
LD=$(CC)#$(BINPATH)avr-ld
OBJCOPY=$(BINPATH)avr-objcopy
OBJDUMP=$(BINPATH)avr-objdump
OBJSTRIP=$(BINPATH)avr-strip
SIZE=$(BINPATH)avr-size
AVRDUDE=$(BINPATH)avrdude
REMOVE=rm -f

##### automatic target names ####
ELFTARGET=$(addprefix $(ELF_DIR),$(PROJECTNAME).elf)
DUMPTARGET=$(addprefix $(BUILD_DIR),$(PROJECTNAME).s)

HEXTARGET=$(addprefix $(HEX_DIR),$(PROJECTNAME).hex)
GDBINITFILE=gdbinit-$(PROJECTNAME)

# Define all object files.

# Start by splitting source files by type
#  C++
CPPFILES=$(filter %.cpp, $(SRC))
CCFILES=$(filter %.cc, $(SRC))
BIGCFILES=$(filter %.C, $(SRC))
#  C
CFILES=$(filter %.c, $(SRC))
#  Assembly
ASMFILES=$(filter %.S, $(SRC))


# List all object files we need to create
#OBJDEPS=$(CFILES:.c=.o)    \
	$(CPPFILES:.cpp=.o)\
	$(BIGCFILES:.C=.o) \
	$(CCFILES:.cc=.o)  \
	$(ASMFILES:.S=.o)

OBJDEPS=$(addprefix $(OBJ_DIR),$(notdir $(CFILES:.c=.o)))	\
	$(addprefix $(OBJ_DIR),$(notdir $(CPPFILES:.cpp=.o)))	\
	$(addprefix $(OBJ_DIR),$(notdir $(BIGCFILES:.C=.o)))	\
	$(addprefix $(OBJ_DIR),$(notdir $(CCFILES:.cc=.o)))	\
	$(addprefix $(OBJ_DIR),$(notdir $(ASMFILES:.S=.o)))


# Define all lst files.
LST=$(filter %.lst, $(OBJDEPS:.o=.lst))

# All the possible generated assembly 
# files (.s files)
GENASMFILES=$(filter %.s, $(OBJDEPS:.o=.s)) 


.SUFFIXES : .c .cc .cpp .C .o .elf .s .S \
	.hex .h .hh .hpp


.PHONY: upload clean stats gdbinit stats test

# Make targets:
# all, disasm, stats, hex, upload/install, clean
all: 	$(ELFTARGET)


disasm: $(DUMPTARGET) stats

stats: $(ELFTARGET)
	$(OBJDUMP) -h $(ELFTARGET)
	$(SIZE) $(ELFTARGET) 

hex: $(HEXTARGET)


upload: hex
	$(AVRDUDE) \
	-c $(AVRDUDE_PROGRAMMERID) \
	-e \
	-p $(PROGRAMMER_MCU) \
	-U flash:w:$(HEXTARGET)

install: upload

$(DUMPTARGET): $(ELFTARGET) 
	$(OBJDUMP) -S  $< > $@


$(ELFTARGET):	$(OBJDEPS)
	@echo 
	@echo Stripping...
	$(OBJSTRIP) $(OBJDEPS) --strip-debug --strip-unneeded
	@echo 
	@echo Linking...
	$(LD) $(LDFLAGS) -o $(ELFTARGET) $(OBJDEPS)
	$(OBJSTRIP) $(ELFTARGET) --strip-debug --strip-unneeded --strip-all

#### Generating assembly ####
# asm from C
%.s: %.c
	@echo Assembling $(^F)
	$(CC) -S $(CFLAGS) $< -o $@

# asm from (hand coded) asm
%.s: %.S
	@echo Assembling $(^F)
	$(CC) -S $(ASMFLAGS) $< > $@


# asm from C++
.cpp.s .cc.s .C.s :
	@echo Assembling $(^F)
	$(CC) -S $(CFLAGS) $(CPPFLAGS) $< -o $@



#### Generating object files ####
# object from C
$(OBJ_DIR)%.o : %.c
	@echo Building $(^F)
	$(CC) $(CFLAGS) -c $< -o $@

# object from C++ (.cc, .cpp, .C files)
# .cc.o .cpp.o .C.o :
$(OBJ_DIR)%.o : %.cpp
	@echo Building $(^F)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)%.o : %.cc
	@echo Building $(^F)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)%.o : %.C
	@echo Building $(^F)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@


# object from asm
.S.o :
	@echo Building $(^F)
	$(CC) $(ASMFLAGS) -c $< -o $@


#### Generating hex files ####
# hex files from elf
#####  Generating a gdb initialisation file    #####
# .elf.hex:
$(HEX_DIR)%.hex : $(ELF_DIR)%.elf
	$(OBJCOPY) -j .text -j .data -O $(HEXFORMAT) $< $@



#####  Generating a gdb initialisation file    #####
##### Use by launching simulavr and avr-gdb:   #####
#####   avr-gdb -x gdbinit-myproject           #####
gdbinit: $(GDBINITFILE)

$(GDBINITFILE): $(ELFTARGET)
	@echo "file $(ELFTARGET)" > $(GDBINITFILE)
	
	@echo "target remote localhost:1212" \
		                >> $(GDBINITFILE)
	
	@echo "load"        >> $(GDBINITFILE) 
	@echo "break main"  >> $(GDBINITFILE)
	@echo "continue"    >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"


#### Cleanup ####
clean:
	$(REMOVE) $(ELFTARGET) $(ELFTARGET).map $(DUMPTARGET)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST) $(GDBINITFILE)
	$(REMOVE) $(GENASMFILES)
	
	
cleanall: clean
	$(REMOVE) $(HEXTARGET)

test:
	@echo ++ Directories
	@echo Sources: $(SRC_DIR)
	@echo Build: $(BUILD_DIR)
	@echo Objects: $(OBJ_DIR)
	@echo Hex: $(HEX_DIR)
	@echo Elf: $(ELF_DIR)
	@echo ++ Sources
	@echo $(SRC)
	@echo ++ Includes
	@echo $(INC)
	@echo ++ Objects
	@echo $(OBJDEPS)
	@echo ++ Lists
	@echo $(LST)
	@echo --------------------------------------------------------------

#####                    EOF                   #####
