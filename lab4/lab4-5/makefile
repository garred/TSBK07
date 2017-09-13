# set this variable to the director in which you saved the common files
commondir = ../../common++/

all : lab

lab : lab.cxx $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -Wall -g -o lab -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES lab.cxx $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -Wno-write-strings -std=c++11

clean :
	rm lab
