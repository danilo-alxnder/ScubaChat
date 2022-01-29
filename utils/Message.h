#ifndef Included_Message
#define Included_Message

#include "MessageType.h"

class Message {
	
public:
	MessageType type;
	vector<char> data;
	int id;

	//Default Constructor
	Message() {
	}

	//Second Constructor
	Message(MessageType newType) {
		type = newType;
	}
	
	//Third Constructor
	Message(MessageType newType, vector<char> newData) {
		type = newType;
		data = newData;
	
	}
};

#endif


/*

enum MessageType {


	FREE = 1,
	BUSY = 2,
	DATA = 3,
	SENDING = 4,
	DONE_SENDING = 5,
	DATA_SHORT = 6,
	END = 8,
	HELLO = 9
};

*/