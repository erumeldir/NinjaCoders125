#include "ClientNetworkManager.h"
#include "ClientObjectManager.h"
#include "TestObject.h"
#include <iostream>
#include <string>
#include <vector>
#include "Action.h"

ClientNetworkManager ClientNetworkManager::CNM;
#define HOST "127.0.0.1"
#define PORT DEFAULT_PORT

/*
	This object handles networking for the client

	Constructor Phases:
		0. Initial Variables
		1. Initialize Components to make connection
		2. Connect
		3. If we didn't connect, exit
		4. Set the mode of the socket to be nonblocking.
		5. disable nagle... don't know what it is, the tutorial said this was optional

*/
ClientNetworkManager::ClientNetworkManager(void) {
	// 0. Inital Variables
    // create WSADATA object
    WSADATA wsaData;

    // the socket
    ConnectSocket = INVALID_SOCKET;

    // holds address info for socket to connect to
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

	// 1. Initialize Components to make connection

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
		system("pause");
        exit(1);
    }

    // set address info
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

	//resolve server address and port 
	//Address to connect to is here. Local host is good for default
    iResult = getaddrinfo(HOST, PORT, &hints, &result);

    if( iResult != 0 ) 
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
		system("pause");
        exit(1);
    }

	// 2. Connect
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
			system("pause");
            exit(1);
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            printf ("The server is down... did not connect.\n");
        }
    }

    // no longer need address info for server
    freeaddrinfo(result);

	// 3. If we didn't connect, exit
    // if connection failed
    if (ConnectSocket == INVALID_SOCKET) 
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
		system("pause");
        exit(1);
    }
	
	// 4. Set the mode of the socket to be nonblocking.
	//  if we really want the server to block everything, we'd need to replace the next
	//  10 lines of code
    u_long iMode = 1;

    iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
    if (iResult == SOCKET_ERROR)
    {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
		system("pause");
        exit(1);        
    }

	//5. disable nagle... don't know what it is, the tutorial said this was optional
    char value = 1;
    setsockopt( ConnectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );
}

ClientNetworkManager::~ClientNetworkManager(void) {

}

int ClientNetworkManager::receivePackets(char * recvbuf) 
{
    iResult = NetworkServices::receiveMessage(ConnectSocket, recvbuf, MAX_PACKET_SIZE);

    if ( iResult == 0 )
    {
        printf("Connection closed\n");
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }

    return iResult;
}

ClientNetworkManager * ClientNetworkManager::get() {
	return &CNM;
}

void ClientNetworkManager::update()
{
    Packet packet;
    int data_length = receivePackets(network_data);

    if (data_length <= 0) 
    {
        //no data recieved
        return;
    }

    int i = 0;
	//
    while ((unsigned int)i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {
            case ACTION_EVENT:
                //printf("client received action event packet from server\n");
					
					// controllerstatus cs;
				memcpy(&(((TestObject*)COM::get()->find(0))->cstat), &packet.packet_data, sizeof(controllerstatus));
					// ((TestObject*)COM::find(0))->cstat = cs;
                break;
            default:
                printf("error in packet types\n");
                break;
        }
    }
}

void ClientNetworkManager::sendData(char * data, int datalen) {
	std::cout << data << std::endl;
	const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;
	memcpy(packet.packet_data, data, datalen);
    packet.serialize(packet_data);

    NetworkServices::sendMessage(ConnectSocket, packet_data, packet_size);
}