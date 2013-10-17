#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 420
#define DEFAULT_PORT "6666"

int main(void) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char *sendbuf = "FILE";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

	ifstream* inFile;

	inFile = new ifstream("myDog.jpg");
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed with error: " << iResult << endl;
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("10.25.99.214", DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        cout << "getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            cout << "socket failed with error: " << WSAGetLastError() << endl;
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
       cout << "Unable to connect to server!" << endl;
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        cout << "send failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    cout << "Sent FILE.\nBytes Sent: " <<  iResult << endl;

	cout << "Sending file..." << endl;

	int i = 0;
	while(inFile->is_open())
	{

		//int size = min(420,(int)(inFile-> - inFile->tellg()));

		char* buf = new char[420];


		inFile->read(buf,420);

		iResult = send( ConnectSocket, sendbuf, 420, 0 );
		if (iResult == SOCKET_ERROR) {
			cout << "send failed with error: " << WSAGetLastError() << endl;
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		cout << "Sent " << ++i << " packets." << endl;
	}

	cout << "Done sending file (total " << i << ") packets." << endl;

	cout << "Sending DONE..." << endl;

    // Send an final buffer
    iResult = send( ConnectSocket, "DONE", (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        cout << "send failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
