#include "ServerNetworkManager.h"
#include "Action.h"
#include "ServerObjectManager.h"
#include "PlayerSObj.h"
#include "ConfigurationManager.h"
#include <iostream>

unsigned int ServerNetworkManager::client_id;
ServerNetworkManager ServerNetworkManager::SNM;

/*
 *	This object handles networking for the server
 *
 *	0. Variable Initialization
 *	1. Initialize Components to make connection
 *	2. Create the socket
 *	3. Set the mode of the socket to be nonblocking
 *	4. Setup the TCP listening socket
 *	5. start listening for new clients attempting to connect
 */
ServerNetworkManager::ServerNetworkManager(void)
{
	client_id = 0;
	//0. Variable Initialization
	// create WSADATA object
    WSADATA wsaData;

    // our sockets for the server
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

    // address info for the server to listen to
    struct addrinfo *result = NULL;
    struct addrinfo hints;

	// 1. Initialize Components to make connection
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        DC::get()->print("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    // set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
    hints.ai_flags = AI_PASSIVE;

	 // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if ( iResult != 0 ) {
        DC::get()->print("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

	// 2. Create the socket
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        DC::get()->print("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // 3. Set the mode of the socket to be nonblocking
    u_long iMode = 1;	
	if(CM::get()->find_config_as_bool("NETWORK_SERVER_USE_NONBLOCKING")) {
		DC::get()->print("Setting Server Network to be non-blocking.");
		iMode = 0;
	}
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        DC::get()->print("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // 4. Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        DC::get()->print("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // 5. start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        DC::get()->print("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
}

// Destructor - does nothing.
ServerNetworkManager::~ServerNetworkManager(void) {}

// Fetch the singleton ServerNetworkManager object.
ServerNetworkManager * ServerNetworkManager::get() {
	return &SNM;
}

// Update loop - accepts new connections
// Collects and stores data 
void ServerNetworkManager::update() {
	// get new clients
	do {
    if(acceptNewClient(client_id)) {
        DC::get()->print("client %d has been connected to the server\n",client_id);

		// Create a Test Server Object for them (for now)
		SOM *som = SOM::get();
		// Ok, since we should only have one object on both sides, the id's will match
		// but how do we get them matching later? maybe the server should send
		// the client the id back or something?
		som->add(new PlayerSObj(som->genId()));

		// TODO Send generated player id back to client

        client_id++;
    }
	} while (client_id == 0);
	// Collect data from clients
    receiveFromClients();
}

// Loop through each client and checks messages sent from them.
void ServerNetworkManager::receiveFromClients() {
    Packet packet;
    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;
    for(iter = sessions.begin(); iter != sessions.end(); iter++) {
        int data_length = receiveData(iter->first, network_data);

		// TODO FOR NOW: CHANGE? loop until you get data from a client....
        while (data_length <= 0) { //no data recieved
            //continue;
			data_length = receiveData(iter->first, network_data);
        }

        int i = 0;
        while ((unsigned int)i < (unsigned int)data_length) {
            packet.deserialize(&(network_data[i]));
            i += sizeof(Packet);
			// <Log Packet>
			//cout << "Iteration: " << packet.iteration << " packet_type: " << packet.packet_type << " object_id: " << packet.object_id << " packet_number: " << packet.packet_number << " command_type: " << packet.command_type << endl;
			DC::get()->print(TIMESTAMP | LOGFILE, "Iteration: %d packet_type: %d object_id: %d packet_number: %d command_type: %d\n", packet.iteration, packet.packet_type, packet.object_id, packet.packet_number, packet.command_type);
			// </Log Packet>
            switch (packet.packet_type) {
				ServerObject* destObject;
                case INIT_CONNECTION:
                    DC::get()->print("server received init packet from client %d\n", iter->first);
                    break;
                case ACTION_EVENT:
                    DC::get()->print("server received action event packet from client %d\n", iter->first);
					//inputstatus is;
					//memcpy(&is, &packet.packet_data, sizeof(inputstatus));

					// Set the input status of the TestSObj (FOR NOW id 0!! needs to change)
					destObject = SOM::get()->find(packet.object_id);

					// TODO handshake to set up player object, so this shouldn't happen after that
					if (destObject != NULL) {
						destObject->deserialize(packet.packet_data);
					}
					//memcpy(&(((TestSObj*)SOM::get()->find(0))->istat), &packet.packet_data, sizeof(inputstatus));

					// Re-send what you gave me xD (wow, we're a useful server =P)
					/*char packet_data[sizeof(Packet)];
					packet.serialize(packet_data);
					sendToAll(packet_data, sizeof(packet));*/
                    // sendActionPackets();
                    break;
                default:
                    DC::get()->print("error in packet types\n");
                    break;
            }
        }
    }
}

/* accept new connections
	id: supposed to be a unique id associated with the client
	
	returns true if "a new" client was added.
*/
bool ServerNetworkManager::acceptNewClient(unsigned int & id) {
    // if client waiting, accept the connection and save the socket
    ClientSocket = accept(ListenSocket,NULL,NULL);

    if (ClientSocket != INVALID_SOCKET) {
        //disable nagle on the client's socket
        char value = 1;
        setsockopt( ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

        // insert new client into session id table
        sessions.insert( pair<unsigned int, SOCKET>(id, ClientSocket) );
        return true;
    }
    return false;
}

// receive incoming data
int ServerNetworkManager::receiveData(unsigned int c_id, char * recvbuf) {
    if( sessions.find(c_id) != sessions.end() ) {
        SOCKET currentSocket = sessions[c_id];
        iResult = NetworkServices::receiveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE);
        if (iResult == 0) {
            DC::get()->print("Connection closed\n");
            closesocket(currentSocket);
        }
        return iResult;
    }
    return 0;
}

char* ServerNetworkManager::getSendBuffer() {
	send_buffer.clear();
	return &(send_buffer.packet_data[0]);
}

void ServerNetworkManager::sendToAll(unsigned int packet_type, unsigned int data_size) {
	sendToAll(0, packet_type, 0, CMD_UPDATE, data_size);
}
void ServerNetworkManager::sendToAll(unsigned int packet_type, unsigned int object_id, unsigned int data_size) {
	sendToAll(0, packet_type, object_id, CMD_UPDATE, data_size);
}
void ServerNetworkManager::sendToAll(unsigned int packet_type, unsigned int object_id, CommandTypes command_type, unsigned int data_size) {
	sendToAll(0, packet_type, object_id, command_type, data_size);
}

// send data to all clients
void ServerNetworkManager::sendToAll(unsigned int iteration, unsigned int packet_type, unsigned int object_id, CommandTypes command_type, unsigned int data_size) {
	char data[sizeof(Packet)];
	send_buffer.iteration = iteration;
	send_buffer.packet_type = packet_type;
	send_buffer.object_id = object_id;
	send_buffer.command_type = command_type;
	send_buffer.data_size = data_size;
	send_buffer.serialize(data);

    SOCKET currentSocket;
    std::map<unsigned int, SOCKET>::iterator iter;
    int iSendResult;

    for (iter = sessions.begin(); iter != sessions.end(); iter++) {
        currentSocket = iter->second;
        iSendResult = NetworkServices::sendMessage(currentSocket, data, sizeof(Packet));

        if (iSendResult == SOCKET_ERROR) {
            DC::get()->print("send failed with error: %d\n", WSAGetLastError());
            closesocket(currentSocket);
        }
    }
}


