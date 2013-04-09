#include "GameServer.h"


unsigned int GameServer::client_id; 

GameServer::GameServer(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetworkManager(); 
}

void GameServer::update() 
{
    // get new clients
    if(network->acceptNewClient(client_id))
    {
        printf("client %d has been connected to the server\n",client_id); 

        client_id++;
    }

    receiveFromClients();
}

GameServer * server;

/* Answers the question "What does the server do every cycle?"

*/
void serverLoop()
{
    while(true) 
    {
        server->update();
    }
}

/* the main function on the server side, initalizes the server loop

*/
int main()
{
	server = new GameServer();
	serverLoop();
}

void GameServer::receiveFromClients()
{

    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }

        int i = 0;
        while ((unsigned int)i < (unsigned int)data_length) 
        {
            packet.deserialize(&(network_data[i]));
            i += sizeof(Packet);

            switch (packet.packet_type) {

                case INIT_CONNECTION:

                    printf("server received init packet from client\n");

                    sendActionPackets();

                    break;

                case ACTION_EVENT:

                    printf("server received action event packet from client\n");

                    sendActionPackets();

                    break;

                default:

                    printf("error in packet types\n");

                    break;
            }
        }
    }
}

void GameServer::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}
