//============================================================================
// Name        : UDP-based file transfer (UDTP)
// Author      : Lucky
// Version     : 0.1
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include "udtp.h"

int UDTP::getSocket(){
	return m_iSocket;
}

int UDTP::getPort(){
	return m_iPort;
}
char* UDTP::getDestination(){
	return m_chAddress;
}
/*	Method: startServer()
	Usage: Starts the server.
	Return: 0 on successful
			1 socket has been initialized already!
			2 could not bind
			3 could not listen
			4 could not start mutex
*/
int UDTP::startServer(int iPort){
	if(!m_bAlive){
		if (pthread_mutex_init(&m_VectorThreadLock, NULL) != 0) {
				perror("mutexlock");
				return 4;
			}
		UDTP::m_iPort = iPort;
		memset(&m_SAddress, 0, sizeof(m_SAddress));
		m_SAddress.sin_port = htons(m_iPort);
		m_SAddress.sin_family = AF_INET;
		m_SAddress.sin_addr.s_addr = INADDR_ANY;
		m_iSocket = socket(AF_INET, SOCK_DGRAM,0);
		
		m_iPrioritySocket = socket(AF_INET, SOCK_STREAM,0); // init TCP socket
		if((bind(m_iPrioritySocket, (struct sockaddr*)&m_SAddress, sizeof(m_SAddress))) < 0){	
			if((bind(m_iSocket, (struct sockaddr*)&m_SAddress, sizeof(m_SAddress)))<0){
				perror("bind");
				return 2;	
				}

		}
	
	//Listen on TCP Socket
		if(listen(m_iPrioritySocket, 0) < 0){
			perror("listen");
			return 2;	
		}
	
	m_bServer = true;
	m_bAlive = true;
	pthread_create(&m_PriorityThread, NULL, &UDTP::PriorityThread, (UDTP*)this);
	pthread_create(&m_MainThread, NULL, &UDTP::processThread, (UDTP*)this);
	pthread_tryjoin_np(m_MainThread, NULL);
	pthreaD_tryjoin_np(m_PriorityThread, NULL);

	return 0;

	}

	// Socket has already been initialized! Do not initialize again!
	return 1;
}


/*	Method: startClient(char* DESTINATION_ADDRESS, int DESTINATION_PORT)
	Usage: Starts a client socket
	Return: 0 on successful
			1 socket has been initialized already!
			4 could not start mutex

*/
int UDTP::startClient(char* chAddress, int iPort){
	if(!m_bAlive){
		if (pthread_mutex_init(&m_VectorThreadLock, NULL) != 0) {
			perror("mutexlock");
			return 4;
		}
		UDTP::m_chAddress = chAddress;
		UDTP::m_iPort = iPort;
		memset(&m_SDestination, 0, sizeof(m_SDestination));
		m_SDestination.sin_addr.s_addr = atoi(m_chAddress);
		m_SDestination.sin_port = htons(m_iPort);
		m_SDestination.sin_family = AF_INET;
		m_iSocket = socket(AF_INET, SOCK_DGRAM, 0);
		m_bServer = false;
		m_bAlive = true;
		pthread_create(&m_MainThread, NULL, &UDTP::processThread, (UDTP*)this);
			pthread_tryjoin_np(m_MainThread, NULL);
		return 0;
	}

	// Socket has been initialized already! Don't do this again!
	return 1;

}
int UDTP::close(){

	if(m_bAlive){
		m_bAlive = false;
	}
	return 0;

}


/*	This will handle everything that goes on with the server, such as queueing up requests and listening to whines.
 *	This is the main part of UDTP.
 *
 *	... I need more ram
 */
 
void *UDTP::priorityThread(void* args){
	pollfd iTCPSocket;
	iTCPSocket.fd = CProperties->m_iPrioritySocket;
	iTCPSocket.events = POLLIN;
	CProperties->m_rgConnections.push_back(iTCPSocket);
	CProperties->pPollSockets = &m_rgConnections[0];
	while(CProperties->m_bAlive){
		//Reiterate the pointer
		CProperties->pPollSockets = &CProperties->m_rgConnections[0];
		CProperties->m_iPollActivity = poll(pPollSockets,m_rgConnections.size(),-1);
		if(m_iPollActivity<0){
			perror("poll");
		}
		if(CProperties->m_rgConnections[0].revents & POLLIN){
			int iAddSocket;
			struct sockaddr_in SClientAddress;
			int iClientAddrLen = sizeof(client_address);
			if ((new_socket = accept(CProperties->m_iProritySocket,
					(struct sockaddr*) &SClientAddress,
					(socklen_t*) &iClientAddrLen)) < 0) {
				perror("accept");
			}else{
				//Client was accepted
				pollfd iNewSocket;
				iNewSocket.fd = iAddSocket;
				iNewSocket.events = POLLIN;
				CProperties->m_rgConnections.push_back(iNewSocket);
				//Create client info
				SClientInfo SNewClient;
				SNewClient.m_iSocket = iNewSocket;
				SNewClient.m_chAddress = inet_ntoa(SClientAddress.sin_addr);
				SNewClient.m_iPort = ntohs(SClientAddress.sin_port);
				SNewClient.m_LastActive = time(NULL);
				CProperties->m_rgClients.push_back(SNewClient);
				
				
				//Need to rework on IDE, only worked on GitHub editor
			}
					
		}
		
		for(int i=0; i<CProperties->m_rgConnections.size(); i++){
			//Check through all sockets except 1 because that is server socket	
		}
		//Sleep and reduce CPU usage
		poll(0,0,100);
	}
	return NULL;
}
void *UDTP::processThread(void* args){
	//Not working! Find out how to pass member variables to thread!
	UDTP *CProperties = (UDTP*) args;

	while(CProperties->m_bAlive){
		if(CProperties->m_bServer){

			//Server processThread programming here

			char buffer[SPLIT_SIZE];
			struct sockaddr_in SClientAddr;

			memset(&SClientAddr, 0, sizeof(SClientAddr));
			socklen_t fromlen = sizeof(SClientAddr);

			int iByteCount = recvfrom(CProperties->m_iSocket, buffer, sizeof (buffer), MSG_PEEK, (struct sockaddr*)&SClientAddr, &fromlen);
				//Increment File ID's
			CProperties->m_uiTestFileId++;

				//Treat it as read
			recvfrom(CProperties->m_iSocket, buffer, sizeof (buffer), 0, (struct sockaddr*)&SClientAddr, &fromlen);

			}

			//			std::cout << inet_ntoa(SClientAddr.sin_addr) << ntohs(SClientAddr.sin_port) << std::endl;
			//			m_SThreadProperties m_sNewProperties;
			//			m_sNewProperties.m_pCAccess = CProperties;
			//			if(one_time){
			//				pthread_t test_thread;
			//				pthread_create(&test_thread, NULL, &UDTP::openThread, (m_SThreadProperties*)&m_sNewProperties);
			//				pthread_tryjoin_np(test_thread, NULL);
			//				one_time = false;
			//			}

			/*	Find out if it is an acknowledgment or a split file (in order to split into another thread)
				If it is a split file being received then have it be processed by creating a new thread of openThread
				passing on m_SThreadProperties with all the information it needs
				to identify itself with what file it should be recvfrom'ing in.
			*/

		}else{
			//Client processThread here

		}
	poll(0, 0, 100);
	}

	return NULL;
}
/*	This will be the threading for all things such as sending data and receiving data
	It will all be declared in the argument for it's use.
	It will also whine requests if something goes wrong and close itself until processThread opens another one again
*/
void* UDTP::openThread(void* args){
	struct m_SThreadProperties *SProperties = (struct m_SThreadProperties *)args;
	while(SProperties->m_pCAccess->m_bAlive)
	{

	}
	return (int*) 0;
}

