#pragma once

#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {
    INIT_CONNECTION = 0,
    ACTION_EVENT = 1,
	MESSAGE = 2,
};

enum CommandTypes {
    CREATE = 0,
    UPDATE = 1,
};

struct Packet {

    unsigned int packet_type;
	unsigned int object_id;
	unsigned int packet_number;
	unsigned int command_type;
	char packet_data[100];

    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};

