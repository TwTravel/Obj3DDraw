//Scene.h
#include "Camera.h"
#include "Color.h"
//#include "Cube.h"
#include "Light.h"
#include "Transformation.h"
#include "Material.h"
#include "GLViewer.h"
#include "LightOff.h"
#include "Geometry.h"
//#include "Sphere.h"
#include "DrawStyle.h"
//#include "Line.h"
//#include "Polygon.h"
//#include "Cylinder.h"

int Light::LightNum=0;
int GLViewer::ViewerNum=0;
int GLViewer::BufType[]={GLUT_SINGLE,GLUT_SINGLE,GLUT_SINGLE};
int GLViewer::WinWidth[]={500,500,500};
int GLViewer::WinHeight[]={500,500,500};
Node *GLViewer::Root[]={NULL,NULL, NULL};

Transformation*GLViewer::trans1=NULL;
Transformation*GLViewer::trans2=NULL;
Transformation*GLViewer::trans3=NULL;
Transformation*GLViewer::trans4=NULL;
Transformation*GLViewer::trans5=NULL;




