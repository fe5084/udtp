//============================================================================
// Name        : UDP-based file transfer (UDTP)
// Author      : Lucky
// Version     : 0.1
// Copyright   : Your copyright notice
// Description :
//============================================================================


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
#include <time.h>
//Check if Windows or BSD ready platform
#ifndef WIN32
	#include <pthread.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/time.h>
	#include <semaphore.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <poll.h>
#else
	#include <winsocks2.h>
#endif


#define SPLIT_SIZE 512

class UDTP{

struct TCPSend{
	char* m_chBuffer;
	//Address to send to
	int m_iSocket;
	
};
struct SClientInfo{
	char* m_chAddress;
	unsigned int m_uiPort;
	time_t m_LastActive;
	int m_iSocket;

};
struct SFile {
	unsigned long int	uiSize;				// total file size
	unsigned long int	nChunks;			// number of chunks
	char			szFileName[256];	// file name (assuming 256 max file name size)
	unsigned int	 iFileId;
	bool rgbChunksComplete [nChunks]; // complete or not?
	bool rgbChunksInProgress [nChunks]; // we may want something else.. didn't want to use int, just bool.
};

public:
	UDTP() { m_bAlive = false; m_iPort = 0; m_uiTestFileId = 0;};
	~UDTP() {};
	//Usage: UDTP(bool m_bServerSocket, struct sockaddr_in SDestination)
	int getSocket();
	char* getDestination();
	int getPort();
	int startClient(char* chAddress, int iPort);
	int startServer(int iPort);
	int close();
private:

	static void* openThread(void* args);
	static void* processThread(void* args);
	static void* relayThread(void* args);
	static void* priorityThread(void* args);

	//Server properties
	struct sockaddr_in m_SAddress;
	std::vector<SClientInfo> m_rgClients;
	std::vector<pollfd> m_rgConnections;
	pollfd* m_pPollSockets;
	int m_iPollActivity;
	unsigned int m_uiTestFileId;
	//Client properties
	char* m_chAddress;
	struct sockaddr_in m_SDestination;


	//Both properties
	bool m_bServer; // Is it a server?
	int m_iPrioritySocket; // TCP Socket
	int m_iSocket; //Socket file descriptor, may remove retrieval function because UDTP makes your shit simple!
			//This is the UDP 
	bool m_bAlive; //Is it running?
	unsigned int m_iPort; //What is the port number?
	pthread_t m_MainThread;
	pthread_t m_PriorityThread;
	pthread_t m_RelayThread;
	pthread_mutex_t m_VectorThreadLock;
	
	std::vector<pthread_t> rgOpenThreads;
	std::vector<SFile> rgProcessFiles;
	std::vector<TCPSend> m_rgTCPRequests;
	struct m_SThreadProperties{
		/*	0 - Receiving Data (This will loop)
		 *  1 - Send Data
		 *  2 - Acknowledgement
		 */
		int m_iType;
		unsigned int m_uiChunkNumber;
		UDTP* m_pCAccess;
		std::vector<char> m_vData;
	};
};
