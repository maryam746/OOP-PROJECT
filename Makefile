RAYLIB_PATH ?= C:/raylib/raylib
CC = gcc
CXX = g++
CFLAGS = -g -std=c99 -Wall -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external
CXXFLAGS = -g -std=c++17 -Wall -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external
LDFLAGS = -L$(RAYLIB_PATH)/src -L$(RAYLIB_PATH)/src/external
LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

PROJECT_NAME ?= main
SRC_FILES := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, build/%.o, $(SRC_FILES))

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
    $(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

build/%.o: src/%.cpp
    mkdir -p build
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -rf build $(PROJECT_NAME)