//Scene.cc

#include "Scene.h"

#define BaseRadius 0.2
#define Radius 0.08
#define BaseLen 1
#define UpLen 0.6
#define LowLen 0.6
#define EyeRadius 0.04
#define ChairLegLen 0.55

int
main(int argc, char **argv)
{

  float v[][3]={{-0.3,-0.2,0.0},{0.3, -0.2, 0.0},
		{0.3, 0.2, 0.0},{-0.3, 0.2, 0.0} };


  float vx[][3]={{0,  -0.02, 0.0},{100, -0.02, 0.0},
		         {100, 0.02, 0.0},{  0,  0.02, 0.0} };

  float vy[][3]={{-0.02,  0, 0.0},{ -0.02, 100, 0.0},
		         {0.02 ,100, 0.0},{  0.02,   0, 0.0} };		
		
  float vz[][3]={{-0.02,  0, 0.0},{ -0.02,  0, 100.0},
		         {0.02 ,0, 100.0},{  0.02,   0, 0.0} };	
		
  //Light nodes
  Light *Light1=new Light;
  Light *Light2=new Light;
  
  TurnOff *Off1=new TurnOff(Light1);
  TurnOff *Off2=new TurnOff(Light2);

  //Setting Light Values :
  Light1->SetValue(POSITION, -2, -3, 1.5, 1);
  Light1->SetValue(SPOT_DIRECTION, 2, 3, -1.5);
  Light1->SetValue(CUTOFFANGLE, 40.0);
  Light1->TurnOn();     Light1->nodename="Light1";
   //string nodespace;

  Light2->SetValue(POSITION, 5, 5, 5, 0);
  Light2->SetValue(SPECULAR, 1.0, 1.0, 1.0, 1.0);
  Light2->SetValue(DIFFUSE, 1.0, 1.0, 1.0, 1.0);
  Light2->TurnOn();  Light2->nodename="Light2";


  Polygon  *AxisXX = new Polygon; AxisXX ->nodename= "AxisXX";
  Polygon  *AxisYY = new Polygon; AxisYY ->nodename= "AxisYY";
  Polygon  *AxisZZ = new Polygon; AxisZZ ->nodename= "AxisZZ";
  
  AxisXX->SetVerticesv(vx, 4); AxisXX->SetMaterial(GetColorMat(360*0.0/8.0, 1.0, 1.0 ));  
  AxisYY->SetVerticesv(vy, 4); AxisYY->SetMaterial(GetColorMat(360*2.0/8.0, 1.0, 1.0 ));
  AxisZZ->SetVerticesv(vz, 4); AxisZZ->SetMaterial(GetColorMat(360*4.0/8.0, 1.0, 1.0 ));
  
  StlShape *StlPart01= new StlShape; StlPart01->nodename = "bunny";
  StlPart01 ->LoadStl("bunny.stl");
  StlPart01 ->SetMaterial(GetColorMat(360*7.0/8.0, 1.0, 1.0 ));
  
  //Nodes for Camera:
  Camera *Camera1=new Camera(PERSPECTIVE);

  Camera1->SetValue(POSITION, 2.2, 0.9, 3);
  Camera1->SetValue(AIMAT, 0, 0, 0);
  Camera1->SetValue(UPDIRECTION, 0, 1, 0);
  Camera1->SetValue(ASPECT, 1);
  Camera1->SetValue(NEAR, 0.1);
  Camera1->SetValue(FAR, 200);
  Camera1->SetValue(YANGLE, 50); 
  Camera1->nodename="Camera1";
  
  //Nodes for Robot:
  Material *RobotMat=new Material; RobotMat  ->nodename="Robot_RobotMaterial"; 
  Material *EyeMat  =new Material; EyeMat    ->nodename="Robot_EyeMaterial  "; 
  Material *PaperMat  =new Material; PaperMat    ->nodename="Robot_PaperMaterial  "; 
  
  Cylinder *Base  =new Cylinder;  Base  ->nodename="Robot_Base"; 
  Sphere   *Head  =new Sphere;    Head  ->nodename="Robot_Head"; 
  Sphere   *EyeL  =new Sphere;    EyeL  ->nodename="Robot_EyeL"; 
  Sphere   *EyeR  =new Sphere;    EyeR  ->nodename="Robot_EyeR"; 
  Cylinder *UpperArmL=new Cylinder; UpperArmL->nodename="Robot_UpperArmL";
  Cylinder *UpperArmR=new Cylinder; UpperArmR->nodename="Robot_UpperArmR";
  Cylinder *LowerArmL=new Cylinder; LowerArmL->nodename="Robot_LowerArmL";
  Cylinder *LowerArmR=new Cylinder; LowerArmR->nodename="Robot_LowerArmR";
  Cylinder *UpperLegL=new Cylinder; UpperLegL->nodename="Robot_UpperLegL";
  Cylinder *UpperLegR=new Cylinder; UpperLegR->nodename="Robot_UpperLegR";
  Cylinder *LowerLegL=new Cylinder; LowerLegL->nodename="Robot_LowerLegL";
  Cylinder *LowerLegR=new Cylinder; LowerLegR->nodename="Robot_LowerLegR";
  

  Transformation *EyeLTrans  =new Transformation; EyeLTrans   ->nodename="Robot_TF_EyeLTrans  ";  
  Transformation *EyeRTrans  =new Transformation; EyeRTrans   ->nodename="Robot_TF_EyeRTrans  ";  
  Transformation *HeadTrans  =new Transformation; HeadTrans   ->nodename="Robot_TF_HeadTrans  ";  
  Transformation *UpArmLTrans=new Transformation; UpArmLTrans ->nodename="Robot_TF_UpArmLTrans";  
  Transformation *UpArmRTrans=new Transformation; UpArmRTrans ->nodename="Robot_TF_UpArmRTrans";  
  Transformation *LowArmTrans=new Transformation; LowArmTrans ->nodename="Robot_TF_LowArmTrans";  
  Transformation *UpLegLTrans=new Transformation; UpLegLTrans ->nodename="Robot_TF_UpLegLTrans";  
  Transformation *UpLegRTrans=new Transformation; UpLegRTrans ->nodename="Robot_TF_UpLegRTrans";  
  Transformation *LowLegTrans=new Transformation; LowLegTrans ->nodename="Robot_TF_LowLegTrans"; 
  Transformation *BaseTrans  =new Transformation; BaseTrans   ->nodename="Robot_TF_BaseTrans  "; 

  //Robot Value:
  RobotMat->SetValue(DIFFUSE, 0.0, 0.0, 1.0, 1.0);
  RobotMat->SetValue(AMBIENT, 0.0, 0.0, 1.0, 1.0);
  RobotMat->SetValue(SPECULAR, 1.0, 1.0, 1.0, 1.0);
  RobotMat->SetValue(SHININESS, 100.0);

  EyeMat->SetValue(DIFFUSE, 1.0, 1.0, 1.0, 1.0);
  EyeMat->SetValue(AMBIENT, 1.0, 1.0, 1.0, 1.0);
  EyeMat->SetValue(SPECULAR, 1.0, 1.0, 1.0, 1.0);
  EyeMat->SetValue(SHININESS, 100.0);

  PaperMat  ->SetValue(DIFFUSE, 1.0, 1.0, 1.0, 1.0);    
  PaperMat  ->SetValue(AMBIENT, 1.0, 1.0, 1.0, 1.0);  
  PaperMat  ->SetValue(SPECULAR, 1.0, 1.0, 1.0, 1.0); 
  PaperMat  ->SetValue(SHININESS, 100.0);             
  
  Base->SetValue(HEIGHT, BaseLen);
  Base->SetValue(RADIUS, BaseRadius);
  Head->SetValue(RADIUS, BaseRadius);
  EyeL->SetValue(RADIUS, EyeRadius);
  EyeR->SetValue(RADIUS, EyeRadius);
  UpperArmL->SetValue(HEIGHT, UpLen);
  UpperArmL->SetValue(RADIUS, Radius);
  LowerArmL->SetValue(HEIGHT, LowLen);
  LowerArmL->SetValue(RADIUS, Radius);
  UpperArmR->SetValue(HEIGHT, UpLen);
  UpperArmR->SetValue(RADIUS, Radius);
  LowerArmR->SetValue(HEIGHT, LowLen);
  LowerArmR->SetValue(RADIUS, Radius);
  UpperLegL->SetValue(HEIGHT, UpLen);
  UpperLegL->SetValue(RADIUS, Radius);
  LowerLegL->SetValue(HEIGHT, LowLen);
  LowerLegL->SetValue(RADIUS, Radius);
  UpperLegR->SetValue(HEIGHT, UpLen);
  UpperLegR->SetValue(RADIUS, Radius);
  LowerLegR->SetValue(HEIGHT, LowLen);
  LowerLegR->SetValue(RADIUS, Radius);


  
  EyeLTrans->SetValue(TRANSLATION, BaseRadius-EyeRadius/2, 0, 0, 0);
  EyeLTrans->SetValue(ROTATION, 30, 0, 0, 1, 1);
  EyeRTrans->SetValue(TRANSLATION, BaseRadius-EyeRadius/2, 0, 0, 0);
  EyeRTrans->SetValue(ROTATION, -30, 0, 0, 1, 1);
  HeadTrans->SetValue(TRANSLATION, 0, 0, BaseLen+BaseRadius+BaseRadius/3, 0);
  UpArmLTrans->SetValue(TRANSLATION, 0, 0, -UpLen, 0);
  UpArmLTrans->SetValue(ROTATION, -45, 0, 1, 0, 1);
  UpArmLTrans->SetValue(TRANSLATION, 0, BaseRadius+Radius, BaseLen, 2);
  UpArmRTrans->SetValue(TRANSLATION, 0, 0, -UpLen, 0);
  UpArmRTrans->SetValue(ROTATION, -45, 0, 1, 0, 1);
  UpArmRTrans->SetValue(TRANSLATION, 0, -(BaseRadius+Radius), BaseLen, 2);
  LowArmTrans->SetValue(TRANSLATION, 0, 0, -UpLen, 0);
  LowArmTrans->SetValue(ROTATION, -45, 0, 1, 0, 1);
  UpLegLTrans->SetValue(TRANSLATION, 0, 0, -UpLen, 0);
  UpLegLTrans->SetValue(ROTATION, -90, 0, 1, 0, 1);
  UpLegLTrans->SetValue(TRANSLATION, 0, BaseRadius+Radius, 0, 2);
  UpLegRTrans->SetValue(TRANSLATION, 0, 0, -UpLen, 0);
  UpLegRTrans->SetValue(ROTATION, -100, 0, 1, 0, 1);
  UpLegRTrans->SetValue(TRANSLATION, 0, -(BaseRadius+Radius), 0, 2);
  LowLegTrans->SetValue(TRANSLATION, 0, 0, -UpLen, 0);
  LowLegTrans->SetValue(ROTATION, 95, 0, 1, 0, 1);
  BaseTrans->SetValue(ROTATION, -90, 1, 0, 0, 0);
  BaseTrans->SetValue(ROTATION, -10, 0, 0, 1, 1);

  Head->SetTransform(HeadTrans);
  EyeL->SetTransform(EyeLTrans);
  EyeR->SetTransform(EyeRTrans);
  EyeR->SetMaterial(EyeMat);
  EyeL->SetMaterial(EyeMat);
  UpperArmL->SetTransform(UpArmLTrans);
  LowerArmL->SetTransform(LowArmTrans);
  UpperArmR->SetTransform(UpArmRTrans);
  LowerArmR->SetTransform(LowArmTrans);
  UpperLegL->SetTransform(UpLegLTrans);
  LowerLegL->SetTransform(LowLegTrans);
  UpperLegR->SetTransform(UpLegRTrans);
  LowerLegR->SetTransform(LowLegTrans);
  Base->SetTransform(BaseTrans);

  //AxisXX->SetTransform(ROTATION, 20, 0, 1, 0, 0);
 // AxisXX->SetTransform(TRANSLATION, 0.05, -0.15, -0.2, 1);

  //Set Relationship in Robot:
  RobotMat->AddChild(Light1); 
 // Light1->AddChild(Base);
  Light1->AddChild(StlPart01);
  Light1->AddChild(AxisXX);
  Light1->AddChild(AxisYY);
  Light1->AddChild(AxisZZ);
  
  

  //Nodes for Chair:
  Cube *Seat=new Cube(0.7, 0.06, 0.7);    Seat->nodename= "Chair_Seat";
  Line *Leg1=new Line;                    Leg1->nodename= "Chair_Leg1";
  Line *Leg2=new Line;                    Leg2->nodename= "Chair_Leg2";
  Line *Leg3=new Line;                    Leg3->nodename= "Chair_Leg3";
  Line *Leg4=new Line;                    Leg4->nodename= "Chair_Leg4";
  DrawStyle *LegStyle  =new DrawStyle;    LegStyle  ->nodename="LegStyle  ";
  Color     *ChairColor=new Color;        ChairColor->nodename="ChairColor";
  
  //Chair Value:
  float v1[][3]={{-0.3, 0, 0.3},{-0.35, -1*ChairLegLen, 0.3}};
  float v2[][3]={{0.3, 0, 0.3},{0.35, -1*ChairLegLen, 0.3}};
  float v3[][3]={{0.3, 0, -0.3},{0.35, -1*ChairLegLen, -0.3}};
  float v4[][3]={{-0.3, 0, -0.3},{-0.35, -1*ChairLegLen, -0.3}};

  ChairColor->SetValue(RED);
  Seat->SetTransform(TRANSLATION, 0, -0.15, 0, 0);
  LegStyle->SetValue(LINEWIDTH, 7);
  Leg1->SetStyle(LegStyle);
  Leg1->SetVerticesv(v1);
  Leg2->SetStyle(LegStyle);
  Leg2->SetVerticesv(v2);
  Leg3->SetStyle(LegStyle);
  Leg3->SetVerticesv(v3);
  Leg4->SetStyle(LegStyle);
  Leg4->SetVerticesv(v4);
  Seat->AddChild(Leg1);
  Seat->AddChild(Leg2);
  Seat->AddChild(Leg3);
  Seat->AddChild(Leg4);
  ChairColor->AddChild(Seat);

  //Transformation Nodes for both Robot and Chair:
  Transformation *Trans1=new Transformation;
  Trans1->SetValue(ROTATION, -90,1,0,0,0);
  Trans1->SetValue(TRANSLATION, -0.5, 0, 0, 2);

  //Trans1->AddChild(ChairColor);
  Trans1->AddChild(RobotMat); Trans1->nodename= "father_tf_robot&chair";

  //Root Node:
  Node *Root=new Node;

  Root->AddChild(Trans1);
  Root->AddChild(Camera1);  Root ->nodename="root node";
  
  //Viewer:
  GLViewer *MyViewer=new GLViewer;

  MyViewer->Init(argc, argv);
  MyViewer->SetValue(BACKCOLOR, GREY);
  MyViewer->SetValue(BUFFER, DOUBLE);
  MyViewer->CreateWin("Working Hard", 800, 800);

  MyViewer->Show(Root);
 /* GLViewer *MyViewer2=new GLViewer;
  
  MyViewer2->Init(argc, argv);
  MyViewer2->SetValue(BACKCOLOR, MAGENTA);
  MyViewer2->CreateWin("Working Hard2", 200, 200);
  MyViewer2->SetValue(BUFFER, DOUBLE);

  MyViewer->Show(Root);
  MyViewer2->Show(Root);*/
  return 0;
}


