#include "GameServer.h"
#include "ConfigurationManager.h"
#include <iostream>
using namespace std;

GameServer::GameServer(void)
{

}

/* the main function on the server side, initalizes the server loop

*/
int main()
{
	// CM::get();
	// cout << CM::get()->find_config("asdf") << endl;
	// system("pause");

	while(true) 
    {
        ServerNetworkManager::get()->update();
    }
}