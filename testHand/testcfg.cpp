#include <stdio.h>
#include <stdlib.h>
#include "read_cfg.h"



int main(int argc, char *argv[])
{
  printf("hello\n");
  std::vector<std::string> lines;
  
  readlistfile(argv[1],  lines); 
  
  int i;
  
  
  
  for( i = 0; i < lines.size(); i++ )
  {
	  printf( "%s\n", (char*) lines[i].c_str() );
  }
    	
  return 1;
}
