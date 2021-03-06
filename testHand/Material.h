//Material.h
#ifndef MAT_H
#define MAT_H

#include "Node.h"

class Material: public Node
{
 public:
  Material();
  void SetValuev(Enum Pname, float *);
  void SetValue(Enum Pname, float v1, float v2, float v3, float v4);
  void SetValue(Enum Pname, float Value);
  void Render();
  void Add();
 private:
  bool Changed[5];
  float Ambient[4];
  float Diffuse[4];
  float Specular[4];
  float Emission[4];
  float Shininess;
  
};

struct COLOUR
{ double r,g,b,h,s,v;
};

COLOUR HSV2RGB(COLOUR c1)
{
   COLOUR c2,sat;

   while (c1.h < 0)
      c1.h += 360.0;
   while (c1.h > 360.0)
      c1.h -= 360.0;

   if (c1.h < 120.0) {
      sat.r = (120.0 - c1.h) / 60.0;
      sat.g = c1.h / 60.0;
      sat.b = 0;
   } else if (c1.h < 240.0) {
      sat.r = 0;
      sat.g = (240.0 - c1.h) / 60.0;
      sat.b = (c1.h - 120.0) / 60.0;
   } else {
      sat.r = (c1.h - 240.0) / 60.0;
      sat.g = 0;
      sat.b = (360.0 - c1.h) / 60.0;
   }
   sat.r = std::min(sat.r,1.0);
   sat.g = std::min(sat.g,1.0);
   sat.b = std::min(sat.b,1.0);

   c2.r = (1.0 - c1.s + c1.s * sat.r) * c1.v;
   c2.g = (1.0 - c1.s + c1.s * sat.g) * c1.v;
   c2.b = (1.0 - c1.s + c1.s * sat.b) * c1.v;

   return(c2);
}
/*
   Calculate HSV from RGB
   Hue is in degrees
   Lightness is betweeen 0 and 1
   Saturation is between 0 and 1
*/
COLOUR RGB2HSV(COLOUR c1)
{
   double themin,themax,delta;
   COLOUR c2;

   themin = std::min(c1.r,std::min(c1.g,c1.b));
   themax = std::max(c1.r,std::max(c1.g,c1.b));
   delta = themax - themin;
   c2.v = themax;
   c2.s = 0;
   if (themax > 0)
      c2.s = delta / themax;
   c2.h = 0;
   if (delta > 0) {
      if (themax == c1.r && themax != c1.g)
         c2.h += (c1.g - c1.b) / delta;
      if (themax == c1.g && themax != c1.b)
         c2.h += (2 + (c1.b - c1.r) / delta);
      if (themax == c1.b && themax != c1.r)
         c2.h += (4 + (c1.r - c1.g) / delta);
      c2.h *= 60.0;
   }
   return(c2);
}

inline void Material::Add()
{
   float color_r,color_g,color_b;
   COLOUR color;
   
   color.r = Ambient[0];
   color.g = Ambient[1];
   color.b = Ambient[2]; 
   color = RGB2HSV(color); color.h +=1.0;   
   color = HSV2RGB(color);
   
  
  // color_r += 0.001; if(color_r>1.0) color_r = 0;
  // color_g += 0.001; if(color_g>1.0) color_g = 0;
 //  color_b += 0.001; if(color_b>1.0) color_b = 0;
   Ambient[0] = color.r;
   Ambient[1] = color.g;
   Ambient[2] = color.b;
   Diffuse[0] = Ambient[0]; 
   Diffuse[1] = Ambient[1];
   Diffuse[2] = Ambient[2];
   
  /* color_r = Ambient[0]*255.0;
   color_g = Ambient[1]*255.0;
   color_b = Ambient[2]*255.0;
   color_r +=1.0; if(color_r>255) color_r = 0;
   color_g +=1.0; if(color_g>255) color_g = 0;
   color_b +=1.0; if(color_b>255) color_b = 0;
   Ambient[0] = float(color_r)/255.0;
   Ambient[1] = float(color_g)/255.0;
   Ambient[2] = float(color_b)/255.0;
   Diffuse[0] = Ambient[0]; 
   Diffuse[1] = Ambient[1];
   Diffuse[2] = Ambient[2];*/
   
}

inline Material::Material()
{
  int i;

  for(i=0; i<5; i++)
    {
      Changed[i]=false;
    }
}

inline void 
Material::SetValuev(Enum Pname, float Value[])
{
  float *temp=NULL;
  int i;

  switch(Pname)
    {
    case AMBIENT:
      temp=Ambient;
      Changed[0]=true;
      break;
    case DIFFUSE:
      temp=Diffuse;
      Changed[1]=true;
      break;
    case SPECULAR:
      temp=Specular;
      Changed[2]=true;
      break;
    case EMISSION:
      temp=Emission;
      Changed[3]=true;
      break;
    default:
      break;
    }
  
  for(i=0; i<4; i++)
    temp[i]=Value[i];
}

inline void 
Material::SetValue(Enum Pname, float v1, float v2, float v3, float v4)
{
  float temp[]={v1, v2, v3, v4};
  
  SetValuev(Pname, temp);
}

inline void
Material::SetValue(Enum Pname, float Value)
{
  if(Pname==SHININESS)
    {
      Changed[4]=true;
      Shininess=Value;
    }
}

inline void
Material::Render()
{ printf("%s *name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  if(Changed[0])
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
  if(Changed[1])
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
  if(Changed[2])
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
  if(Changed[3])
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
  if(Changed[4])
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
}

#endif
