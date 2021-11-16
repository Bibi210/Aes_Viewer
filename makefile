SHELL = /bin/sh
#definition des commandes utilisees
ifeq ($(shell uname -s), Darwin)
GL_LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
else
GL_LDFLAGS = -lGL -lglut -lGLU
endif

CXX = g++
CXXFLAGS := -std=c++17 -finline-functions -funroll-loops  -mtune=native -flto -O3
LDFLAGS = -lm $(GL_LDFLAGS)
EXE := Aes_img

all: $(EXE)

Aes_img: Lib/Src/aes.o

aes.o: Lib/aes.hpp


clean:
	$(RM) -r -f $(EXE) *~ $(shell find . -name "*.o") Aes_img.dSYM

.PHONY: all clean