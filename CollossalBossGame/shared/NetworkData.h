#pragma once

#include <string.h>

#define MAX_PACKET_SIZE 1000000

// The type of message sent between client and server.
enum PacketTypes {
    INIT_CONNECTION = 0,
    ACTION_EVENT = 1,
	MESSAGE = 2,
};

// Commands sent from the ServerObjectManager to the ClientObjectManager.
enum CommandTypes {
    CREATE = 0,
    UPDATE = 1,
};

struct Packet {
	unsigned int iteration;		// The number of times through the loop for either client or server
    unsigned int packet_type;	// Type of Packet - Initialization, Actions, Messages, etc
	unsigned int object_id;		// ID of object to be updated on either client or server.
	unsigned int packet_number;	// The packet being sent back and forth.
	unsigned int command_type;	// Command read by the ClientObjectManager
	char packet_data[1024];		// Buffer to hold the data of the Packet.

	// Copies the data of this struct into a character buffer.
    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

	// Copies the data from the character buffer into this struct.
    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};

