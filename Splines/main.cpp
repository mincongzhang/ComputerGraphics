#include <stdlib.h>
#include <vector>
#include <iostream>
#include "point.h"

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

#define SPACE 32

int testcase = 0;
int numberOfPoints;
bool convert_bezier = false;

int numberOfTestCases = 7;

static float cp0[3][2] = {
	{-5.0, 0.0}, {0.0, 0.0},
	{5.0, 0.0}
};

static float cp1[4][2] = {
	{-5.0, 0.0}, {-1.0, 3.0},
	{1.0,-3.0}, {5.0, 0.0}
};

static float cp2[5][2] = {
	{-5.0, 0.0}, {0.0, 5.0},
	{5.0,0.0}, {0.0, -5.0},
	{-5.0,0.0}
};

static float cp3[11][2] = {
	{-5.0, 0.0}, {-4.0, 3.0},
	{-3.0,-3.0}, {-2.0, 3.0},
	{-1.0,-3.0}, {0.0, 3.0},
	{1.0,-3.0}, {2.0, 3.0},
	{3.0,-3.0}, {4.0, 3.0},
	{5.0, 0.0}
};

static float cp4[4][2] = {
	{-3.0, 3.0}, {2.0, -2.0},
	{-2.0,-2.0}, {3.0, 3.0}
};

static float cp5[10][2] = {
	{-5.0, 0.0}, {-4.5, 4.0},
	{-3.5,1.0}, {-3.0, 3.0},
	{-1.0,-2.0},{0.0,-3.0},
	{0.5,-3.0},{1.0,-3.0},
	{4.0,1.0},{5.0,0.0}
};

static float cp6[4][2] = {
	{-2.0, 0.0},{-2.0, 2.5},
	{2.0, 2.5},{2.0, 0.0},
};

vector<Point>* toVector(float controlPointArray[][2]) {
	// convert an array into a vector
	vector<Point>* controlPoints = new vector<Point>();
	for (int i = 0; i < numberOfPoints; i++) {
		controlPoints->push_back(Point(controlPointArray[i][0],controlPointArray[i][1],0.0));
	}
	return controlPoints;
}

vector<Point>* generate_bspline_controlpoint(vector<Point>* controlPoints) {
	//TO DO: Change this so that a B-spline is created
	vector<Point>* bspline_control_points = new vector<Point>();

	// go over current control points
	int control_end = controlPoints->size()-1;
	int first_point_x = controlPoints->at(0).x();
	int first_point_y = controlPoints->at(0).y();
	int end_point_x = controlPoints->at(control_end).x();
	int end_point_y = controlPoints->at(control_end).y();
	bool is_loop = first_point_x==end_point_x && first_point_y==end_point_y;

	//1.If the curve is not a loop, generate 2n-3 new control points
	//Throw away the first(i=0) and last(i=control_end) control
	for (int i = 1; i <= control_end-1; i++) {
		float x_k = controlPoints->at(i-1).x();
		float y_k = controlPoints->at(i-1).y();
		float x_i = controlPoints->at(i).x();
		float y_i = controlPoints->at(i).y();
		float x_j = controlPoints->at(i+1).x();
		float y_j = controlPoints->at(i+1).y();

		//Refining Uniform Cubic Bsplines:
		//(1)middle control point
		float x_ki = 1.0f/2*(x_k+x_i);
		float y_ki = 1.0f/2*(y_k+y_i);
		float x_ij = 1.0f/2*(x_i+x_j);
		float y_ij = 1.0f/2*(y_i+y_j);
		//(2)new control point
		float x_i_new = 1.0f/8*(x_k+6*x_i+x_j);
		float y_i_new = 1.0f/8*(y_k+6*y_i+y_j);

		bspline_control_points->push_back(Point(x_ki,y_ki,0.0f));
		bspline_control_points->push_back(Point(x_i_new,y_i_new,0.0f));
		if(i==control_end-1){
			bspline_control_points->push_back(Point(x_ij,y_ij,0.0f));
			//2.If the curve is a loop, generate 2n new control points by averaging across the loop
			if(is_loop){
				//push back 3 extra control points acrossing the loop
				float x_end = controlPoints->at(control_end).x();
				float y_end = controlPoints->at(control_end).y();
				float x_0 = controlPoints->at(0).x();
				float y_0 = controlPoints->at(0).y();
				float x_1 = controlPoints->at(1).x();
				float y_1 = controlPoints->at(1).y();
				float x_2 = controlPoints->at(2).x();
				float y_2 = controlPoints->at(2).y();
				//(1)refine the last(first) control point
				float x_loopend = 1.0f/8*(x_i+6*x_end+x_1);
				float y_loopend = 1.0f/8*(y_i+6*y_end+y_1);
				bspline_control_points->push_back(Point(x_loopend,y_loopend,0.0f));
				//(2)push back a mid control point between the last(first) and the second control point
				float x_mid1 = 1.0f/2*(x_0+x_1);
				float y_mid1 = 1.0f/2*(y_0+y_1);
				bspline_control_points->push_back(Point(x_mid1,y_mid1,0.0f));
				//(3)refine the second control point
				float x_loopfirst = 1.0f/8*(x_0+6*x_1+x_2);
				float y_loopfirst = 1.0f/8*(y_0+6*y_1+y_2);
				bspline_control_points->push_back(Point(x_loopfirst,y_loopfirst,0.0f));
				//(4)push back a mid control point between the second and the third control point
				float x_mid2 = 1.0f/2*(x_1+x_2);
				float y_mid2 = 1.0f/2*(y_1+y_2);
				bspline_control_points->push_back(Point(x_mid2,y_mid2,0.0f));
			}
		}
	}
	return bspline_control_points;
}

//draw B-spline curve
vector<Point>* bspline(vector<Point>* bspline_controlpoints) {
	vector<Point>* bspline_points = new vector<Point>();
	int divisions = 100;

	// go over bspline control points
	int control_end = bspline_controlpoints->size()-1;
	for (int i = 0; i <= control_end-3; i++) {
		
		float x0 = bspline_controlpoints->at(i).x();
		float y0 = bspline_controlpoints->at(i).y();
		float x1 = bspline_controlpoints->at(i+1).x();
		float y1 = bspline_controlpoints->at(i+1).y();
		float x2 = bspline_controlpoints->at(i+2).x();
		float y2 = bspline_controlpoints->at(i+2).y();
		float x3 = bspline_controlpoints->at(i+3).x();
		float y3 = bspline_controlpoints->at(i+3).y();

		//B-spline matrix parameters
		float B0[4] = {1.0f/6,-3.0f/6,3.0f/6,-1.0f/6};
		float B1[4] = {4.0f/6,0.0f/6,-6.0f/6,3.0f/6};
		float B2[4] = {1.0f/6,3.0f/6,3.0f/6,-3.0f/6};
		float B3[4] = {0,0,0,1.0f/6};

		//B-spline points 
		float bspline_x[4] = {x0*B0[0]+x1*B1[0]+x2*B2[0]+x3*B3[0],x0*B0[1]+x1*B1[1]+x2*B2[1]+x3*B3[1],
							  x0*B0[2]+x1*B1[2]+x2*B2[2]+x3*B3[2],x0*B0[3]+x1*B1[3]+x2*B2[3]+x3*B3[3]};
		float bspline_y[4] = {y0*B0[0]+y1*B1[0]+y2*B2[0]+y3*B3[0],y0*B0[1]+y1*B1[1]+y2*B2[1]+y3*B3[1],
							  y0*B0[2]+y1*B1[2]+y2*B2[2]+y3*B3[2],y0*B0[3]+y1*B1[3]+y2*B2[3]+y3*B3[3]};

		// split the curve between two control points into many pieces to get a curve like result
		for (int j = 0; j <= divisions; j++) {
			float t = (float) j / (float) divisions;
			float xt = bspline_x[0]+bspline_x[1]*t+bspline_x[2]*t*t+bspline_x[3]*t*t*t;
			float yt = bspline_y[0]+bspline_y[1]*t+bspline_y[2]*t*t+bspline_y[3]*t*t*t;
			bspline_points->push_back(Point(xt,yt,0.0f));
		}
	}
	return bspline_points;
}

//Convert B-spline control points to Bezier control points
vector<Point>* bspline2bezier(vector<Point>* bspline_controlpoints) {
	vector<Point>* bezier_controlpoints = new vector<Point>();

	// go over bspline control points
	int control_end = bspline_controlpoints->size()-1;
	//convert a complete B-spline into piecewise Biezier form
	//Method 1: Forward (transfer B-spline control points to Biezier control points)
	//Drawback: cannot generate sufficient Biezier control point (B-spline to  Bezier == one to many mapping)
	/*
	for (int i = 0; i <= control_end; i++) {

		float x0 = bspline_controlpoints->at(i/3).x();
		float y0 = bspline_controlpoints->at(i/3).y();
		float x1 = bspline_controlpoints->at(i/3+1).x();
		float y1 = bspline_controlpoints->at(i/3+1).y();
		float x2 = bspline_controlpoints->at(i/3+2).x();
		float y2 = bspline_controlpoints->at(i/3+2).y();

		if(i%3==0){
			bezier_controlpoints->push_back(Point(1.0f/6*(x0+4*x1+x2),1.0f/6*(y0+4*y1+y2),0.0f));
		};
		if(i%3==1){
			bezier_controlpoints->push_back(Point(1.0f/6*(4*x1+2*x2),1.0f/6*(4*y1+2*y2),0.0f));
		};
		if(i%3==2){
			bezier_controlpoints->push_back(Point(1.0f/6*(2*x1+4*x2),1.0f/6*(2*y1+4*y2),0.0f));
		};
	}
	*/

	//Method 2: Backward (get Biezier control points from B-spline control points)
	for (int i = 0; i <=control_end-2; i++) {

		float x0 = bspline_controlpoints->at(i).x();
		float y0 = bspline_controlpoints->at(i).y();
		float x1 = bspline_controlpoints->at(i+1).x();
		float y1 = bspline_controlpoints->at(i+1).y();
		float x2 = bspline_controlpoints->at(i+2).x();
		float y2 = bspline_controlpoints->at(i+2).y();

		if(i==0){
			bezier_controlpoints->push_back(Point(1.0f/6*(x0+4*x1+x2),1.0f/6*(y0+4*y1+y2),0.0f));
		}
		else{
			bezier_controlpoints->push_back(Point(1.0f/6*(4*x0+2*x1),1.0f/6*(4*y0+2*y1),0.0f));
			bezier_controlpoints->push_back(Point(1.0f/6*(2*x0+4*x1),1.0f/6*(2*y0+4*y1),0.0f));
			bezier_controlpoints->push_back(Point(1.0f/6*(x0+4*x1+x2),1.0f/6*(y0+4*y1+y2),0.0f));
		};
		
	}
	return bezier_controlpoints;
}

void display(void)
{
	vector<Point>* controlPoints;

	// load control points
   	switch(testcase) {
		case 0:
			numberOfPoints = 3;
			controlPoints = toVector(cp0);
			break;
		case 1:
			numberOfPoints = 4;
			controlPoints = toVector(cp1);
			break;
		case 2:
			numberOfPoints = 5;
			controlPoints = toVector(cp2);
			break;
		case 3:
			numberOfPoints = 11;
			controlPoints = toVector(cp3);
			break;
		case 4:
			numberOfPoints = 4;
			controlPoints = toVector(cp4);
			break;
		case 5:
			numberOfPoints = 10;
			controlPoints = toVector(cp5);
			break;
		case 6:
			numberOfPoints = 4;
			controlPoints = toVector(cp6);
			break;
		default:
			cout << "Invalid Test case" << endl;
			return;
	}

   vector<Point>* bspline_controlpoint = generate_bspline_controlpoint(controlPoints);
   vector<Point>* bspline_points = bspline(bspline_controlpoint);

   glClear(GL_COLOR_BUFFER_BIT);
   glPointSize(5.0f);

   // draw B-spline control points
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      for (int i = 0; i < controlPoints->size(); i++) 
         glVertex2f(controlPoints->at(i).x(),controlPoints->at(i).y());
   glEnd();

   // draw B-spline curve
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (int i = 0; i < bspline_points->size(); i++) 
         glVertex2f(bspline_points->at(i).x(),bspline_points->at(i).y());
   glEnd();

   if(convert_bezier){
   vector<Point>* bezier_controlpoints = bspline2bezier(controlPoints);
   vector<Point>* bezier_points = bspline2bezier(bspline_points);
      // draw Bezier control points
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (int i = 0; i < bezier_controlpoints->size(); i++) 
         glVertex2f(bezier_controlpoints->at(i).x(),bezier_controlpoints->at(i).y());
   glEnd();

   // draw Bezier curve
   glColor3f(1.0, 0.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (int i = 0; i < bezier_points->size(); i++) 
         glVertex2f(bezier_points->at(i).x(),bezier_points->at(i).y());
   glEnd();

   };
   glutSwapBuffers();

	delete controlPoints;
	delete bspline_points;

}

static void Reshape(int width, int height) {
	// when window is resized or reshaped
	glClearColor (0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport (0, 0, width, height);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	//use orthogonal projection
	glOrtho(-5.0, 5.0, -5.0, 5.0, -1.0, 1.0);
}

static void Key(unsigned char key, int x, int y) {
	switch (key) {
		case SPACE:
			testcase = (testcase + 1) % numberOfTestCases;
			break;
		case 'b':
			convert_bezier = !convert_bezier;
			break;
		default:
			glutIdleFunc(display);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("B splines");

	glutKeyboardFunc(Key);
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();
}
