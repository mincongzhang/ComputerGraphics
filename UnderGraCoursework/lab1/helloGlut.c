#include <GL/glut.h>

void display_func()
{
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutCreateWindow("Hello GLUT!");
  glutDisplayFunc(display_func);
  glutMainLoop();  
}
