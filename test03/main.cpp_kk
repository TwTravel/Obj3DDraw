#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <list>
#include <sstream>
#include <iomanip>
#include <locale>
#include <cstdlib>
using namespace std;

#include "readstl.h"


int read_binary_STL_file(std::string STL_filename,std::vector<triangle> & facet,
               double & x_min, double & x_max,
			   double & y_min, double & y_max,
			   double & z_min, double & z_max);
			   
			   
int main(int argc, char* argv[])
{
  std::vector<triangle>  facet;
  double  x_min,  x_max, y_min,  y_max, z_min, z_max;
  
  read_binary_STL_file(argv[1], facet,
         x_min,x_max,y_min,y_max,z_min,z_max);

  printf("%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf\n", 
         x_min,  x_max, y_min,  y_max, z_min, z_max);
  printf("hello\n");

  return 1;
}