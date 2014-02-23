CC = c++

#############################################################################
# compiling #################################################################
# GLUT_HOME = /usr/opt/glut/build/glut-3.6
# CFLAGS = -I${GLUT_HOME}/include
# LIBS = -lglut -lGL -lGLU -lX11 -lXmu -lXi
# LIBDIR = -L${GLUT_HOME}/lib/glut -L/usr/X11R6/lib -L/usr/lib 

#For MacOS
LIBS = -framework OpenGL -framework GLUT
LIBDIR = 
CFLAGS = 


.cpp.o:
	${CC} ${CFLAGS} -c $< -o $@

all: robot

robot.o : robot.cpp
	${CC} ${CFLAGS} -c $? -o $@
point.o : point.cpp
	${CC} ${CFLAGS} -c $? -o $@
vector.o : vector.cpp
	${CC} ${CFLAGS} -c $? -o $@

objects = robot.o point.o vector.o

robot: 	$(objects)
	$(CC) ${LIBDIR} ${LIBS} $(objects) $(LIBS) -o $@

clean:
	\rm -f *.o robot

