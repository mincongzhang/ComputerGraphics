
/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* Modified by Jan Kautz to match the Coursework */

/* MACOS compile: g++ cube.c -framework OpenGL -framework glut */

#include <stdlib.h>
#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <math.h>
#include <GL/glaux.h>

#define TORSO_LENGTH 1.0
#define TORSO_WIDTH 0.7
#define TORSO_THICKNESS 0.3

#define HEAD_RADIUS  0.25

#define LIMB_DIAMETER 0.2
#define LIMB_HEIGHT 0.35
#define JOINT_BALL_RADIUS LIMB_DIAMETER/2+0.02

#define UPPER_ARM_WIDTH  0.7

//control the colour change 
int c_flag = true;
//control the arm change 
int arm_flag = true;
//control the leg change 
int leg_flag = true;
//control walking 
int walk_flag = true;
//control waving 
int wave_flag = true;

//stop and move limbs back to original position
int stop_walking_flag = true;

int moving, beginx, beginy;
GLfloat anglex = 0;   /* in degrees */
GLfloat angley = 0;   /* in degrees */
GLfloat arm_angle = 0;   /* in degrees */
GLfloat lower_right_arm_angle = 0;   /* in degrees */
GLfloat lower_left_arm_angle = 0;   /* in degrees */
GLfloat leg_angle = 0;   /* in degrees */
GLfloat lower_right_leg_angle = 0;   /* in degrees */
GLfloat lower_left_leg_angle = 0;   /* in degrees */
GLfloat waving_angle = 0;   /* in degrees */
GLfloat walk_distance = 0;   

GLfloat light_diffuse[] = {0.1, 0.1, 0.1, 1.0};  /* Red diffuse light. */
GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};  /* Grey ambient light. */
GLfloat light_position[] = {1.0, 0.5, 0.0, 0.0};  /* Infinite light location. */

//eyes colour
GLfloat eye_diffuse[] = {0.0, 0.0, 0.0, 1.0}; 
GLfloat eye_ambient[] = {0.3, 0.3, 0.3, 1.0}; 
GLfloat eye_specular[] = {0.0, 0.0, 0.0, 1.0}; 
const GLfloat eye_shininess = 1.0;

//texture
GLuint g_texture = 0;

//GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
//  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
//  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

// shading
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat emission[4];
  GLfloat shininess;
} materialStruct;

materialStruct MetalShinyMaterials = {
   {0.749, 0.678, 0.435, 1.0},
   {0.1, 0.1, 0.0, 1.0},
   {0.1, 0.1, 0.0, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   100.0
};

materialStruct GreySurfaceMaterials = {
   {0.5, 0.5, 0.5, 1.0},
   {0.5, 0.5, 0.5, 1.0},
   {0.1, 0.1, 0.1, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   0.0
};

materialStruct WhiteSurfaceMaterials = {
   {0.5, 0.5, 0.5, 1.0},
   {0.5, 0.5, 0.5, 1.0},
   {0.5, 0.5, 0.5, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   0.0
};

materialStruct *currentMaterials;

void EyeSurface(){
  glMaterialfv(GL_FRONT, GL_AMBIENT, eye_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,eye_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, eye_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, &eye_shininess);
}

void MetalShiny(){
  currentMaterials = &MetalShinyMaterials;
  glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);
}

void GreySurface(){
  currentMaterials = &GreySurfaceMaterials;
  glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);
}

void WhiteSurface(){
  currentMaterials = &WhiteSurfaceMaterials;
  glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);
}


//insert a image
int LoadGLTextures(GLuint& unTexture, const char* chFileName)                
{
    AUX_RGBImageRec *TextureImage;                    //get the pointer to the image
    TextureImage = auxDIBImageLoad("texture.bmp"); //get the image

    glGenTextures(1, &unTexture);                    // generate a texture,unTexture

    glBindTexture(GL_TEXTURE_2D, unTexture);        //bind and get the data from the image
    
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);   

    if (TextureImage)                //Free the data
    {
        if (TextureImage->data)
        {
            free(TextureImage->data);
        }
        free(TextureImage);
    }

    return 1;
}


void draw_texture()
{
	glBindTexture(GL_TEXTURE_2D, g_texture);        //use texture

    glPushMatrix();  
	glBegin(GL_QUADS); 

    // draw frontside
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.501f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.501f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f( 0.5f,  0.5f,  0.501f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-0.5f,  0.5f,  0.501f);

	// draw backside
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.501f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-0.5f,  0.5f, -0.501f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f( 0.5f,  0.5f, -0.501f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.501f);

    glEnd();

    glPopMatrix(); 
}


/*Draw the torso of the robot */
void torso(void)
{
	glPushMatrix();
   
    glScalef(TORSO_WIDTH, TORSO_LENGTH, TORSO_THICKNESS);
	draw_texture();
	GreySurface();
	glutSolidCube(1);
	glPopMatrix();
}

/*Draw the head of the robot */
void head(void)
{
	//head
	glPushMatrix();
	WhiteSurface();
	glScalef(HEAD_RADIUS, HEAD_RADIUS, HEAD_RADIUS);
	glutSolidSphere(1,20,10);
	glPopMatrix();

	//right eye
	glPushMatrix();
	EyeSurface();
	glTranslatef(0.11,0.0,HEAD_RADIUS-0.02);
	glScalef(0.03, 0.03, 0.01);
	glutSolidSphere(1,20,10);
	glPopMatrix();

	//left eye
	glPushMatrix();
	EyeSurface();
	glTranslatef(-0.11,0.0,HEAD_RADIUS-0.02);
	glScalef(0.03, 0.03, 0.01);
	glutSolidSphere(1,20,10);
	glPopMatrix();

}

/*Draw a basic limb(arm or leg) of the robot */
void limb(void)
{
	//joint ball
	glPushMatrix();
	MetalShiny();
	glScalef(JOINT_BALL_RADIUS, JOINT_BALL_RADIUS, JOINT_BALL_RADIUS);
	glutSolidSphere(1,20,10);
	glPopMatrix();

	glPushMatrix();
	GreySurface();
	glTranslatef(0.0,-LIMB_HEIGHT/2-LIMB_DIAMETER/2,0.0);
	glScalef(LIMB_DIAMETER, LIMB_HEIGHT, LIMB_DIAMETER);
	glutSolidCube(1);
	glPopMatrix();
}

/*Draw a lower_leg with foot */
void lower_leg(void)
{

	glPushMatrix();
	//foot
	GreySurface();
	glTranslatef(0.0,-LIMB_HEIGHT-2*JOINT_BALL_RADIUS-JOINT_BALL_RADIUS,0.6*JOINT_BALL_RADIUS);
	glScalef(LIMB_DIAMETER, JOINT_BALL_RADIUS-0.03, 3*JOINT_BALL_RADIUS);
	glutSolidCube(1);
	glPopMatrix();

	//foot joint ball
	glPushMatrix();
	MetalShiny();
	glTranslatef(0.0,-LIMB_HEIGHT-LIMB_DIAMETER,0.0);
	glScalef(JOINT_BALL_RADIUS, JOINT_BALL_RADIUS, JOINT_BALL_RADIUS);
	glutSolidSphere(1,20,10);
	glPopMatrix();

	//basic limb of lower leg
	limb();
	
}

/*Draw a lower_arm with hand */
void lower_arm(void)
{
	//hand
	glPushMatrix();
	WhiteSurface();
	glTranslatef(0.0,-LIMB_HEIGHT-LIMB_DIAMETER+0.03,0.0);
	glScalef(JOINT_BALL_RADIUS-0.07, JOINT_BALL_RADIUS-0.05, JOINT_BALL_RADIUS-0.05);
	glutSolidDodecahedron();
	glPopMatrix();

	//basic limb of lower arm
	limb();
}

/*Draw the whole body of the robot */
void robot(void)
{
	//add torso
	torso();

	//add and move head to the correct position
	glPushMatrix();
	glTranslatef(0.0,TORSO_LENGTH/2+HEAD_RADIUS,0.0);
	head();
	glPopMatrix();

	/*draw the right leg */
	//add and move right leg to the correct position
	glPushMatrix();
	glTranslatef(TORSO_WIDTH/2-0.17,-TORSO_LENGTH/2-JOINT_BALL_RADIUS,0.0);
	glRotatef(-leg_angle, 1.0, 0.0, 0.0);
		/*draw the whole leg */
		//use a basic limb as upper leg
		limb();

		//move the lower leg to the right position
		glPushMatrix();
		glTranslatef(0.0,-LIMB_HEIGHT-LIMB_DIAMETER,0.0);
		glRotatef(-lower_right_leg_angle, 1.0, 0.0, 0.0);
		lower_leg();
		glPopMatrix();
		/*end of drawing the whole leg */
	glPopMatrix();

	/*draw the left leg */
	//add and move left leg to the correct position
	glPushMatrix();
	glTranslatef(-(TORSO_WIDTH/2-0.17),-TORSO_LENGTH/2-JOINT_BALL_RADIUS,0.0);
	glRotatef(leg_angle, 1.0, 0.0, 0.0);
		/*draw the whole leg */
		//use a basic limb as upper leg
		limb();

		//move the lower leg to the right position
		glPushMatrix();
		glTranslatef(0.0,-LIMB_HEIGHT-LIMB_DIAMETER,0.0);
		glRotatef(-lower_left_leg_angle, 1.0, 0.0, 0.0);
		lower_leg();
		glPopMatrix();
		/*end of drawing the whole leg */
	glPopMatrix();

	/*draw the right arm */
	//add and move right arm to the correct position
	glPushMatrix();
	glTranslatef(TORSO_WIDTH/2+JOINT_BALL_RADIUS,TORSO_LENGTH/2-JOINT_BALL_RADIUS,0.0);
	glRotatef(waving_angle, 0.0, 0.0, 1.0);
	glRotatef(arm_angle, 1.0, 0.0, 0.0);
	    /*draw the whole arm */
		//use a basic limb as upper arm
		limb();

		//move the lower arm to the right position
		glPushMatrix();
		glTranslatef(0.0,-LIMB_HEIGHT-LIMB_DIAMETER,0.0);
		glRotatef(lower_left_arm_angle, 1.0, 0.0, 0.0);
		lower_arm();
		glPopMatrix();
		/*end of drawing the whole arm */
	glPopMatrix();

	/*draw the left arm */
	//add and move left arm to the correct position
	glPushMatrix();
	glTranslatef(-(TORSO_WIDTH/2+JOINT_BALL_RADIUS),TORSO_LENGTH/2-JOINT_BALL_RADIUS,0.0);
	glRotatef(-arm_angle, 1.0, 0.0, 0.0);
		/*draw the whole arm */
		//use a basic limb as upper arm
		limb();

		//move the lower arm to the right position
		glPushMatrix();
		glTranslatef(0.0,-LIMB_HEIGHT-LIMB_DIAMETER,0.0);
		glRotatef(lower_right_arm_angle, 1.0, 0.0, 0.0);
		lower_arm();
		glPopMatrix();
		/*end of drawing the whole arm */
	glPopMatrix();

}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Adjust cube position according to user input */
  /* Note, this is very basic, and suffers from Gimball lock */
  glLoadIdentity();

  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  glTranslatef(0.0, 0.0, -1.0);
  glRotatef(angley, 1.0, 0.0, 0.0);
  glRotatef(-anglex, 0.0, 1.0, 0.0);

  /* Coursework: replace simple box with (animated) robot! */
  //drawBox();
  //draw robot
  	glPushMatrix();
	glTranslatef(0.0,0.0,walk_distance);
	robot();
	glPopMatrix();

  glutSwapBuffers();
}

void init(void)
{
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -0.5;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 0.5;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -0.5;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 0.5;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 0.5;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -0.5;

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /*enable texture*/
  glCullFace(GL_BACK);                        
  glEnable(GL_CULL_FACE);                       
  glEnable(GL_TEXTURE_2D);
  LoadGLTextures(g_texture, "texture.bmp");          

}

void animate(void)
{
  
    //control colour smoothly changing  
   if (c_flag) {
		eye_ambient[1] += 0.001f;
		if( eye_ambient[1] > 0.999f ) c_flag = !c_flag;
    }
   else {
		eye_ambient[1] -= 0.001f;
		if( eye_ambient[1] < 0.001f ) c_flag = !c_flag;
    }

   /*Walk*/
   if(!stop_walking_flag){
		//move waving limb back to original position
		if(waving_angle>0) waving_angle -= 0.12;
		if(waving_angle<0) waving_angle += 0.12;
		
			//control arm moving 
			if (arm_flag) {
				arm_angle += 0.05;
				if( arm_angle < 0 ) lower_left_arm_angle = arm_angle;
				if( arm_angle > 0 ) lower_right_arm_angle = -arm_angle;
				if( arm_angle > 30 ) arm_flag = !arm_flag;
			}
			else {
				arm_angle -= 0.05;
				if( arm_angle < 0 ) lower_left_arm_angle = arm_angle;
				if( arm_angle > 0 ) lower_right_arm_angle = -arm_angle;
				if( arm_angle < -30 ) arm_flag = !arm_flag;
			}

			//control leg moving 
			if (leg_flag) {
				leg_angle += 0.05;
				lower_left_leg_angle = -abs(leg_angle)*2/3;
				lower_right_leg_angle = -abs(leg_angle)*2/3;
				if( leg_angle > 30 ) leg_flag = !leg_flag;
			}
			else {
				leg_angle -= 0.05;
				lower_left_leg_angle = -abs(leg_angle)*2/3;
				lower_right_leg_angle = -abs(leg_angle)*2/3;
				if( leg_angle < -30 ) leg_flag = !leg_flag;
			}

			//control walking towards
			if (walk_flag) {
				walk_distance += 0.001;
				if( walk_distance > 1.5 ) walk_flag = !walk_flag;
			}
			else {
				walk_distance -= 0.001;
				if( walk_distance < -1.5 ) walk_flag = !walk_flag;
			}
		
	}
   else{
	   //move all limbs back to original positions
	   if(arm_angle>0) arm_angle -= 0.05;
	   if(arm_angle<0) arm_angle += 0.05;
	   if(leg_angle>0) leg_angle -= 0.05;
	   if(leg_angle<0) leg_angle += 0.05;
	   if(lower_left_arm_angle>0) lower_left_arm_angle -= 0.05;
	   if(lower_left_arm_angle<0) lower_left_arm_angle += 0.05;
	   if(lower_right_arm_angle>0) lower_right_arm_angle -= 0.05;
	   if(lower_right_arm_angle<0) lower_right_arm_angle += 0.05;
	   if(lower_left_leg_angle>0) lower_left_leg_angle -= 0.05;
	   if(lower_left_leg_angle<0) lower_left_leg_angle += 0.05;
	   if(lower_right_leg_angle>0) lower_right_leg_angle -= 0.05;
	   if(lower_right_leg_angle<0) lower_right_leg_angle += 0.05;

			//control waving
			if (wave_flag) {
				waving_angle += 0.05;
				if( waving_angle > 180 ) wave_flag = !wave_flag;
			}
			else {
				waving_angle -= 0.05;
				if( waving_angle < 150 ) wave_flag = !wave_flag;
			}			

   }

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  /* Coursework: you can remove the color animation and instead
                 use it to animate your robot */

  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    moving = 1;

    beginx = x;
    beginy = y;
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    moving = 0;
  }
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
  stop_walking_flag = !stop_walking_flag;
  }
}

void motion(int x, int y)
{
  if (moving) {
    anglex = anglex + (x - beginx);
    angley = angley + (y - beginy);
    beginx = x;
    beginy = y;
    glutPostRedisplay();
  }
}



int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(640, 640);
  glutCreateWindow("Robot");
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutIdleFunc(animate);

  init();

  glutMainLoop();
  return 0;
}
