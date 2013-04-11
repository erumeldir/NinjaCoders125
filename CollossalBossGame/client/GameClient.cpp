#include "GameClient.h"
#include <iostream>
using namespace std;

unsigned int GameClient::exper;

GameClient::GameClient(void)
{

}

void GameClient::sendActionPackets()
{
	/*
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
	*/
}
