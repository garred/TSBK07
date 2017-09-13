# set this variable to the director in which you saved the common files
OBJ = obj/
SRC = src/


COMMON_DIR = ../common++/
INCLUDES = -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -DGL_GLEXT_PROTOTYPES
COMMON_LIBRARIES = $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)loadobj.c $(COMMON_DIR)LoadTGA.c $(COMMON_DIR)Linux/MicroGlut.c
LIBRARIES_LINKED = -lXt -lX11 -lGL -lm
CC = g++ -Wall -g -pg -Wno-write-strings -std=c++11
OBJECT_FILES = $(OBJ)graphics.o $(OBJ)game.o $(OBJ)main.o $(OBJ)interface.o $(OBJ)entity.o $(OBJ)entities.o $(OBJ)random.o $(OBJ)helpers.o

all : main

$(OBJ)%.o: $(SRC)%.cpp $(SRC)%.hpp
	$(CC) -c -o $@ $< $(INCLUDES)

main : $(OBJECT_FILES)
	$(CC) -o $@ $(INCLUDES) $(COMMON_LIBRARIES) $(LIBRARIES_LINKED) $(OBJ)*.o

clean :
	rm $(OBJ)*.o
