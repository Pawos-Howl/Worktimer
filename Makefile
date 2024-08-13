CC_PATH       ?= /usr/bin
CC             = $(CC_PATH)/g++

BUILD_DIR     ?= build
BIN_DIR       ?= bin

# LIBRARIES      = -lSDL2 -lSDL2_image -lSDL2_ttf
LIBRARIES	= -lSDL3 -lSDL3_ttf
INCLUDES	= -Iinclude -Ilib

# select one of these for Debug vs. Release
CC_DBG         = -g -ggdb
#CC_DBG         =

CCWARN        = -Wall -Wextra -Wpedantic

#CCOPTIM       = -O2
CCOPTIM       = -O3 -ffast-math

CCFLAGS       = $(CC_DBG) $(INCLUDES) -std=c++11 -march=native $(CCOPTIM) $(CCWARN)
LDFLAGS       = $(CC_DBG) $(LIBRARIES)


# MAIN EXECUTABLE

EXECUTABLE = worktimer
SOURCES = driver.cpp $(shell find src -name "*.c*")
OBJECTS_C = $(SOURCES:.c=.o)
OBJECTS_C_CPP = $(OBJECTS_C:.cpp=.o)
OBJECTS = $(addprefix ${BUILD_DIR}/,$(OBJECTS_C_CPP))


## TEST 01 -- tests/test01-interpolation.cpp
#
#T01_EXECUTABLE = test01-interpolation
#T01_SOURCES = tests/test01-interpolation.cpp src/animations/animation.cpp src/animations/commonanimations.cpp src/animations/commoninterpolators.cpp
#T01_OBJECTS_C = $(T01_SOURCES:.c=.o)
#T01_OBJECTS_C_CPP = $(T01_OBJECTS_C:.cpp=.o)
#T01_OBJECTS = $(addprefix ${BUILD_DIR}/,$(T01_OBJECTS_C_CPP))


.PHONY: main
.DEFAULT: main

all: main tests

#tests: $(T01_EXECUTABLE)
tests: 

main: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$(EXECUTABLE) $(OBJECTS)

#$(T01_EXECUTABLE): $(T01_OBJECTS)
#	$(CC) $(LDFLAGS) -o $(BUILD_DIR)/$(T01_EXECUTABLE) $(T01_OBJECTS)


$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*
	rm $(BIN_DIR)/$(EXECUTABLE)
