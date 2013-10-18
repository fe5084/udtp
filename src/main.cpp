#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>
//Check if Windows or BSD ready platform

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define SPLIT_SIZE 420

using namespace std;
int master_socket;
bool server_online;
struct file_baby{
	ofstream *my_file;
	int socket;
};
vector<file_baby> files;

void* connectionThread(void*){
	pollfd master_pollsocket;
	master_pollsocket.fd = master_socket;
	master_pollsocket.events = POLLIN;
	vector<pollfd> poll_sockets;
	pollfd* poll_sockets_ptr;
	int poll_activity;
	poll_sockets.push_back(master_pollsocket);
	while(server_online){
		poll_sockets_ptr = &poll_sockets[0];
		poll_activity = poll(poll_sockets_ptr, poll_sockets.size(), -1);
		if (poll_activity<0){
			perror("poll");
		}

		if(poll_sockets[0].revents & POLLIN){
			int new_socket;
			struct sockaddr_in client_address;
			int client_address_len = sizeof(client_address);
			if(new_socket = accept(master_socket, (struct sockaddr*)&client_address, (socklen_t*)&client_address_len) < 0){
				perror("accept");

			}else{
				pollfd add_pollsocket;
				add_pollsocket.fd = new_socket;
				add_pollsocket.events = POLLIN;
				poll_sockets.push_back(add_pollsocket);
				cout << "Client " << new_socket << " has connected!" << endl;
			}
		}

		for(int i=1; poll_sockets.size(); i++){
			if(poll_sockets[i].revents & POLLIN){
				cout << "Atleast it can tell its reading?" << endl;
				char buffer[SPLIT_SIZE];
				cout << read(poll_sockets[i].fd, buffer, SPLIT_SIZE) << endl;
				if (read(poll_sockets[i].fd, buffer, SPLIT_SIZE)!=0){
					cout << buffer << endl;
				}else{
					cout << "Client " << poll_sockets[i].fd << " has disconnected!" << endl;
					poll_sockets.erase(poll_sockets.begin()+i);
				}

			}
		}
	}
}

// void* connectionThread(void*){
// 	pollfd bing_socket;
// 	bing_socket.fd = master_socket;
// 	bing_socket.events = POLLIN;
// 	vector<pollfd> poll_sockets;
// 	pollfd* poll_sockets_ptr;
// 	poll_sockets.push_back(bing_socket);
// 	int poll_activity;
// 	while(server_online){
// 		poll_sockets_ptr = &poll_sockets[0];
// 		poll_activity = poll(poll_sockets_ptr, poll_sockets.size(), -1);
// 		if (poll_activity < 0){
// 			perror("poll");
// 		}
// 		if(poll_sockets[0].revents & POLLIN){
// 			int new_socket;
// 			struct sockaddr_in client_address;
// 			int client_address_len = sizeof(client_address);
// 			if(new_socket = accept(poll_sockets[0].fd, (struct sockaddr*)&client_address, (socklen_t*)&client_address_len) <0){
// 				perror("accept");
// 			}else{
// 				pollfd add_socket;
// 				add_socket.fd = new_socket;
// 				add_socket.events = POLLIN;
// 				poll_sockets.push_back(add_socket);
// 				cout << "Justin has connnected" << endl;
// 			}
// 		}

// 		for(int i=0; i<poll_sockets.size(); i++){
// 			if(i!=0){
// 				if(poll_sockets[i].revents & POLLIN){
// 					cout << "test read" << endl;
// 					char buffer[SPLIT_SIZE];
// 				 if ((read(poll_sockets[i].fd, buffer, SPLIT_SIZE)) != 0) {
// 						cout << buffer << endl;
// 						char* file_char = "FILE";
// 						char* file_done = "DONE";
// 						if((strcmp(buffer, file_char) == 0)){
// 							file_baby new_file_struct;
// 							new_file_struct.my_file = new ofstream("test_file.png");
// 							new_file_struct.socket = poll_sockets[i].fd;
// 							files.push_back(new_file_struct);
// 							cout << "MADE NEW FILE" << endl;

						
// 						}else if((strcmp(buffer, file_done) == 0)){
// 							for(int b=0; b<files.size(); b++){
// 								if(files[b].socket == poll_sockets[i].fd){
// 									files[b].my_file->close();
// 								}
// 							}
// 						}else{
// 								for(int b=0; b<files.size(); b++){
// 									if(files[b].socket == poll_sockets[i].fd){
// 										files[b].my_file->write(buffer,SPLIT_SIZE);
// 									}
								
// 							}
// 						}

// 							cout << "TEST" << endl;

						
				
// 					}else{
// 						cout << "Justin has disconnected!"<< endl;
// 						poll_sockets.erase(poll_sockets.begin()+i);
// 					}

// 				}
// 			}
// 		}
// 		cout << "Deadlock" << endl;
// 	}
// 	return NULL;
// }

int main(int argc, char* argv[]){
	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	server_online = false;
	struct sockaddr_in server_address;
	server_address.sin_port = htons(6666);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;


	if(bind(master_socket, (struct sockaddr*)&server_address, sizeof(server_address))<0){
		perror("bind");
		return 1;
	}
	if(listen(master_socket, 0) < 0){
		perror("listen");
	}
	server_online = true;
	pthread_t t1;
	pthread_create(&t1, NULL, connectionThread, NULL);
	pthread_join(t1, NULL);
	cout << "Server is up" << endl;
}
// #include <iostream>
// #include <fstream>
// #include <iomanip>
// #include <vector>
// #include <cstring>
// #include <string>
// #include "udtp.h"
// #include "file.h"


// #define SPLIT_SIZE 512

// using namespace std;
// //Put anything in parameter to run as client
// int main(int argc, char* argv[]){
// 	UDTP master;
// 	master.startServer(6665);
// 	while(true){
// 	}



// //	file fTest("dog.jpg");
// //	char *chHeader = new char[SPLIT_SIZE];
// //	 fTest.getHeader(chHeader);
// //	cout << chHeader << endl;

// //	ofstream out_file("new_dog.jpg", ofstream::binary);
// //
// //	char* buf;
// //
// //	char* buf2 = new char[SPLIT_SIZE];
// //
// //	unsigned int size;
// //
// //	size = 0;
// //
// //	file fTest("dog.jpg");
// //
// //	int nCount = 0;
// //	bool bKeepGoing = true;
// //	while(bKeepGoing)
// //	{
// //		if(fTest.getNextSplit(buf))
// //		{
// //			// here is where you'd want to shoot off buf in a socket
// //
// //			// once the client receives the socket, you would need to pass the data
// //			// from the socket (buf) into parseSplit along with a second empty buffer (buf2)
// //			// and this function will fill buf2 with the raw file data that needs to be written
// //			fTest.parseSplit(buf,buf2,size);
// //
// //			// you've now got the clean file data chunk to be written into the file
// //			if(out_file.is_open()){
// //				out_file.write(buf2,size);
// //				cout << "Wrote #" << nCount++ << endl;
// //
// //			}
// //		}
// //		else
// //			bKeepGoing = false;
// //	}
// //	out_file.close();


// }
