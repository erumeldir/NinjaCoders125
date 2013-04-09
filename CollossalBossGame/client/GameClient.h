#include <winsock2.h>
#include <Windows.h>
#include "ClientNetworkManager.h"

class GameClient
{

public:

    GameClient(void);
    ~GameClient(void);

    ClientNetworkManager * network;
	
	void sendActionPackets();

    char network_data[MAX_PACKET_SIZE];

    void update();

private:

   // IDs for the clients connecting for table in ServerNetwork
    static unsigned int exper;
};