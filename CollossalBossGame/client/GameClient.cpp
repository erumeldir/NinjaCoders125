#include "GameClient.h"
#include "NetworkData.h"

unsigned int GameClient::exper;

GameClient::GameClient(void)
{
    network = new ClientNetworkManager();

	// send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

GameClient * client;

/* Answers the question "What does the client do every cycle?"

*/
void clientLoop()
{
    while(true)
    {
        //printf("Hi from clientLoop!");
    }
}

/* the main function on the client side, initalizes the client loop

*/
int main()
{
	client = new GameClient();
	clientLoop();
}
