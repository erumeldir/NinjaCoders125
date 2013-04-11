#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {
    INIT_CONNECTION = 0,
    ACTION_EVENT = 1,
	MESSAGE = 2,
};

struct Packet {

    unsigned int packet_type;
	char packet_data[100];

    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};

