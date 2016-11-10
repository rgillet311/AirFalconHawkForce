#Requirements
#libsdl2-image-dev
#libsdl2-mixer-dev
#libsdl2-ttf-dev
#libsdl2-net-dev
#libsdl2-gfx-dev
#libsdl2-dev

#OBJS specifies which files to compile as part of the project
#OBJS = $(shell find *.cpp)
OBJS = sdlTest.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
APPNAME = run

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(APPNAME)

clean:
	rm -rf run