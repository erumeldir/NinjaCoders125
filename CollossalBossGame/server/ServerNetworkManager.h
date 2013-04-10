#include <winsock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include "NetworkData.h"
#include <ws2tcpip.h>
#include <map>
using namespace std; 
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881" 

class ServerNetworkManager
{
private:
	ServerNetworkManager(void);
    ~ServerNetworkManager(void);

	static ServerNetworkManager SNM;	// Class Singleton
	static unsigned int client_id;		// Unique Client Ids for each connecting client
	char network_data[MAX_PACKET_SIZE];	// data buffer
public:
	static ServerNetworkManager * get();

	void update();												// Generic Update cycle - manages buffer and connections.
	void receiveFromClients();									// Collects data from all clients
	bool acceptNewClient(unsigned int & id);					// accept new connections
    int  receiveData(unsigned int client_id, char * recvbuf);	// receive incoming data
    void sendToAll(char * packets, int totalSize);				// send data to all clients

    SOCKET ListenSocket;	// Socket to listen for new connections
    SOCKET ClientSocket;	// Socket to give to the clients
    int iResult;			// for error checking return values
    std::map<unsigned int, SOCKET> sessions; // table to keep track of each client's socket
	
	
};

