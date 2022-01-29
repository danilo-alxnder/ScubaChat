#ifndef Included_MessageType
#define Included_MessageType

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

#endif