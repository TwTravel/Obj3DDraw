#ifndef READ_CONFIG_HEAD
#define READ_CONFIG_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <math.h>
#include <string.h>
#include "c24bitmap.h"
#include "c256bitmap.h"
#include <algorithm>
using namespace std;
 
//######################################################################################

#include <fstream>
#include <iostream>

 

bool inline readFile(const char* path, std::string& content)
{
	content.clear();
	std::ifstream fin;
	fin.open(path, std::ios::binary);
	if (!fin) {
		return false;
	}
	const int LINE_LENGTH = 100000;
	char str[LINE_LENGTH];
	while(fin.getline(str,LINE_LENGTH)) {
		content.append(str);
		content.append("\n");
	}
	return true;
}

void inline splitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while(std::string::npos != pos2) {
		string item = s.substr(pos1, pos2-pos1);

		if( item.size()>1&& item[item.length()-1] == '\r')
			item = item.substr(0, item.length()-1);

		if(item.size()>0)
			v.push_back(item);
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length()) {
		v.push_back(s.substr(pos1));
	}
}

string inline getpathfilename(string fname)
{
	int i;
	for(i= (fname.length()-1); i>=0; i--) {
		if(fname[i] == '/')
			return fname.substr( i+1 );
	}
	return fname;
}

void inline readlistfile(const char* filename, std::vector<std::string>&lines)
{
	std::string content;
	bool ret = readFile(filename, content);
	if (ret == false) {
		std::cout << "Read File Failed File:" << filename << std::endl;
	}
	splitString(content, lines, "\n");
	
}
//######################################################################################
//######################################################################################
void inline getfiledata(char * filename, std::vector<char> & data)
{
// data.clear();

	FILE* finput = fopen(filename, "rb");
	if(finput==0)
		return ;
	fseek (finput, 0, SEEK_END);
	long filelen = ftell(finput);
	//printf("%d\n",filelen);

	data.resize( filelen );
	fseek(finput, 0, SEEK_SET);
	fread(&data[0], filelen, 1, finput);
	fclose(finput);

}


void inline writefiledata(char * filename, char* data, int datalen)
{

	FILE* foutput = fopen(filename, "wb+" );
	fwrite(data, datalen, 1, foutput);
	fclose(foutput);

}


#endif  