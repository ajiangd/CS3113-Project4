SRCS = main.cpp

TARGET = raylib_app

UNAME_S := $(shell uname -s)

ifeq ($(wildcard CS3113/cs3113.cpp),CS3113/cs3113.cpp)
    SRCS += CS3113/cs3113.cpp
endif

ifeq ($(wildcard CS3113/Entity.cpp),CS3113/Entity.cpp)
    SRCS += CS3113/Entity.cpp
endif

ifeq ($(wildcard CS3113/Map.cpp),CS3113/Map.cpp)
    SRCS += CS3113/Map.cpp
endif

ifeq ($(wildcard CS3113/Scene.cpp),CS3113/Scene.cpp)
    SRCS += CS3113/Scene.cpp
endif

ifeq ($(wildcard CS3113/MenuScene.cpp),CS3113/MenuScene.cpp)
    SRCS += CS3113/MenuScene.cpp
endif

ifeq ($(wildcard CS3113/LevelA.cpp),CS3113/LevelA.cpp)
    SRCS += CS3113/LevelA.cpp
endif

ifeq ($(wildcard CS3113/LevelB.cpp),CS3113/LevelB.cpp)
    SRCS += CS3113/LevelB.cpp
endif

ifeq ($(wildcard CS3113/LevelC.cpp),CS3113/LevelC.cpp)
    SRCS += CS3113/LevelC.cpp
endif

ifeq ($(wildcard CS3113/WinScene.cpp),CS3113/WinScene.cpp)
    SRCS += CS3113/WinScene.cpp
endif

ifeq ($(wildcard CS3113/LoseScene.cpp),CS3113/LoseScene.cpp)
    SRCS += CS3113/LoseScene.cpp
endif

CXX = g++
CXXFLAGS = -std=c++11

RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib)
RAYLIB_LIBS = $(shell pkg-config --libs raylib)

ifeq ($(UNAME_S), Darwin)
    CXXFLAGS += -arch arm64 $(RAYLIB_CFLAGS)
    LIBS = $(RAYLIB_LIBS) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    EXEC = ./$(TARGET)
else ifneq (,$(findstring MINGW,$(UNAME_S)))
    CXXFLAGS += -IC:/raylib/include
    LIBS = -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET := $(TARGET).exe
    EXEC = ./$(TARGET)
else
    CXXFLAGS +=
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    EXEC = ./$(TARGET)
endif

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	@if [ -f "$(TARGET)" ]; then rm -f $(TARGET); fi
	@if [ -f "$(TARGET).exe" ]; then rm -f $(TARGET).exe; fi

run: $(TARGET)
	$(EXEC)
