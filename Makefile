#OBJS specifies which files to compile as part of the project
OBJS = main.cpp

OBJSLINK = Chip8CPU.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

DBG_FLAGS = -g

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = chip8

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(DBG_FLAGS) $(OBJS) $(OBJSLINK) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)