//Transformation.h
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Node.h"

class Transformation: public Node
{
 public:
  Transformation();
  void SetValuev(Enum Pname, float *, int Order);
  void SetValue(Enum Pname, float x, float y, float z, int Order);
  void SetValue(Enum Pname, float a, float x, float y, float z, int Order);
  void Render();

 private:
  int OpNum;
  int TransformOrder[10];
  float Transform[10][4];
  //float Translation[10][3];
  //float Rotation[10][4];
  //float Scale[10][3];

};

inline Transformation::Transformation()
{
  int i;

  OpNum=0;
  for(i=0; i<10; i++)
    TransformOrder[i]=-1;
}

inline void 
Transformation::SetValuev(Enum Pname, float *Value, int Order)
{
  //float *temp=NULL;
  int i;

  if(OpNum<Order+1)
    OpNum=Order+1;

  switch(Pname)
    {
    case TRANSLATION:
      TransformOrder[Order]=TRANSLATION;
      //temp=Translation;
      break;
    case ROTATION:
      TransformOrder[Order]=ROTATION;
      //temp=Rotation;
      break;
    case SCALE:
      TransformOrder[Order]=SCALE;  
      //temp=Scale;
      break;
    default:
      break;
    }

  for(i=0; i<3; i++)
    Transform[Order][i]=Value[i];
  if(Pname==ROTATION)
    Transform[Order][3]=Value[3];
}

inline void
Transformation::SetValue(Enum Pname, float x, float y, float z, int Order)
{
  float temp[]={x, y, z};

  SetValuev(Pname, temp, Order);
}

inline void 
Transformation::SetValue(Enum Pname, float a, float x, float y, float z, 
			 int Order)
{
  float temp[]={a, x, y, z};

  SetValuev(Pname, temp, Order);
}

inline void
Transformation::Render()
{
  
  int i;
  if(ParentNode!=NULL)
     nodespace = ParentNode->nodespace +"<--->";
  for(i=OpNum-1; i>=0; i--)
    {
      switch(TransformOrder[i])
	{
	case TRANSLATION:
	  printf("%s name:%s, TRANSLATION:%.2f,%.2f,%.2f\n",(char*)nodespace.c_str(),(char*)nodename.c_str(),
	         Transform[i][0], Transform[i][1], Transform[i][2]);
	  glTranslatef(Transform[i][0], Transform[i][1], Transform[i][2]);
	  break;
	case ROTATION:
	  printf("%s name:%s, ROTATION:%.2f,%.2f,%.2f\n",(char*)nodespace.c_str(),(char*)nodename.c_str(),
	        Transform[i][0], Transform[i][1], Transform[i][2]);
	  glRotatef(Transform[i][0], Transform[i][1], Transform[i][2], 
		    Transform[i][3]);
	  break;
	case SCALE:
	  printf("%s name:%s, SCALE:%.2f,%.2f,%.2f\n",(char*)nodespace.c_str(),(char*)nodename.c_str(),
	        Transform[i][0], Transform[i][1], Transform[i][2]);
	  glScalef(Transform[i][0], Transform[i][1], Transform[i][2]);
	  break;
	default:
	  break;
	}
    }
}

#endif
