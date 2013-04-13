#include "NetworkServices.h"
#include "NetworkData.h"

/* this current object sends a message
	curSocket: the socket we'll send the message through
	message: the location of where the message is located
	messageSize: the size of our messsage

	Returns the number of bytes it was able to send, or an error value if unable to send

	ERROR CHECK the return value
*/

int NetworkServices::sendMessage(SOCKET curSocket, char * message, int messageSize)
{
	//the 0 is saying that we don't need to set any flags
    return send(curSocket, message, messageSize, 0);
}

/* this current object recieves a message
	curSocket: the socket 
	buffer: recv places the date to be read into here
	bufSize: the max amount that can be read "every time receiveMessage() is called"

	Returns the number of bytes it was able to read, or an error value

	ERROR CHECK the return value
*/
int NetworkServices::receiveMessage(SOCKET curSocket, char * buffer, int bufSize)
{
	//the 0 is saying that we don't need to set any flags
    return recv(curSocket, buffer, bufSize, 0);
}
