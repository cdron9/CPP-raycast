# Makefile for CPPRaycaster (SDL3)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Auto-detect Homebrew prefix
BREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /usr/local)
SDL3_LIB := $(BREW_PREFIX)/lib
SDL3_INCLUDE := $(BREW_PREFIX)/include

LIBS = -lSDL3 -lm
RPATH = -Wl,-rpath,$(SDL3_LIB)

SRC = main.cpp
OUT = raycast

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(SDL3_INCLUDE) $(SRC) -o $(OUT) $(LIBS) $(RPATH)

clean:
	rm -f $(OUT)

