//Geometry.h
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Node.h"
#include "Color.h"
#include "Material.h"
#include "Transformation.h"
#include "DrawStyle.h"
#include "readstl.h"

class Geometry: public Node
{
 public:
  Geometry();
  ~Geometry();

  void SetColor(Enum);
  void SetColor(float, float, float);
  void SetColorv(float *);
  void SetColor(Color *);
  void SetMaterial(Enum, float, float, float, float);
  void SetMaterialv(Enum, float *);
  void SetMaterial(Enum, float);
  void SetMaterial(Material *);
  void SetTransform(Enum, float *, int);
  void SetTransform(Enum, float, float, float, int);
  void SetTransform(Enum, float, float, float, float, int);
  void SetTransform(Transformation *);
  void SetStyle(Enum, Enum);
  void SetStyle(Enum, float);
  void SetStyle(DrawStyle *);
  virtual void Render();

 protected:
  Color *ColorNode;
  Material *MatNode;
  Transformation *TransNode;
  DrawStyle *StyleNode;
};



inline Geometry::Geometry()
{
  ColorNode=NULL;
  MatNode=NULL;
  TransNode=NULL;
  StyleNode=NULL;
}

inline Geometry::~Geometry()
{
  if(ColorNode)
    {
      delete ColorNode;
      ColorNode=NULL;
    }
  if(MatNode)
    {
      delete MatNode;
      MatNode=NULL;
    }
  if(TransNode)
    {
      delete TransNode;
      TransNode=NULL;
    }
  if(StyleNode)
    {
      delete StyleNode;
      StyleNode=NULL;
    }
}

inline void Geometry::SetColor(Enum C)
{
  if(ColorNode==NULL)
    ColorNode=new Color;

  ColorNode->SetValue(C);
}

inline void 
Geometry::SetColor(float v1, float v2, float v3)
{
  if(ColorNode==NULL)
    ColorNode=new Color;

  ColorNode->SetValue(v1, v2, v3);
}

inline  void 
Geometry::SetColorv(float *v)
{
  if(ColorNode==NULL)
    ColorNode=new Color;

  ColorNode->SetValuev(v);
}

inline  void 
Geometry::SetColor(Color *C)
{
  if(ColorNode!=NULL)
    delete ColorNode;

  ColorNode=C;
}

inline  void 
Geometry::SetMaterial(Enum PName, float v1, float v2, float v3, float v4)
{
  if(MatNode==NULL)
    MatNode=new Material;

  MatNode->SetValue(PName, v1, v2, v3, v4);
}

inline  void 
Geometry::SetMaterialv(Enum PName, float *v)
{
  if(MatNode==NULL)
    MatNode=new Material;

  MatNode->SetValuev(PName, v);
}

inline  void 
Geometry::SetMaterial(Enum PName, float v)
{
  if(MatNode==NULL)
    MatNode=new Material;

  MatNode->SetValue(PName, v);
}

inline  void 
Geometry::SetMaterial(Material *M)
{
  if(MatNode!=NULL)
    delete MatNode;
  M->ParentNode = (this);
  MatNode=M;
}

inline  void 
Geometry::SetTransform(Enum Pname, float *v, int Order)
{
  if(TransNode==NULL)
    TransNode=new Transformation;

  TransNode->SetValuev(Pname, v, Order);
}

inline  void 
Geometry::SetTransform(Enum Pname, float x, float y, float z, int Order)
{
  if(TransNode==NULL)
    TransNode=new Transformation;

  TransNode->SetValue(Pname, x, y, z, Order);
}

inline  void 
Geometry::SetTransform(Enum Pname, float a, float x, float y, float z, 
               int Order)
{
  if(TransNode==NULL)
    TransNode=new Transformation;

  TransNode->SetValue(Pname, a, x, y, z, Order);
}


inline  void 
Geometry::SetTransform(Transformation *T)
{
  if(TransNode!=NULL)
    delete TransNode;

  T->ParentNode = (this);
  TransNode=T;
}

inline void
Geometry::SetStyle(Enum Pname, Enum v)
{
  if(StyleNode==NULL)
    StyleNode=new DrawStyle;

  StyleNode->SetValue(Pname, v);  
}

inline void
Geometry::SetStyle(Enum Pname, float v)
{
  if(StyleNode==NULL)
    StyleNode=new DrawStyle;

  StyleNode->SetValue(Pname, v);
}

inline void
Geometry::SetStyle(DrawStyle *S)
{
  if(StyleNode==NULL)
    StyleNode=new DrawStyle;

  StyleNode=S;  
}

inline  void 
Geometry::Render()
{
    printf("%s name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
}

class Line: public Geometry
{
 public:
  Line(){};
  void SetVertices(float *, float *);
  void SetVerticesv(float v[][3]);
  void Render();

 private:
  float Vertices[2][3];
};


inline void
Line::SetVertices(float *v1, float *v2)
{
  int i;
  for(i=0; i<3; i++)
    {
      Vertices[0][i]=v1[i];
      Vertices[1][i]=v2[i];
    }
}

inline void
Line::SetVerticesv(float v[][3])
{
  SetVertices(v[0], v[1]);
}

inline void
Line::Render()
{
  printf("%s name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  //glPushAttrib KH(GL_ALL_ATTRIB_BITS);

  if(ColorNode)
    ColorNode->Render();
  if(MatNode)
  {MatNode->nodespace = ParentNode->nodespace +"</-/>"; MatNode->Render();}
  if(TransNode)
    TransNode->Render();
  if(StyleNode)
    {StyleNode->nodespace = ParentNode->nodespace +"</-/>"; StyleNode->Render();}

  glBegin(GL_LINES);
    glVertex3fv(Vertices[0]);
    glVertex3fv(Vertices[1]);
  glEnd();  
    
  //glPopAttrib KH();
  
}


class Cube:public Geometry
{
 public:
  Cube(){};
  Cube(float, float, float);
  void SetValue(Enum PName, float v);
  void Render();

 private:
  //  void Polygon(int a, int b, int c, int d);
  void Polygon(GLubyte *Index);
  float Height;
  float Width;
  float Depth;

};


Cube::Cube(float w, float h, float d)
{

  Width=w;
  Height=h;
  Depth=d;
}

inline void
Cube::SetValue(Enum PName, float v)
{
  switch(PName)
    {
    case WIDTH:
      Width=v;
      break;
    case HEIGHT:
      Height=v;
      break;
    case DEPTH:
      Depth=v;
      break;
    default:
      break;
    }
}


inline void
Cube::Polygon(GLubyte *Index)
{

  float x1=(-1)*(Width/2);
  float x2=(Width/2);
  float y1=(-1)*(Height/2);
  float y2=(Height/2);
  float z1=(-1)*(Depth/2);
  float z2=(Depth/2);

  
  GLfloat V[][3]={{x1, y1, z2},{x1, y2, z2},{x2, y2, z2},{x2, y1, z2},
          {x1, y1, z1},{x1, y2, z1},{x2, y2, z1},{x2, y1, z1}};

  glBegin(GL_POLYGON);
    glVertex3fv(V[Index[0]]);
    glVertex3fv(V[Index[1]]);
    glVertex3fv(V[Index[2]]);
    glVertex3fv(V[Index[3]]);
  glEnd();

}

inline void
Cube::Render()
{
  printf("%s name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  int i;
  //glPushAttrib KH(GL_ALL_ATTRIB_BITS);

  if(ColorNode)
    ColorNode->Render();
  if(MatNode)
    {MatNode->nodespace = ParentNode->nodespace +"</-/>"; MatNode->Render();}
  if(TransNode)
    TransNode->Render();
  if(StyleNode)
    {StyleNode->nodespace = ParentNode->nodespace +"</-/>"; StyleNode->Render();}

  GLubyte CubeIndex[]={0, 3, 2, 1, 2, 3, 7, 6, 3, 0, 4, 7, 1, 2, 6, 5,
               4, 5, 6, 7, 5, 4, 0, 1};

  GLfloat N[][3]={{0, 0, 1},{1, 0, 0},{0, -1, 0},{0, 1, 0},
          {0, 0, -1},{-1, 0, 0}};

  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //glEnable(GL_COLOR_MATERIAL);
  printf("%s name:%s Cube_Draw_Polygon\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  //glutSolidSphere(1.0, 40, 40);
  for(i=0; i<6; i++)
    {
      glNormal3fv(N[i]);
      Polygon(&CubeIndex[i*4]);      
    }
  /*    
  Polygon(0, 3, 2, 1);
  Polygon(2, 3, 7, 6);
  Polygon(3, 0, 4, 7);
  Polygon(1, 2, 6, 5);
  Polygon(4, 5, 6, 7);
  Polygon(5, 4, 0, 1);
  */

  //glPopAttrib KH();
}



class Cylinder: public Geometry
{
 public:
  Cylinder(){};
  void SetValue(Enum, float);
  void Render();

 private:
  float Height;
  float Radius;
};


inline void
Cylinder::SetValue(Enum Pname, float v)
{
  switch(Pname)
    {
    case HEIGHT:
      Height=v;
      break;
    case RADIUS:
      Radius=v;
      break;
    default:
      break;
    }
}

inline void
Cylinder::Render()
{
  printf("%s *name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  //glPushAttrib KH(GL_ALL_ATTRIB_BITS);

  if(ColorNode)
    ColorNode->Render();
  if(MatNode)
    {MatNode->nodespace = ParentNode->nodespace +"</-/>"; MatNode->Render();}
  if(TransNode)
    TransNode->Render();
  if(StyleNode)
    {StyleNode->nodespace = ParentNode->nodespace +"</-/>"; StyleNode->Render();}

  GLUquadricObj *CylinderObj;
  //KH CylinderObj=gluNewQuadric();
  //KH gluCylinder(CylinderObj, Radius, Radius, Height, 30, 30);
   
  //glPopAttrib KH();
}





class Sphere: public Geometry
{
 public:
  Sphere(){};
  Sphere(float R);
  void SetValue(Enum Pname, float v);
  void Render();

 private:
  float Radius;
};

inline Sphere::Sphere(float R)
{
  Radius=R;
}

inline void
Sphere::SetValue(Enum Pname, float v)
{
  if(Pname==RADIUS)
    Radius=v;
}

inline void
Sphere::Render()
{
  printf("%s name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  //glPushAttrib KH(GL_ALL_ATTRIB_BITS);

  if(ColorNode)
    ColorNode->Render();
  if(MatNode)
    {MatNode->nodespace = ParentNode->nodespace +"</-/>"; MatNode->Render();}
  if(TransNode)
    TransNode->Render();
  if(StyleNode)
    {StyleNode->nodespace = ParentNode->nodespace +"</-/>"; StyleNode->Render();}
  
  printf("%s<-*-> name:%s glutSolidSphere\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  //KH glutSolidSphere(Radius, 40, 40);

  //glPopAttrib KH();
}

class Polygon: public Geometry
{
 public:
  Polygon(){};
  void SetVerticesv(float v[][3], int);
  void Render();

 private:
  float Vertices[1000][3];
  int Size;
};

inline void
Polygon::SetVerticesv(float v[][3], int S)
{
  int i, j;
  Size=S;

  for(i=0; i<S; i++)
    for(j=0; j<3; j++)
      Vertices[i][j]=v[i][j];
}

inline void
Polygon::Render()
{
  int i;
  printf("%s name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  //glPushAttrib KH(GL_ALL_ATTRIB_BITS);

  if(ColorNode)
    ColorNode->Render();
  if(MatNode)
    {MatNode->nodespace = ParentNode->nodespace +"</-/>"; MatNode->Render();}
  if(TransNode)
    TransNode->Render();
  if(StyleNode)
    {StyleNode->nodespace = ParentNode->nodespace +"</-/>"; StyleNode->Render();}

  printf("%s name draw GL_POLYGONs:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  glBegin(GL_POLYGON);
  for(i=0; i<Size; i++)
    glVertex3fv(Vertices[i]);
  glEnd();  
  
  //glPopAttrib KH();
  
}

//=============================================================
//=============================================================
/*
   Calculate RGB from HSV, reverse of RGB2HSV()
   Hue is in degrees
   Lightness is between 0 and 1
   Saturation is between 0 and 1
*/

Material *GetColorMat(double c1_h, double c1_s, double c1_v)
{
	double sat_r, sat_g, sat_b;

   while (c1_h < 0)
      c1_h += 360;
   while (c1_h > 360)
      c1_h -= 360;

   if (c1_h < 120) {
      sat_r = (120 - c1_h) / 60.0;
      sat_g = c1_h / 60.0;
      sat_b = 0;
   } else if (c1_h < 240) {
      sat_r = 0;
      sat_g = (240 - c1_h) / 60.0;
      sat_b = (c1_h - 120) / 60.0;
   } else {
      sat_r = (c1_h - 240) / 60.0;
      sat_g = 0;
      sat_b = (360 - c1_h) / 60.0;
   }
   sat_r = std::min(sat_r,1.0);
   sat_g = std::min(sat_g,1.0);
   sat_b = std::min(sat_b,1.0);

   sat_r = (1 - c1_s + c1_s * sat_r) * c1_v;
   sat_g = (1 - c1_s + c1_s * sat_g) * c1_v;
   sat_b = (1 - c1_s + c1_s * sat_b) * c1_v;
   //==================================================
   Material *RobotMat=new Material;
   //Robot Value:
   RobotMat->SetValue(DIFFUSE, sat_r, sat_g, sat_b, 1.0);
   RobotMat->SetValue(AMBIENT, sat_r, sat_g, sat_b, 1.0);
   RobotMat->SetValue(SPECULAR, 1.0, 1.0, 1.0, 1.0);
   RobotMat->SetValue(SHININESS, 100.0);
   return RobotMat;
}

/*struct COLOUR
{
};

struct HSV
{
};

COLOUR HSV2RGB(HSV c1)
{
  

   return(c2);
}*/

class StlShape: public Geometry
{
 public:
  StlShape(){};
  void LoadStl(char*file);
  void SetVerticesv();
  void Render();
  std::vector<triangle>  facet;
 private:
  float Vertices[1000][3];
  int Size;
};

inline void StlShape::LoadStl(char*file)
{
 // getStlGeometryInput
 
  double  x_min,  x_max, y_min,  y_max, z_min, z_max;
  
   getStlGeometryInput(file, facet,
         x_min,x_max,y_min,y_max,z_min,z_max);

  printf("%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf\n", 
         x_min,  x_max, y_min,  y_max, z_min, z_max);
}

inline void
StlShape::SetVerticesv()
{
  /*int i, j;
  Size = facet.size();

  for(i=0; i<S; i++)
    for(j=0; j<3; j++)
      Vertices[i][j]= facet.point[i][j];*/
}

inline void
StlShape::Render()
{
  int i;
  printf("%s name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  //glPushAttrib KH(GL_ALL_ATTRIB_BITS);

  if(ColorNode)
    ColorNode->Render();
  if(MatNode)
    {MatNode->nodespace = ParentNode->nodespace +"</-/>"; MatNode->Render();}
  if(TransNode)
    TransNode->Render();
  if(StyleNode)
    {StyleNode->nodespace = ParentNode->nodespace +"</-/>"; StyleNode->Render();}

  printf("%s name draw GL_POLYGONs:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());

 /*
 class triangle
{
public:
    /// 3 components of the normal vector to the triangle
    vec3d normal;
    /// 3 coordinates of the three vertices of the triangle
    vec3d point[3];
};
 */
  //glScalef(4, 4, 4);
  glBegin(GL_TRIANGLES);
  for(int i = 0; i < facet.size(); i++)
  {
        // 遍历三角形的所有点
      for(int whichVertex = 0; whichVertex < 3; whichVertex++ )
       {
        // 给出法向量
      glNormal3f(facet[i].normal.x, facet[i].normal.y, facet[i].normal.z);
        // 如果对象具有纹理
        glVertex3f( facet[i].point[whichVertex].x , 
		            facet[i].point[whichVertex].y , 
				    facet[i].point[whichVertex].z ); /* */
		/*glNormal3f(facet[i].normal.z,facet[i].normal.x, facet[i].normal.y);
        // 如果对象具有纹理
        glVertex3f( facet[i].point[whichVertex].z ,
		            facet[i].point[whichVertex].x , 
		            facet[i].point[whichVertex].y 
				     );*/
       }
   }
  glEnd();
  /*glBegin(GL_POLYGON);
  for(i=0; i<Size; i++)
    glVertex3fv(Vertices[i]);
  glEnd();  */
  
  //glPopAttrib KH();
  
}

#endif
