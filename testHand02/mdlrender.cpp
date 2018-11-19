#include <stdio.h>
#include <stdlib.h>
#include "read_cfg.h"
#include "Scene.h"
#include <vector>
#include <map>

using namespace std;

 string trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}

int main(int argc, char *argv[])
{
  printf("hello\n");
  std::vector<std::string> lines;
  
  readlistfile(argv[1],  lines); 
  
  /*CAMERA_POS, 2.2, 0.9, 3, 50 , #posx posy posz angle
   SYSROTATION, -90,  1, 0 ,0, 0
   SYSTRANSLTN, -0.5, 0, 0, 2, 0
bunny.stl, 10.0, 1.0, 1.0, 0, 0, 0, 0, 0, 0, 0, 4 , #OBJ(color3)(pos3)(rot4)(scale1)
bunny.stl, 40.0, 1.0, 1.0, -10, 0, 0, 0, 0, 0, 0, 2,#OBJ(color3)(pos3)(rot4)(scale1)  
bunny.stl, 50.0, 1.0, 1.0, 10, 0, 0, 0, 0, 0,  0,  3,#OBJ(color3)(pos3)(rot4)(scale1)   
  */
  
    float vx[][3]={{0,  -0.02, 0.0},{100, -0.02, 0.0},
		         {100, 0.02, 0.0},{  0,  0.02, 0.0} };

    float vy[][3]={{-0.02,  0, 0.0},{ -0.02, 100, 0.0},
		         {0.02 ,100, 0.0},{  0.02,   0, 0.0} };		
		
    float vz[][3]={{-0.02,  0, 0.0},{ -0.02,  0, 100.0},
		         {0.02 ,0, 100.0},{  0.02,   0, 0.0} };	
  
   int i;
   double  camx, camy, camz, campov, lookatx, lookaty, lookatz;
   double  rot_angle,rot_x,rot_y,rot_z;
   double  trans_x,trans_y,trans_z;
   
   std::vector<std::string> strvec;
 
//##################################################################
//################################################################## 
   strvec.clear(); splitString(lines[0], strvec, ",");
   
   camx   = atof( trim(strvec[1]).c_str());
   camy   = atof( trim(strvec[2]).c_str() );
   camz   = atof( trim(strvec[3]).c_str() );
   campov = atof( trim(strvec[4]).c_str() );  
   
   lookatx = atof( trim(strvec[5]).c_str() );  
   lookaty = atof( trim(strvec[6]).c_str() ); 
   lookatz = atof( trim(strvec[7]).c_str() ); 
   
   Camera *Camera1=new Camera(PERSPECTIVE);
   
   Camera1->SetValue(POSITION, camx, camy, camz);
   Camera1->SetValue(AIMAT,  lookatx, lookaty, lookatz);
   Camera1->SetValue(UPDIRECTION, 0, 1, 0);
   Camera1->SetValue(ASPECT, 1);
   Camera1->SetValue(NEAR, 0.1);
   Camera1->SetValue(FAR, 200);
   Camera1->SetValue(YANGLE, campov); 
   Camera1->nodename="Camera1";
   
//##################################################################
//##################################################################  
   strvec.clear(); splitString(lines[1], strvec, ","); 
   rot_angle   = atof( trim(strvec[1]).c_str() );
   rot_x       = atof( trim(strvec[2]).c_str() );
   rot_y       = atof( trim(strvec[3]).c_str() );
   rot_z       = atof( trim(strvec[4]).c_str() ); 
   strvec.clear(); splitString(lines[2], strvec, ","); 
   trans_x = atof( trim(strvec[1]).c_str() );
   trans_y = atof( trim(strvec[2]).c_str() );
   trans_z = atof( trim(strvec[3]).c_str() );
   
   Transformation *SysTrans = new Transformation;
   
   SysTrans->SetValue(ROTATION, rot_angle,rot_x, rot_y, rot_z,0);
   SysTrans->SetValue(TRANSLATION, trans_x,  trans_y, trans_z, 2);
//##################################################################
//##################################################################  
   int element_num = lines.size() - 3 ;
  
//   Transformation *StlTrans    = new Transformation[ element_num ];
   Light *Light1=new Light;
   Light1->SetValue(POSITION,  2, -1,  1.8, 1);
   Light1->SetValue(SPOT_DIRECTION, -2, 1, -1.8);
   Light1->SetValue(CUTOFFANGLE, 180.0);
   Light1->TurnOn();     Light1->nodename="Light1";
  
   Polygon  *AxisXX = new Polygon; AxisXX ->nodename= "AxisXX";
   Polygon  *AxisYY = new Polygon; AxisYY ->nodename= "AxisYY";
   Polygon  *AxisZZ = new Polygon; AxisZZ ->nodename= "AxisZZ";
  
   AxisXX->SetVerticesv(vx, 4); AxisXX->SetMaterial(GetColorMat(360*0.0/8.0, 1.0, 1.0 ));  
   AxisYY->SetVerticesv(vy, 4); AxisYY->SetMaterial(GetColorMat(360*2.0/8.0, 1.0, 1.0 ));
   AxisZZ->SetVerticesv(vz, 4); AxisZZ->SetMaterial(GetColorMat(360*4.0/8.0, 1.0, 1.0 ));
   
     TextureSurface* groundface = new TextureSurface;
   
   float vground[][3]={{-8,  -2, -1.2},{  1.5,   -2,  -1.2},
		                    {1.5,  5, -1.2},{  -8,   5,  -1.2} };
				 
  /* float vground[][3]={{-200,  200, 0.0},{-200, -200, 0.0},
		               { 200,  -200, 0.0},{ 200, 200, 0.0} };*/
   groundface->SetVerticesv(vground,4);
   // groundface->SetVerticesv(vx, 4);
   groundface->SetMaterial(GetColorMat(360*0.0/8.0, 1.0, 1.0 ));
   Light1->AddChild(groundface);
   
  
   vector<StlShape*>        stl_shape_vec;
   vector<Transformation*>  stl_tranf_vec;
   Material *MatArray[200];
   
   map<string, StlShape *> model_dictionary;
   // StlShape       *StlElements = new StlShape[ element_num ];
   //Transformation *StlTrans    = new Transformation[ element_num ];
   for( i = 0; i <  element_num; i++ )
   {
	   strvec.clear(); splitString(lines[3+i], strvec, ",");
	   
	   double  mat_h, mat_s, mat_v;
	   double  obj_x, obj_y, obj_z;
       double  rot_angle, rot_x, rot_y, rot_z , obj_scale;	   
	   string  fname;
	   fname = strvec[0+2];
	   mat_h = atof(trim(strvec[1+2]).c_str());
	   mat_s = atof(trim(strvec[2+2]).c_str());
	   mat_v = atof(trim(strvec[3+2]).c_str());
	   
	   obj_x = atof(trim(strvec[4+2]).c_str());
	   obj_y = atof(trim(strvec[5+2]).c_str());
	   obj_z = atof(trim(strvec[6+2]).c_str());
	   
	   rot_angle =  atof(trim(strvec[ 7+2]).c_str());
	   rot_x     =  atof(trim(strvec[ 8+2]).c_str());
	   rot_y     =  atof(trim(strvec[ 9+2]).c_str());
	   rot_z     =  atof(trim(strvec[10+2]).c_str());
	   obj_scale =  atof(trim(strvec[11+2]).c_str()); 
	   
	   printf("##########################trans: %.2f,  %.2f,  %.2f\n", obj_x ,    obj_y,  obj_z);
	   
	   StlShape      *StlElement = new StlShape;
	   Transformation*StlTrans   = new Transformation;
	   
	   stl_tranf_vec.push_back(StlTrans);
	   stl_shape_vec.push_back(StlElement);
	   StlElement->LoadStl((char*)trim(fname).c_str());
	   StlElement->nodename   = trim(strvec[0]);
	   StlElement->parentname = trim(strvec[1]);
	   
	   model_dictionary[StlElement->nodename] = StlElement;
	    
	   StlTrans->SetValue(ROTATION    , rot_angle, rot_x,  rot_y, rot_z, 0);
	   StlTrans->SetValue(TRANSLATION , obj_x ,    obj_y,  obj_z  , 1);
	   //StlTrans->SetValue(SCALE       , obj_scale, obj_scale, obj_scale,2); 
	   StlElement->scale = obj_scale;
	   StlElement->SetTransform(StlTrans);
	   MatArray[i] = GetColorMat(mat_h, mat_s, mat_v );
     StlElement->SetMaterial( MatArray[i] );  //GetColorMat(360*0.0/8.0, 1.0, 1.0 ));//
       
	   //SysTrans->AddChild(&StlElements[i]);
	   printf( "%s\n", trim(lines[i]).c_str() );
   }
  
  for( i=0; i< stl_shape_vec.size();i++)
  {
	  if(stl_shape_vec[i]->parentname == "SYS_COORD")
	  {
		  SysTrans->AddChild(stl_shape_vec[i]);
	  }
	  else
	  {
		 model_dictionary[stl_shape_vec[i]->parentname ]->AddChild(stl_shape_vec[i]);
	  }
  }
  
  Light1->AddChild(AxisXX);
  Light1->AddChild(AxisYY);
  Light1->AddChild(AxisZZ);
  
  Node *Root=new Node;
  
  for(i=0;i<30;i++)
  {
	  Root->node_func_buffer[i] =  MatArray[i];
  }
  
  /*Root->node_func_buffer[0] =  MatArray[0];
  Root->node_func_buffer[1] =  MatArray[1];
  Root->node_func_buffer[2] =  MatArray[2];
  Root->node_func_buffer[3] =  MatArray[3];
  Root->node_func_buffer[4] =  MatArray[4];
  Root->node_func_buffer[5] =  MatArray[5];
  Root->node_func_buffer[6] =  MatArray[6];*/   
  
  Root->node_func_buffer[30+0]  = stl_tranf_vec[1];
  Root->node_func_buffer[30+1]  = stl_tranf_vec[2];
  Root->node_func_buffer[30+2]  = stl_tranf_vec[3];
  Root->node_func_buffer[30+3]  = stl_tranf_vec[4];
  Root->node_func_buffer[30+4]  = stl_tranf_vec[5];
  
  
  Root->AddChild(SysTrans);
  SysTrans->AddChild(Light1);
  
  //Root->AddChild(Light1);
  //Light1->AddChild(SysTrans);
  Root->AddChild(Camera1); Root ->nodename="root node";
  
  //Viewer:
  GLViewer *MyViewer=new GLViewer;

  MyViewer->Init(argc, argv);
  MyViewer->SetValue(BACKCOLOR, GREY);
  MyViewer->SetValue(BUFFER, DOUBLE);
  MyViewer->CreateWin("Working Hard", 800, 800);

  MyViewer->Show(Root);
   
  return 1;
}
