/*********************************/
/* CS 590CGS Lab framework        */
/* (C) Bedrich Benes 2020        */
/* bbenes ~ at ~ purdue.edu      */
/* Press +,- to add/remove points*/
/*       r to randomize          */
/*       s to change rotation    */
/*       c to render curve       */
/*       t to render tangents    */
/*       p to render points      */
/*       s to change rotation    */
/*********************************/

#include <stdio.h>
#include <string.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>  //Standard template library class

// in house created libraries
#include "math/vect3d.h"  //for vector manipulation
#include "trackball.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "freeglut.lib")

using namespace std;

// some trackball variables -> used in mouse feedback
TrackBallC trackball;
bool mouseLeft, mouseMid, mouseRight;

GLuint points = 0;  // number of points to display the object
int steps = 20;     //# of subdivisions
bool needRedisplay = false;
GLfloat sign = +1;                      // diretcion of rotation
const GLfloat defaultIncrement = 1.2f;  // speed of rotation

vector<Vect3d> v;  // all the points will be stored here
vector<Vect3d> bezier_control_vertex;
vector<Vect3d> bezier_colors;
vector<Vect3d> sample_bezier_control_vertex;
vector<Vect3d> sample_bezier_vertex;
vector<Vect3d> fitting_bezier_control_vertex;
vector<Vect3d> fitting_bezier_vertex;
int beizer_control_vertex_num = 4;
int bezier_vertex_num = 100;
// window size
GLint wWindow = 1200;
GLint hWindow = 800;

// this defines what will be rendered
// see Key() how is it controlled
bool tangentsFlag = false;
bool pointsFlag = false;
bool curveFlag = true;
bool genBezierFlag = false;
bool bezierFlag = false;
bool genPieceBezierFlag = false;
bool pieceBezierFlag = false;
bool isFittingFlag = false;
bool startFittingFlag = false;
/*********************************
Some OpenGL-related functions DO NOT TOUCH
**********************************/
// displays the text message in the GL window
void GLMessage(char* message) {
	static int i;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.f, 100.f, 0.f, 100.f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3ub(0, 0, 255);
	glRasterPos2i(10, 10);
	for (i = 0; i < (int)strlen(message); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

// called when a window is reshaped
void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glEnable(GL_DEPTH_TEST);
	// remember the settings for the camera
	wWindow = w;
	hWindow = h;
}

// Some simple rendering routines using old fixed-pipeline OpenGL
// draws line from a to b with color
void DrawLine(Vect3d a, Vect3d b, Vect3d color) {
	glColor3fv(color);
	glBegin(GL_LINES);
	glVertex3fv(a);
	glVertex3fv(b);
	glEnd();
}

// draws point at a with color
void DrawPoint(Vect3d a, Vect3d color) {
	glColor3fv(color);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3fv(a);
	glEnd();
}
Vect3d GetDeCasteljauPoint(int i, int j, float t,
	vector<Vect3d> control_points) {
	if (i == 0) {
		return control_points[j];
	}
	else {
		return (1 - t) * GetDeCasteljauPoint(i - 1, j - 1, t, control_points) +
			t * GetDeCasteljauPoint(i - 1, j, t, control_points);
	}
}
void DrawBezier(vector<Vect3d> control_points, int sample_num,Vect3d color) {
	float interval = 1 / (float)sample_num;
	int degree = control_points.size() - 1;
	for (int i = 0; i <= sample_num; ++i) {
		Vect3d tmp_point = GetDeCasteljauPoint(degree, degree, (float)i * interval,
			control_points);
		DrawPoint(tmp_point, color);
	}
}


/**********************
LAB related MODIFY
***********************/

// This defines the actual curve
inline Vect3d P(GLfloat t) {
	const float rad = 0.2f;
	const float height = 1.f;
	const float rot = 5.f;
	return Vect3d(
		rad * (float)sin(rot * M_PI * t), height * t,
		rad *
		(float)cos(rot * M_PI *
			t));  // spiral with radius rad, height, and rotations rot
}

// This fills the <vector> *a with data.
void CreateCurve(vector<Vect3d>* a, int n) {
	GLfloat step = 1.f / n;
	for (int i = 0; i < n; i++) {
		a->push_back(P(i * step));
	}
}

// Call THIS for a new curve. It clears the old one first
void InitArray(int n) {
	v.clear();
	CreateCurve(&v, n);
}

// returns random number from <-1,1>
inline float random11() { return 2.f * rand() / (float)RAND_MAX - 1.f; }

// randomizes an existing curve by adding random number to each coordinate
void Randomize(vector<Vect3d>* a) {
	const float intensity = 0.01f;
	for (unsigned int i = 0; i < a->size(); i++) {
		Vect3d r(random11(), random11(), random11());
		a->at(i) = a->at(i) + intensity * r;
	}
}
void CreateBezierControlPoints(int num) {
	bezier_control_vertex.clear();
	for (int i = 0; i < num; ++i) {
		Vect3d r(random11(), random11(), random11());
		bezier_control_vertex.push_back(r);
	}
}

vector<Vect3d> CreateBezierFittingControlPoints(int num) {
	std::vector<Vect3d> vertexs;
	for (int i = 0; i < num; ++i) {
		Vect3d r(random11(), random11(), random11());
		vertexs.push_back(r);
	}
	return vertexs;
}
std::vector<Vect3d>  CreateBezierFittingPoints(std::vector<Vect3d> control_points,int num) {
	std::vector<Vect3d> vertexs;
	float interval = 1 / (float)num;
	int degree = control_points.size() - 1;
	for (int i = 0; i <= num; ++i) {
		Vect3d tmp_point = GetDeCasteljauPoint(degree, degree, (float)i * interval,
			control_points);
		vertexs.push_back(tmp_point);
	}
	return vertexs;
}

std::vector<Vect3d> ParameterizeT(vector<Vect3d> samples) {
	float sum_dis = 0;
	vector<Vect3d>::iterator itor;
	Vect3d tmp_point = *samples.begin();
	for (itor = samples.begin()+1; itor != samples.end(); ++itor) {
		sum_dis += (*itor - tmp_point).Length();
		tmp_point = *itor;
	}
	tmp_point= *samples.begin();
	float intergalDis = 0;
	vector<float> parameterizedT;
	for (itor = samples.begin(); itor != samples.end(); ++itor) {
		intergalDis += (*itor - tmp_point).Length();
		tmp_point = *itor;
		parameterizedT.push_back(intergalDis / sum_dis);

	}
}
std::vector<Vect3d> FitBezier(vector<Vect3d> samples,int start,int end) {
	// compute two tangent vectors
	Vect3d t_hat_1 = (samples[1] - samples[0]).GetNormalized();
	Vect3d t_hat_2 = (samples[samples.size() - 1] - samples[samples.size() - 2]).GetNormalized();
	std::vector<Vect3d> parameterized_t;

}
void CreateC1piecewiseBezierControlPoints() {
	bezier_control_vertex.clear();
	int control_points_num = 10;
	for (int i = 0; i < 4; ++i) {
		Vect3d r(random11(), random11(), random11());
		bezier_control_vertex.push_back(r);
	}
	bezier_control_vertex.push_back(2 * bezier_control_vertex[3] -
		bezier_control_vertex[2]);
	for (int i = 0; i < 2; ++i) {
		Vect3d r(random11(), random11(), random11());
		bezier_control_vertex.push_back(r);
	}
	bezier_control_vertex.push_back(2 * bezier_control_vertex[6] -
		bezier_control_vertex[5]);
	for (int i = 0; i < 2; ++i) {
		Vect3d r(random11(), random11(), random11());
		bezier_control_vertex.push_back(r);
	}
}

// display coordinate system
void CoordSyst() {
	Vect3d a, b, c;
	Vect3d origin(0, 0, 0);
	Vect3d red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1),
		almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

	// draw the coordinate system
	a.Set(1, 0, 0);
	b.Set(0, 1, 0);
	c.Set(Vect3d::Cross(a, b));  // use cross product to find the last vector
	glLineWidth(4);
	DrawLine(origin, a, red);
	DrawLine(origin, b, green);
	DrawLine(origin, c, blue);
	glLineWidth(1);
}

// this is the actual code for the lab
void Lab01() {
	Vect3d a, b, c;
	Vect3d origin(0, 0, 0);
	Vect3d red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1),
		almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

	CoordSyst();
	// draw the curve
	if (curveFlag)
		for (unsigned int i = 0; i < v.size() - 1; i++) {
			DrawLine(v[i], v[i + 1], almostBlack);
		}

	// draw the points
	if (pointsFlag)
		for (unsigned int i = 0; i < v.size() - 1; i++) {
			DrawPoint(v[i], blue);
		}

	// draw the tangents
	if (tangentsFlag)
		for (unsigned int i = 0; i < v.size() - 1; i++) {
			Vect3d tan;
			tan =
				v[i + 1] -
				v[i];  // too simple - could be better from the point after AND before
			tan.Normalize();
			tan *= 0.2;
			DrawLine(v[i], v[i] + tan, red);
		}
	if (bezierFlag) {
		DrawBezier(bezier_control_vertex, steps,red);
	}
	if (pieceBezierFlag) {
		int counter = 0;
		for (int i = 0; i < 3; ++i) {
			vector<Vect3d> tmpControlPoitns;
			for (int j = 0; j < 4; ++j) {
				tmpControlPoitns.push_back(bezier_control_vertex[counter++]);
			}
			counter--;
			DrawBezier(tmpControlPoitns, steps, bezier_colors[i]);
		}
	}
	if (isFittingFlag) {
		DrawBezier(sample_bezier_control_vertex, steps, red);
		DrawBezier(fitting_bezier_control_vertex, steps, blue);
	}
}
// the main rendering function
void RenderObjects() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// set camera
	glMatrixMode(GL_MODELVIEW);
	trackball.Set3DViewCamera();
	// call the student's code from here
	Lab01();
}

// Add here if you want to control some global behavior
// see the pointFlag and how is it used
void Kbd(unsigned char a, int x, int y)  // keyboard callback
{
	switch (a) {
	case 27:
		exit(0);
		break;
	case 't':
		tangentsFlag = !tangentsFlag;
		break;
	case 'p':
		pointsFlag = !pointsFlag;
		break;
	case 'c':
		curveFlag = !curveFlag;
		break;
	case 32: {
		if (isFittingFlag == false) {
			isFittingFlag = true;
		}
		break;
	}
	case 's': {
		sign = -sign;
		break;
	}
	case '-': {
		steps--;
		if (steps < 1) steps = 1;
		InitArray(steps);
		break;
	}
	case '+': {
		steps++;
		InitArray(steps);
		break;
	}
	case 'r': {
		Randomize(&v);
		break;
	}
	case 'R': {
		if (genBezierFlag == false) {
			CreateBezierControlPoints(beizer_control_vertex_num);

		}
		bezierFlag = !bezierFlag;
		genBezierFlag = !genBezierFlag;
		break;
	}
	case 'B': {
		if (genPieceBezierFlag == false) {
			CreateC1piecewiseBezierControlPoints();
		}
		genPieceBezierFlag = !genPieceBezierFlag;
		pieceBezierFlag = !pieceBezierFlag;
		break;
	}
	}
	cout << "[points]=[" << steps << "]" << endl;
	glutPostRedisplay();
}

/*******************
OpenGL code. Do not touch.
******************/
void Idle(void) {
	glClearColor(0.5f, 0.5f, 0.5f, 1);  // background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLMessage("Lab 2 - CS 590CGS");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (GLfloat)wWindow / (GLfloat)hWindow, 0.01,
		100);         // set the camera
	glMatrixMode(GL_MODELVIEW);  // set the scene
	glLoadIdentity();
	gluLookAt(0, 10, 10, 0, 0, 0, 0, 1,
		0);  // set where the camera is looking at and from.
	static GLfloat angle = 0;
	angle += defaultIncrement;
	if (angle >= 360.f) angle = 0.f;
	glRotatef(sign * angle, 0, 1, 0);
	RenderObjects();
	glutSwapBuffers();
}

void Display(void) {}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		trackball.Set(true, x, y);
		mouseLeft = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		trackball.Set(false, x, y);
		mouseLeft = false;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		trackball.Set(true, x, y);
		mouseMid = true;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
		trackball.Set(true, x, y);
		mouseMid = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		trackball.Set(true, x, y);
		mouseRight = true;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		trackball.Set(true, x, y);
		mouseRight = false;
	}
}

void MouseMotion(int x, int y) {
	if (mouseLeft) trackball.Rotate(x, y);
	if (mouseMid) trackball.Translate(x, y);
	if (mouseRight) trackball.Zoom(x, y);
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	for (int i = 0; i < 3; ++i) {
		bezier_colors.push_back(Vect3d(random11(), random11(), random11()));
	}
	glutInitDisplayString("stencil>=2 rgb double depth samples");
	glutInit(&argc, argv);
	glutInitWindowSize(wWindow, hWindow);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Surface of Revolution");
	// GLenum err = glewInit();
	// if (GLEW_OK != err){
	// fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	//}
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Kbd);  //+ and -
	glutSpecialUpFunc(NULL);
	glutSpecialFunc(NULL);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	InitArray(steps);
	glutMainLoop();
	return 0;
}
