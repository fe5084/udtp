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
const char REQUEST_FILE_ID[] = "00";
class UDTP{


struct SClientInfo{
	char* m_chAddress;
	unsigned int m_uiPort;
	time_t m_LastActive;

};
struct SFile {
	unsigned long int	uiSize;				// total file size
	unsigned long int	nChunks;			// number of chunks
	char			szFileName[256];	// file name (assuming 256 max file name size)
	unsigned int	 iFileId;
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
	static void check();

	//Server properties
	struct sockaddr_in m_SAddress;
	std::vector<SClientInfo> rgClients;
	unsigned int m_uiTestFileId;
	//Client properties
	char* m_chAddress;
	struct sockaddr_in m_SDestination;


	//Both properties
	bool m_bServer; // Is it a server?
	int m_iSocket; //Socket file descriptor, may remove retrieval function because UDTP makes your shit simple!
	bool m_bAlive; //Is it running?
	unsigned int m_iPort; //What is the port number?
	pthread_t m_MainThread;
	pthread_mutex_t m_VectorThreadLock;

	std::vector<pthread_t> rgOpenThreads;
	std::vector<SFile> rgProcessFiles;


	struct m_SThreadProperties{
		/*	0 - Receiving Data (This will loop)
		 *  1 - Send Data
		 *  2 - Acknowledgement
		 */
		int m_iType;
		UDTP* m_pCAccess;
		std::vector<char> m_vData;
	};
};
