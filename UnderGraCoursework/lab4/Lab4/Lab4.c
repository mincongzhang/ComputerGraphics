#include <stdlib.h>
#include <GL/glut.h>

#define TORSO_RADIUS 1.7
#define TORSO_LENGTH 2.5
#define LEG_RADIUS  0.7
#define LEG_LENGTH 2.5
#define HEAD_RADIUS  1.7 
#define RING_OUTSIDE_RADIUS  0.7 
#define RING_INSIDE_RADIUS  0.07 
#define HORN_LENGTH  1.3 
#define HORN_RADIUS  0.25
#define TAIL_RADIUS  0.10
#define TAIL_LENGTH  1.25
#define EYE_RADIUS  0.10
#define EYE_GAP  0.40


GLUquadricObj *p; // pointer to quadric object


void tail()
{
   glRotatef (45.0, 1.0, 0.0, 0.0);   
   glTranslatef(0.0,0.3,-TAIL_LENGTH-0.1);
   glPushMatrix();
   glColor3ub (116, 60, 127);
   glScalef(TAIL_RADIUS,TAIL_RADIUS,TAIL_LENGTH);
   gluCylinder (p, 1, 1, 1, 20, 20);
   glPopMatrix();
   
   //ball in the tail      
   glPushMatrix();
    glColor3ub (119, 95, 159);
   glScalef(2.5*TAIL_RADIUS,2.5*TAIL_RADIUS,2.5*TAIL_RADIUS);
   glutSolidSphere(1,10,10);
   glPopMatrix();

}

void torso()
{
   glPushMatrix();
   glColor3ub (119, 95, 159);
   glScalef(TORSO_RADIUS+0.2,TORSO_RADIUS, TORSO_LENGTH);
   glutSolidSphere(1,10,10);
   glPopMatrix();
}

void leg()
{
  //leg
  glPushMatrix();
  glColor3ub (116, 60, 127);
  glTranslatef(0.0,-LEG_LENGTH,0.0);
  glScalef(LEG_RADIUS, LEG_LENGTH, LEG_RADIUS);
  glRotatef (-90.0, 1.0, 0.0, 0.0);
  gluCylinder (p, 1, 1, 1, 20, 20);
  glPopMatrix();
  
  //feet
  glPushMatrix();
   glColor3ub (119, 95, 159);
  glTranslatef(0.0,-LEG_LENGTH,0.0);
  glScalef(LEG_RADIUS, LEG_RADIUS, LEG_RADIUS);
  glutSolidSphere(1,10,10);
  glPopMatrix();
}

void head()
{
  
  glPushMatrix();
  glColor3ub (116, 60, 127);
  glTranslatef(0.0,-0.3,1.3*HEAD_RADIUS);
  glScalef(HEAD_RADIUS-0.1, HEAD_RADIUS, HEAD_RADIUS-0.2);
  glutSolidSphere(1,20,20);
  glPopMatrix();
  
  //right eye
  glPushMatrix();
  glColor3ub (0.0, 0.0, 0.0);
  glTranslatef(-EYE_GAP,0.2*HEAD_RADIUS,2.1*HEAD_RADIUS);
  glScalef(EYE_RADIUS, EYE_RADIUS, EYE_RADIUS);
  glutSolidSphere(1,10,10);
  glPopMatrix();
  
  //left eye
  glPushMatrix();
  glColor3ub (0.0, 0.0, 0.0);
  glTranslatef(EYE_GAP,0.2*HEAD_RADIUS,2.1*HEAD_RADIUS);
  glScalef(EYE_RADIUS, EYE_RADIUS, EYE_RADIUS);
  glutSolidSphere(1,10,10);
  glPopMatrix();
  
  //ring in the nose
  glPushMatrix();
  glColor3ub (255, 236, 148);
  glTranslatef(0.0,-0.8*HEAD_RADIUS,2.2*HEAD_RADIUS);
  glRotatef (-20.0, 1.0, 0.0, 0.0);
  glutSolidTorus(RING_INSIDE_RADIUS,RING_OUTSIDE_RADIUS,40, 40);
  glPopMatrix();
  
  
  //horn
  glPushMatrix();
  glColor3ub (255, 236, 148);
  glTranslatef(-0.5*HORN_LENGTH,0.7*HEAD_RADIUS,1.7*HEAD_RADIUS);
  glRotatef (90.0, 0.0, 1.0, 0.0);
  glScalef(HORN_RADIUS, HORN_RADIUS, HORN_LENGTH);
  gluCylinder (p, 1, 1, 1, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
  glColor3ub (255, 236, 148);
  glTranslatef(0.5*HORN_LENGTH,0.7*HEAD_RADIUS,1.7*HEAD_RADIUS);
  glRotatef (50.0, 0.0, 1.0, 0.0);
  glutSolidCone(HORN_RADIUS, 1*HORN_LENGTH, 20,20);
  glPopMatrix();
  
  glPushMatrix();
  glColor3ub (255, 236, 148);
  glTranslatef(-0.5*HORN_LENGTH,0.7*HEAD_RADIUS,1.7*HEAD_RADIUS);
  glRotatef (-50.0, 0.0, 1.0, 0.0);
  glutSolidCone(HORN_RADIUS, 1*HORN_LENGTH, 20,20);
  glPopMatrix();
  
}


void object()
{
  glColor3f (0.0, 0.0, 1.0);
  torso();
  
  glPushMatrix();
  glTranslatef(0.0, 0.0, -TORSO_LENGTH);//the porsition of the tail 
  tail(); 
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.5*TORSO_RADIUS, 0.0, 0.5*TORSO_LENGTH);//the porsition of leg1 
  glRotatef(10, 0.0, 0.0, 1.0);//slope 10 degree 
  leg();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-0.5*TORSO_RADIUS, 0.0, 0.5*TORSO_LENGTH);//the porsition of leg2
  glRotatef(-10, 0.0, 0.0, 1.0);//slope 10 degree
  leg();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-0.5*TORSO_RADIUS, 0.0, -0.5*TORSO_LENGTH);//the porsition of leg3
  glRotatef(-10, 0.0, 0.0, 1.0);//slope 10 degree
  glColor3f (0.0, 1.0, 0.0);
  leg();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.5*TORSO_RADIUS, 0.0, -0.5*TORSO_LENGTH);//the porsition of leg4 
  glRotatef(10, 0.0, 0.0, 1.0);//slope 10 degree
  glColor3f (0.0, 1.0, 0.0);
  leg();
  glPopMatrix();
  
       
  glPushMatrix();
  glRotatef (-20.0, 1.0, 0.0, 0.0);//rise the head 
  glTranslatef(0.0, -0.7*HEAD_RADIUS, 0.5*TORSO_LENGTH);//the porsition of the head
  head();
  glPopMatrix();
}



void display()
{  
  gluLookAt(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  object();
  glutSwapBuffers();
}


void init()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  p = gluNewQuadric();
 }

void reshape(int w, int h)
{
  glViewport (0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();  
  if (w <= h){
  glOrtho(-7.0, 7.0, -7.0* (GLfloat) h / (GLfloat) w, 7.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
  }
  else {
  glOrtho(-7.0*(GLfloat) w / (GLfloat) h, 7.0*(GLfloat) w / (GLfloat) h, -7.0, 7.0, -10.0, 10.0);
  }
  glutPostRedisplay();
}

void idle()
{
  glutPostRedisplay();
}

int main(int argc, char **argv)
{

  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("cow");
  glutIdleFunc(idle);
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glEnable(GL_DEPTH_TEST); /* Enable hidden-surface removal */
  init();
  glutMainLoop();
}


