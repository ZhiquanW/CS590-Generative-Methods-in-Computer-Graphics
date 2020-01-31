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
const GLfloat defaultIncrement = 0.7f;  // speed of rotation
GLfloat angleIncrement = defaultIncrement;

vector<Vect3d> v;  // all the points will be stored here

// window size
GLint wWindow = 1200;
GLint hWindow = 800;

// this defines what will be rendered
// see Key() how is it controlled
bool tangentsFlag = false;
bool pointsFlag = false;
bool curveFlag = true;
bool frenetFlag = false;
bool osculatingFlag = false;
int osculatingID = 0;
    // user define curve id
int curve_id = 1;
/*********************************
Some OpenGL-related functions DO NOT TOUCH
**********************************/
// displays the text message in the GL window
void GLMessage(char *message) {
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

void DrawCircle(Vect3d c, Vect3d N,Vect3d B,float r, Vect3d color) {
  int n = 1000;
  float interval = 360.0f / (float)n;
  Vect3d original = c - N.GetRotatedAxis(0, N).GetNormalized()*r;
  Vect3d next;
  for (int i = 0; i < n; ++i) {
    next = c - N.GetRotatedAxis(interval*(float)i, B).GetNormalized()*r;
    DrawLine(original,next ,color);
    original = next;
  }
}

// draws point at a with color
void DrawPoint(Vect3d a, Vect3d color) {
  glColor3fv(color);
  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex3fv(a);
  glEnd();
}

/**********************
LAB related MODIFY
***********************/

// This defines the actual curve
inline Vect3d P(GLfloat t) {
  if (curve_id == 1) {
    const float rad = 0.2f;
    const float height = 1.f;
    const float rot = 5.0f;
    // spiral with radius rad, height, and rotations rot
    return Vect3d(rad * (float)sin(rot * M_PI * t), height * t,
                  rad * (float)cos(rot * M_PI * t));
  } else if (curve_id == 2) {
    const float control = 10.0f;
    const float height = 1.f;
    const float rad = 0.05f;
    return Vect3d(
        rad * 16 * sin(control * t) * sin(control * t) * sin(control * t),
        height * t,
        rad * (13 * cos(control * t) - 5 * cos(control * 2 * t) -
               2 * cos(control * 3 * t) - cos(control * 4 * t)));

  } else if (curve_id == 3) {
    const float control = 10.0f;
    const float height = 1.f;
    const float rad = 0.05f;
    return Vect3d(
        rad /((t+1)*2)* 16 * sin(control * t) * sin(control * t) * sin(control * t),
        height * t,
        rad /((t+1)*2)* 16 * cos(control * t) * cos(control * t) * cos(control * t));
  }
}

// This fills the <vector> *a with data.
void CreateCurve(vector<Vect3d> *a, int n) {
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
void Randomize(vector<Vect3d> *a) {
  const float intensity = 0.01f;
  for (unsigned int i = 0; i < a->size(); i++) {
    Vect3d r(random11(), random11(), random11());
    a->at(i) = a->at(i) + intensity * r;
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
  vector<Vect3d> T_vector;
  vector<Vect3d> N_vector;
  vector<Vect3d> B_vector;
  for (int i = 0; i < v.size(); i++) {
    Vect3d T;
    int after_id = i + 1 >= v.size() ? v.size() - 1 : i + 1;
    int previous_id = i - 1 < 0 ? 0 : i - 1;
    T = (v[after_id] - v[previous_id]).GetNormalized() * 0.2f;
    T_vector.push_back(T);
  }
  for (int i = 0; i < v.size(); ++i) {
    Vect3d N;
    int after_id = i + 1 >= v.size() ? v.size() - 1 : i + 1;
    int previous_id = i - 1 < 0 ? 0 : i - 1;
    N = (T_vector[after_id] - T_vector[previous_id]).GetNormalized() * 0.2f;
    N_vector.push_back(N);
  }
  for (int i = 0; i < v.size(); ++i) {
    Vect3d B = T_vector[i].Cross(N_vector[i]).GetNormalized() * 0.2f;
    Vect3d N = B.Cross(T_vector[i]).GetNormalized() * 0.2f;
    N_vector[i] = N;
    B_vector.push_back(B);
  }
  CoordSyst();
  // draw the curve
  if (curveFlag) {
    for (unsigned int i = 0; i < v.size() - 1; i++) {
      DrawLine(v[i], v[i + 1], almostBlack);
    }
  }

  // draw the points
  if (pointsFlag) {
    for (unsigned int i = 0; i < v.size() - 1; i++) {
      DrawPoint(v[i], blue);
    }
  }

  // draw the tangents
  if (tangentsFlag) {
    for (unsigned int i = 0; i < v.size() - 1; i++) {
      Vect3d tan;
      tan =
          v[i + 1] -
          v[i];  // too simple - could be better from the point after AND before
      tan.Normalize();
      tan *= 0.2;
      DrawLine(v[i], v[i] + tan, red);
    }
    // draw T
    for (int i = 0; i < v.size(); i++) {
      DrawLine(v[i], v[i] + T_vector[i], red);
      DrawLine(v[i], v[i] + N_vector[i], blue);
      DrawLine(v[i], v[i] + B_vector[i], green);
    }
   
  }
  float k = T_vector[osculatingID].Cross(N_vector[osculatingID]).Length() /
            pow((T_vector[osculatingID].Length()), 3);
  float r = 1.0f / k;
  // std::cout << r << std::endl;
  // Vect3d a = B_vector[osculatingID].GetNormalized();
  DrawCircle(v[osculatingID] + N_vector[osculatingID].GetNormalized() * r,
             N_vector[osculatingID], B_vector[osculatingID], r, red);
 // if (frenetFlag) {
 // 
	//
 //   if (osculatingFlag) {
 //    
	//}
 //   //  if (curve_id == 1) {
 //   //    const float rad = 0.2f;
 //   //    const float height = 1.f;
 //   //    const float rot = 5.0f;
 //   //    const float tmpRPI = rot * M_PI;
 //   //    const float tmpRRPI = tmpRPI * tmpRPI;
 //   //    for (unsigned int i = 0; i < v.size() - 1; ++i) {
 //   //      float t = 1.0f / v.size() * i;
 //   //      // draw T
 //   //      Vect3d tan(rad * tmpRPI * cos(tmpRPI * t), height,
 //   //                 -rad * tmpRPI * sin(tmpRPI * t));
 //   //      tan.Normalize();
 //   //      tan *= 0.2;
 //   //      DrawLine(v[i], v[i] + tan, red);
 //   //      // draw N
 //   //      Vect3d N(-rad * tmpRRPI * sin(tmpRPI * t), 0,
 //   //               -rad * tmpRRPI * cos(tmpRPI * t));
 //   //      N.Normalize();
 //   //      N *= 0.2;
 //   //      DrawLine(v[i], v[i] + N, Vect3d(0.2, 0.3, 0.7));
 //   //      // draw B
 //   //      Vect3d B = tan.Cross(N);
 //   //      B.Normalize();
 //   //      B *= 0.2f;
 //   //      DrawLine(v[i], v[i] + B, Vect3d(0.5, 0.3, 0.4));
 //   //    }

 //   //  } else if (curve_id == 2) {
 //   //    const float control = 10.0f;
 //   //    const float height = 1.f;
 //   //    for (unsigned int i = 0; i < v.size() - 1; i++) {
 //   //      const float rad = 0.05f;
 //   //      float t = 1.0f / v.size() * i;
 //   ////Draw T
 //   //      Vect3d T(rad * 48 * control * sin(control * t) * sin(control * t) *
 //   //                   cos(control * t),
 //   //               height, rad * (-13 * control * sin(control * t)));
 //   //      T.Normalize();
 //   //      T *= 0.2;
 //   //      DrawLine(v[i], v[i] + T, red);
 //   ////Draw N
 //   //      Vect3d N(96.0f * rad * control * control *
 //   //      cos(control*t),0,rad*(-13*control*control*2*cos(control*t)-20*control*control*cos(2*control*t)+18*control*control*cos(control
 //   //*3*t)+16*control*control*cos(control*4*t)));
 //   //      N.Normalize();
 //   //      N *= 0.2f;
 //   //      DrawLine(v[i], v[i] + N, Vect3d(0.2, 0.3, 0.7));
 //   //     /* Vec3td N()
 //   //	return Vect3d(
 //   //          rad * 16 * sin(control * t) * sin(control * t) * sin(control *
 //   //          t), height * t, rad * (13 * cos(control * t) - 5 * cos(control *
 //   //          2 * t) -
 //   //                 2 * cos(control * 3 * t) - cos(control * 4 * t)));*/
 //   //    }
 //   //  }
 // }
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
    case '1':
      curve_id = 1;
      InitArray(steps);
      break;
    case '2':
      curve_id = 2;
      InitArray(steps);
      break;
    case '3':
      curve_id = 3;
      InitArray(steps);
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
    case 'f':
      frenetFlag = !frenetFlag;
      break;
    case 'o':
      osculatingFlag = !osculatingFlag;
      break;
    case '>':
      if (osculatingID == v.size() - 1) {
        break;
      }
      osculatingID += 1;
      break;
    case '<':
      if (osculatingID == 0) {
        break;
	  }
      osculatingID -= 1;
      break;
    case 32: {
      if (angleIncrement == 0)
        angleIncrement = defaultIncrement;
      else
        angleIncrement = 0;
      break;
    }
    case 's': {
      sign = -sign;
      break;
    }
    case '-': {
      steps--;
      if (steps < 1) steps = 1;
      if (osculatingID >= steps) osculatingID = steps - 1;
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
  GLMessage("Lab 1 - CS 590CGS");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40, (GLfloat)wWindow / (GLfloat)hWindow, 0.01,
                 100);         // set the camera
  glMatrixMode(GL_MODELVIEW);  // set the scene
  glLoadIdentity();
  gluLookAt(0, 10, 10, 0, 0, 0, 0, 1,
            0);  // set where the camera is looking at and from.
  static GLfloat angle = 0;
  angle += angleIncrement;
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

int main(int argc, char **argv) {
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
