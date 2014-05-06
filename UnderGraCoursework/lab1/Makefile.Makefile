CC = gcc
GL_LIB_DIR = -L/usr/lib
GL_LIBS = -lglut32 -lglu32 -lopengl32
X11_LIBS =  -lX11 -lXmu -lXt -lSM -lICE -lXext -lXi
X11_LIB_DIR = -L/usr/X11R6/lib
OTHER_LIBS = -lm
CFLAGS = -O2 -g -mno-cygwin

APP = helloGlut
SRC = $(APP).c
OBJ = $(APP).o

all: $(APP)

$(APP): $(OBJ)
	$(CC) $(CFLAGS) $(X11_LIB_DIR) $(X11_LIBS) $(OTHER_LIBS) $(GL_LIB_DIR) $(LDFLAGS) $(OBJ) $(GL_LIBS) -o $(APP) 

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(GL_INC_DIR) -c $(SRC)

clean:
	rm -f $(OBJ)
	rm -f *~*
	rm -f $(APP)

