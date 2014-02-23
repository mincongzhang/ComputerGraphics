#include "point.h"

#include <stdio.h>
#include <iostream>

#include <stdlib.h>
#include <math.h>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//NEW
#include "vector.h"

using namespace std;

#define SPACE 32

int numberOfPoints;

int numberOfTestCases = 8;
int testcase = 0;

//height and width of screen
int height=400, width=400;

//NEW
//Boundarys of desktop
//int numberOfBoundaryPoints = 4;
//int numberOfBoundaryPoints = 5;
int numberOfBoundaryPoints = 6;
static float boundary_point[6][3] = {
	//{-150.0,-150.0,0.0},{150.0,-150.0,0.0},{150.0,150.0,0.0},{-150.0,150.0,0.0}
	//{-90.0,-90.0,0.0},{90.0,-90.0,0.0},{120.0,90.0,0.0},{0.0,160.0,0.0},{-120.0,90.0,0.0}
	{-120.0,-80.0,0.0},{0.0,-130.0,0.0},{120.0,-80.0,0.0},{120.0,80.0,0.0},{0.0,130.0,0.0},{-120.0,80.0,0.0}
};

//points of polygon
static float unclipped0[4][3] = {
	{-100.0, -100.0, 0.0}, {100.0, -100.0, 0.0},
	{100.0, 100.0, 0.0}, {-100.0, 100.0, 0.0}
};

static float unclipped1[5][3] = {
	{-100.0, -100.0, 0.0}, {-100.0, 100.0, 0.0},
	{100.0, 100.0, 0.0}, {180.0, 0.0, 0.0},
	{100.0, -100.0, 0.0}
};

static float unclipped2[4][3] = {
	{-100.0, -100.0, 0.0}, {-100.0, 100.0, 0.0},
	{180.0, 180.0, 0.0}, {100.0, -100.0, 0.0}
};

static float unclipped3[4][3] = {
	{-180.0, -100.0, 0.0}, {180.0, 100.0, 0.0},
	{180.0, -100.0, 0.0}, {-180.0, 100.0, 0.0}
};

static float unclipped4[8][3] = {
	{-100.0, -130.0, 0.0}, {-180.0, -100.0, 0.0},
	{-100.0, -70.0, 0.0}, {-100.0, 70.0, 0.0},
	{-180.0, 100.0, 0.0}, {-100.0, 130.0, 0.0},
	{0.0, 130.0, 0.0}, {0.0, -130.0, 0.0}
};

static float unclipped5[8][3] = {
	{-180.0, -130.0, 0.0}, {-100.0, -100.0, 0.0},
	{-180.0, -70.0, 0.0}, {-180.0, 70.0, 0.0},
	{-100.0, 100.0, 0.0}, {-180.0, 130.0, 0.0},
	{-190.0, 130.0, 0.0}, {-190.0, -130.0, 0.0}
};

static float unclipped6[15][3] = {
	{-110.0, 130.0, 0.0}, {-190.0, 100.0, 0.0},
	{-110.0, 70.0, 0.0}, {-90.0, -80.0, 0.0},
	{-160.0, -120.0, 0.0}, {-70.0, -150.0, 0.0},
	{0.0, 0.0, 0.0}, {140.0, -180.0, 0.0},
	{200.0, -100.0, 0.0}, {20.0, 170.0, 0.0},
	{80.0, 170.0, 0.0}, {120.0, 120.0, 0.0},
	{140.0, 120.0, 0.0}, {100.0, 180.0, 0.0},
	{-50.0, 180.0, 0.0}
};

static float unclipped7[9][3] = {
	{0.0, 0.0, 0.0},{-30.0, -150.0, 0.0}, {150.0, -160.0, 0.0}, 
	{160.0, -10.0, 0.0}, {150.0, 70.0, 0.0}, 
	{50.0, 20.0, 0.0}, {100.0, 10.0, 0.0}, 
	{100.0, -120.0, 0.0}, {10.0, -130.0, 0.0}, 
};


//NEW
Point intersect (Point PA, Point PB, Point P0, Point P1)
{
	Point P;
    
	float A = PB.y() - PA.y();
	float B = -(PB.x() - PA.x());
	float C = P1.y() - P0.y();
	float D = -(P1.x() - P0.x());
    float E = A*PA.x() + B*PA.y();
	float F = C*P0.x() + D*P0.y();
	//calculate determinant
	P.x() = (E*D - B*F) / (A*D - B*C);
	P.y() = (A*F - E*C) / (A*D - B*C);
	P.z() = 0.0;
	
	return P;
}


vector<Point>* clip(vector<Point>* unclipped) {

	vector<Point>* clipped = new vector<Point>();
	// places all points in the clipped vector
	// TO DO: Change this, so that all the parts of the polygon that are outside the desktop area
	// are not drawn


	//NEW
	//initial a temporal unclipped vector
	vector<Point>* unclipped_temp = new vector<Point>();
	unclipped_temp = unclipped;



	//go through every boundary point
	for (int a = 0; a < numberOfBoundaryPoints; a++){
	
	// get one boundary vector
	// a is curent Boundary point(PA), b is next Boundary point(PB)
	int b;
	if(a == numberOfBoundaryPoints-1)
		b = 0;
	else
		b=a+1;
	
	//get boundary points PA and PB
	Point PA = Point(boundary_point[a][0],boundary_point[a][1],boundary_point[a][2]);
	Point PB = Point(boundary_point[b][0],boundary_point[b][1],boundary_point[b][2]);
	//calculate boundary vector
	Vector boundary = PB-PA;

		for (int i = 0; i < unclipped_temp->size(); i++) {
			//NEW
			//judge whether current point P0 is inside the boundary

			// i is curent object point(P0), j is next object point(P1)
			int j;
			if(i == unclipped_temp->size()-1)
				j = 0;
			else
				j=i+1;

			//get polygon points P0 and P1
			Point P0 = Point(unclipped_temp->at(i));
			Point P1 = Point(unclipped_temp->at(j));

			//get vector
			Vector PA_P0 = P0-PA;
			Vector PA_P1 = P1-PA;

			//current point P0 inside
			if ((PA_P0*boundary).z() <= 0){
				//next point P1 inside
				if((PA_P1*boundary).z() <= 0){
					//entirely inside
					//add only P1
					clipped->push_back(P1);
				}
				//next point P1 outside
				else{
					//leaving the boundary
					//add only P
					//intersect function
					Point P = intersect(PA,PB,P0,P1);
					clipped->push_back(P);
				}
			}
			//current point P0 outside
			else{
				//next point P1 inside
				if((PA_P1*boundary).z() <= 0){
					//entering the boundary
					//add P and P1
					//intersect function
					Point P = intersect(PA,PB,P0,P1);
					clipped->push_back(P);
					clipped->push_back(P1);
				}
				//next point P1 outside
				else{
					//entirely outside
					// do nothing
				}
			}//end judging point outside/inside
	
		}//end for (int i = 0; i < unclipped_temp->size(); i++)

		//if not reaching the last boundary point
		if (a != numberOfBoundaryPoints-1){
			//clear unclipped_temp
			unclipped_temp->clear();
			//assign clipped to unclipped
			unclipped_temp = clipped;
			//clear clipped
			//delete clipped;
			clipped = new vector<Point>();//don't know why 'clipped_temp->clear();' doesn't work
		}

	}//end for (int a = 0; a < numberOfBoundaryPoints; a++)
	return clipped;
}


vector<Point>* toVector(float unclippedArray[][3]) {
	vector<Point>* unclipped = new vector<Point>();
	for (int i = 0; i < numberOfPoints; i++) {
		unclipped->push_back(Point(unclippedArray[i][0],unclippedArray[i][1],unclippedArray[i][2]));
	}
	return unclipped;
}

void display(void) {

	vector<Point>* unclipped;

	switch(testcase) {
		case 0:
			numberOfPoints = 4;
			unclipped = toVector(unclipped0);
			break;
		case 1:
			numberOfPoints = 5;
			unclipped = toVector(unclipped1);
			break;
		case 2:
			numberOfPoints = 4;
			unclipped = toVector(unclipped2);
			break;
		case 3:
			numberOfPoints = 4;
			unclipped = toVector(unclipped3);
			break;
		case 4:
			numberOfPoints = 8;
			unclipped = toVector(unclipped4);
			break;
		case 5:
			numberOfPoints = 8;
			unclipped = toVector(unclipped5);
			break;
		case 6:
			numberOfPoints = 15;
			unclipped = toVector(unclipped6);
			break;
		case 7:
			numberOfPoints = 9;
			unclipped = toVector(unclipped7);
			break;
		default:
			cout << "Invalid Test case" << endl;
			return;
	}

	glClear(GL_COLOR_BUFFER_BIT);

	//draw desktop
	glPointSize(2.0f);
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINE_LOOP);
	//NEW
	for (int i = 0; i < numberOfBoundaryPoints; i++) {
		glVertex3f(boundary_point[i][0],boundary_point[i][1],boundary_point[i][2]);
	}
	//glVertex3f(-150.0f,-150.0f,1.0f);
	//glVertex3f(150.0f,-150.0f,1.0f);
	//glVertex3f(150.0f,150.0f,1.0f);
	//glVertex3f(-150.0f,150.0f,1.0f);
	glEnd();

	//draw Polygon
	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numberOfPoints; i++) {
		glVertex3fv(unclipped->at(i).toArray());
	}
	glEnd();

	vector<Point>* clipped = clip(unclipped);

	//draw clipped
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < clipped->size(); i++) {
		glVertex3fv(clipped->at(i).toArray());
	}
	glEnd();


	glutSwapBuffers();

	delete clipped;
	delete unclipped;
	

}

static void Reshape(int w, int h) {
	// when window is resized or reshaped
	glClearColor (0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	width = w;
	height = h;

	glViewport (0, 0, width, height);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	//use orthogonal projection
	glOrtho(-width/2, width/2, -height/2, height/2, -1.0, 1.0);
}

static void Key(unsigned char key, int x, int y) {
	switch (key) {
		case SPACE:
			testcase = (testcase + 1) % numberOfTestCases;
			break;
		default:
			glutIdleFunc(display);
	}
}

int main(int argc, char **argv) {

	// main method
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Clipping");

	glutKeyboardFunc(Key);
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();
}