# ---------------------------------------------------------------
# MSP430 project Makefile
#
#   include/   your .h files
#   src/       your .c files (and .s assembly files, if any)
#   build/     .o and .d files      (made automatically)
#   bin/       main.elf             (made automatically)
#
#   make         build bin/main.elf
#   make load    build, then load it onto the LaunchPad
#   make clean   delete build/ and bin/
# ---------------------------------------------------------------

# ---- Chip and tools ----
CPU = msp430g2553
CC  = msp430-elf-gcc
TI_INCLUDE = /opt/ti/msp430-gcc/include

# ---- Where the course's timer library lives ----
# Assumes this project is arch1-project2/project/ and the library is in
# arch1-project2/demos/. Change TIMER_DIR if your folder is somewhere else.
TIMER_DIR = ../demos
TIMER_LIB = $(TIMER_DIR)/lib/libTimer.a

# ---- Folders ----
SRC_DIR   = src
INC_DIR   = include
BUILD_DIR = build
BIN_DIR   = bin
TARGET    = $(BIN_DIR)/main.elf

# ---- Flags ----
#  -Os         optimize for size (the chip only has 16 KB of flash)
#  -MMD -MP    record which headers each .c uses, so editing a .h rebuilds the right files
CFLAGS   = -mmcu=$(CPU) -Os -g -Wall -I$(INC_DIR) -I$(TIMER_DIR)/h -I$(TI_INCLUDE)
DEPFLAGS = -MMD -MP
LDFLAGS = -L$(TIMER_DIR)/lib -L$(TI_INCLUDE)
LDLIBS  = -lTimer

# Which programmer mspdebug uses. Keeps your own MSPLOADER_OPTION if you set one.
MSPLOADER_OPTION ?= tilib

# ---- Find the source files ----
C_SRCS = $(wildcard $(SRC_DIR)/*.c)
S_SRCS = $(wildcard $(SRC_DIR)/*.s)
OBJS   = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS)) \
         $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(S_SRCS))
DEPS   = $(OBJS:.o=.d)

# ---- Rules ----
all: $(TARGET)

# Link every .o plus libTimer into bin/main.elf
$(TARGET): $(OBJS) $(TIMER_LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

# Compile src/foo.c -> build/foo.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Assemble src/foo.s -> build/foo.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(CC) -mmcu=$(CPU) -c $< -o $@

# Build and install libTimer if it isn't there yet
$(TIMER_LIB):
	$(MAKE) -C $(TIMER_DIR)/timerLib install

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

load: $(TARGET)
	mspdebug $(MSPLOADER_OPTION) "prog $<"

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all load clean

# Pull in the header dependencies (missing on the first build, which is fine)
-include $(DEPS)
