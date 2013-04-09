#include "ServerNetworkManager.h"

class GameServer
{

public:
    GameServer(void);
    ~GameServer(void);

    void update();
	void receiveFromClients();
	void sendActionPackets();

private:
    static unsigned int client_id;		// IDs for the clients connecting for table in ServerNetwork 
    ServerNetworkManager * network;		// The ServerNetwork object 
    char network_data[MAX_PACKET_SIZE];	// data buffer
};