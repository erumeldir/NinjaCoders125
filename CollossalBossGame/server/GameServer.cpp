#include "GameServer.h"


unsigned int GameServer::client_id; 

GameServer::GameServer(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetworker(); 
}

void GameServer::update() 
{

    // get new clients
    if(network->acceptNewClient(client_id))
    {
        printf("client %d has been connected to the server\n",client_id); 

        client_id++;
    }
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
