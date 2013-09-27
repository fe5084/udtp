#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <string>
#include "udtp.h"
#include "file.h"


#define SPLIT_SIZE 512

using namespace std;
//Put anything in parameter to run as client
int main(int argc, char* argv[]){
	UDTP master;
	master.startServer(6665);
	while(true){
	}



//	file fTest("dog.jpg");
//	char *chHeader = new char[SPLIT_SIZE];
//	 fTest.getHeader(chHeader);
//	cout << chHeader << endl;

//	ofstream out_file("new_dog.jpg", ofstream::binary);
//
//	char* buf;
//
//	char* buf2 = new char[SPLIT_SIZE];
//
//	unsigned int size;
//
//	size = 0;
//
//	file fTest("dog.jpg");
//
//	int nCount = 0;
//	bool bKeepGoing = true;
//	while(bKeepGoing)
//	{
//		if(fTest.getNextSplit(buf))
//		{
//			// here is where you'd want to shoot off buf in a socket
//
//			// once the client receives the socket, you would need to pass the data
//			// from the socket (buf) into parseSplit along with a second empty buffer (buf2)
//			// and this function will fill buf2 with the raw file data that needs to be written
//			fTest.parseSplit(buf,buf2,size);
//
//			// you've now got the clean file data chunk to be written into the file
//			if(out_file.is_open()){
//				out_file.write(buf2,size);
//				cout << "Wrote #" << nCount++ << endl;
//
//			}
//		}
//		else
//			bKeepGoing = false;
//	}
//	out_file.close();


}
