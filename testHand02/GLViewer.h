//GLViewer.h
#ifndef GLVIEWER_H
#define GLVIEWER_H

#include "Node.h"

class GLViewer
{
 public:
  GLViewer();
  ~GLViewer();

  void CreateWin(char *Name, int Width, int Height);

  //set buffer, backcolor
  void SetValue(Enum PName, Enum Type);
  void Init(int argc, char **argv);
  void Show(Node *N);

 private:
  void GLInit();
  int ViewerIndex;
  char *WinName;
  float BackColor[3];

  static Node *Root[3];
  static int ViewerNum;
  static int BufType[3];
  static int WinWidth[3];
  static int WinHeight[3];
  static void Reshape0(int w, int h);
  static void Display0();
  static void Reshape1(int w, int h);
  static void Display1();
  static void Reshape2(int w, int h);
  static void Display2();
  
};

GLViewer::GLViewer()
{
  int i;

  ViewerIndex=ViewerNum;
  ViewerNum++;

  WinName="";
  BufType[ViewerIndex]=GLUT_SINGLE;
  Root[ViewerIndex]=NULL;
  for(i=0; i<3; i++)
    BackColor[i]=0.0;
}

GLViewer::~GLViewer()
{
  if(Root[ViewerIndex])
    {
      delete Root[ViewerIndex];
      Root[ViewerIndex]=NULL;
    }
}

void
GLViewer::Init(int argc, char **argv)
{
  if(ViewerIndex==0)
    glutInit(&argc, argv);
}

void 
GLViewer::CreateWin(char *Name, int Width, int Height)
{
  WinName=Name;
  WinWidth[ViewerIndex]=Width;
  WinHeight[ViewerIndex]=Height;
}

void
GLViewer::SetValue(Enum PName, Enum Type)
{
  switch(PName)
    {
    case BUFFER:
      if(Type==DOUBLE)
	BufType[ViewerIndex]=GLUT_DOUBLE;
      else if(Type==SINGLE)
	BufType[ViewerIndex]=GLUT_SINGLE;
      break;
    case BACKCOLOR:
      Root[ViewerIndex]->GetColor(Type, BackColor);
      break;
    default:
      break;
    }
}

void
GLViewer::Show(Node *N)
{
  GLInit();
  Root[ViewerIndex]=N;
  if(ViewerIndex==(ViewerNum-1))
    glutMainLoop();
}

//private
void 
GLViewer::GLInit()
{
  glutInitDisplayMode(BufType[ViewerIndex] | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WinWidth[ViewerIndex], WinHeight[ViewerIndex]);
  glutCreateWindow(WinName);
  switch(ViewerIndex)
    {
    case 0:
      glutReshapeFunc(Reshape0);
      glutDisplayFunc(Display0);
      break;
    case 1:
      glutReshapeFunc(Reshape1);
      glutDisplayFunc(Display1);
      break;
    case 2:
      glutReshapeFunc(Reshape2);
      glutDisplayFunc(Display2);
      break;
    default:
      break;
    }

  glEnable(GL_DEPTH_TEST);
  glClearColor(BackColor[0], BackColor[1], BackColor[2], 1.0);
  glutIdleFunc(Display0);
}

void
GLViewer::Display0()
{printf("=============================================\n");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Root[0]->Traverse();
  if(BufType[0]==GLUT_DOUBLE)
    glutSwapBuffers();
  glFlush();
  
  Transformation *trans1,*trans2,*trans3,*trans4,*trans5; 
  
   trans1 = (Transformation*) Root[0]->node_func_buffer[30+0]  ;
   trans2 = (Transformation*) Root[0]->node_func_buffer[30+1]  ;
   trans3 = (Transformation*) Root[0]->node_func_buffer[30+2]  ;
   trans4 = (Transformation*) Root[0]->node_func_buffer[30+3] ;
   trans5 = (Transformation*) Root[0]->node_func_buffer[30+4] ;
                              
   trans1->Transform[0][0] = int(trans1->Transform[0][0] + 1)%360;             
   trans2->Transform[0][0] = int(trans2->Transform[0][0] + 1)%360;             
   trans3->Transform[0][0] = int(trans3->Transform[0][0] + 1)%360;             
   trans4->Transform[0][0] = int(trans4->Transform[0][0] + 1)%360;             
   trans5->Transform[0][0] = int(trans5->Transform[0][0] + 1)%360; 
                    
	for(int i=0; i<30; i++)
         ((Material*)(Root[0]->node_func_buffer[i]))->Add();                                                                      
  /* ((Material*)(Root[0]->node_func_buffer[0]))->Add();                         
   ((Material*)(Root[0]->node_func_buffer[1]))->Add();                         
   ((Material*)(Root[0]->node_func_buffer[2]))->Add();                         
   ((Material*)(Root[0]->node_func_buffer[3]))->Add();                         
   ((Material*)(Root[0]->node_func_buffer[4]))->Add();                         
   ((Material*)(Root[0]->node_func_buffer[5]))->Add();                         
   ((Material*)(Root[0]->node_func_buffer[6]))->Add();*/
   
}


void
GLViewer::Reshape0(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  WinWidth[0]=w;
  WinHeight[0]=h;
}

void
GLViewer::Display1()
{printf("--------------------------------------------\n");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Root[1]->Traverse();
  if(BufType[1]==GLUT_DOUBLE)
    glutSwapBuffers();
  glFlush();
}

void
GLViewer::Reshape1(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  WinWidth[1]=w;
  WinHeight[1]=h;
}

void
GLViewer::Display2()
{ printf("++++++++++++++++++++++++++++++++++++++++++++");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Root[2]->Traverse();
  if(BufType[2]==GLUT_DOUBLE)
    glutSwapBuffers();
  glFlush();
}

void
GLViewer::Reshape2(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  WinWidth[2]=w;
  WinHeight[2]=h;
}


#endif
