#include "GameClient.h"
#include <iostream>
using namespace std;

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

void GameClient::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void GameClient::update()
{
    Packet packet;
    int data_length = network->receivePackets(network_data);

    if (data_length <= 0) 
    {
        //no data recieved
        return;
    }

    int i = 0;
    while ((unsigned int)i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {
            case ACTION_EVENT:
                printf("client received action event packet from server\n");
				cout << packet.packet_data << endl;
                sendActionPackets();
                break;
            default:
                printf("error in packet types\n");
                break;
        }
    }
}