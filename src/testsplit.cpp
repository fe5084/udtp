#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <string>
#include "file.h"

#define SPLIT_SIZE 512

using namespace std;

vector<char*> split;
vector<int> split_this_shit;

int main(int argc, char* argv[]){
	ofstream out_file("finally.png", ofstream::binary);

	char* buf;

	char* buf2 = new char[512];

	unsigned int size;

	size = 0;

	file fTest("logo_myDog.png");
	
	int nCount = 0;
	bool bKeepGoing = true;
	while(bKeepGoing)
	{
		if(fTest.getNextSplit(buf))
		{
			fTest.parseSplit(buf,buf2,size);

			if(out_file.is_open()){
				out_file.write(buf2,size);
				cout << "Wrote #" << nCount++ << endl;
			}
		}
		else
			bKeepGoing = false;
	}
	out_file.close();

}
