#include "ClientNetworkManager.h"

class GameClient
{

public:

    GameClient(void);
    ~GameClient(void);
	
	void sendActionPackets();

    char network_data[MAX_PACKET_SIZE];

private:

   // IDs for the clients connecting for table in ServerNetwork
    static unsigned int exper;
};