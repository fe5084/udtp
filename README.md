UDTP API for C++ Documentation
====

UDTP is a file transfer implementation to the User Datagram protocol. It uses a set amount of threads to send a file, split up in optimal size, more faster and efficiently than standard TCP. Both server and client will have a process to check the integrity of the file through an acknowledgement system. The API will include functions that allow to check the completion of the transfer and allow for direct handling of the file once it is entirely there -- it will return a fstream.

UDTP will try and remain as simple as possible. For client and servers: you initialize the UDTPSocket, connect to the server, and send a file or much like polling file descriptors -- wait until there is a file to receive. You will not have to worry about the troubling matters of splitting up a file and sending it, UDTP will handle that given a fstream. Whether you are creating a Photo sharing app or just want to transfer large files, let UDTP take care of the delivery and receipt while you focus on the design.

Supported languages
====
C++, Objective-C

Status
====
UDTP is currently under development.

Team
====
Kevin Trinh - Networking Architecture.
Justin Holdsclaw - File transfer (assembly and disassembly) architecture.




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
	pollfd add_socket;
	add_socket.fd = master_socket;
	add_socket.events = POLLIN;
	vector<pollfd> poll_sockets;
	pollfd* poll_sockets_ptr;
	poll_sockets.push_back(add_socket);
	int poll_activity = 0;
	while(server_online){
		poll_sockets_ptr = &poll_sockets[0];
		poll_activity = poll(poll_sockets_ptr, poll_sockets.size(), -1);
		if (poll_activity < 0){
			perror("poll");
		}
		if(poll_sockets[0].revents & POLLIN){
			int new_socket;
			struct sockaddr_in client_address;
			int client_address_len = sizeof(client_address);
			if(new_socket = accept(poll_sockets[0].fd, (struct sockaddr*)&client_address, (socklen_t*)&client_address_len) <0){
				perror("accept");
			}else{
				pollfd add_socket;
				add_socket.fd = new_socket;
				add_socket.events = POLLIN;
				poll_sockets.push_back(add_socket);
				cout << "Justin has connnected" << endl;
			}
		}

		for(int i=0; i<poll_sockets.size(); i++){
			if(i!=0){
				if(poll_sockets[i].revents & POLLIN){
					char buffer[SPLIT_SIZE];
					if((read(poll_sockets[i].fd, buffer, SPLIT_SIZE)!=0)){
						if((memcmp(buffer, "FILE", 4) == 0)){
							cout << "TEST" << endl;

						
						}
					}else{
						cout << "Justin has disconnected!"<< endl;
						poll_sockets.erase(poll_sockets.begin()+i);
					}

				}
			}
		}
		cout << "Deadlock" << endl;
	}
	return NULL;
}

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
