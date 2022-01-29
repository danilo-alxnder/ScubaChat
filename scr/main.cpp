#include <thread>
#include <string>
#include <cstring>
#include <vector>
#include <utility>

#include "utils/BlockingQueue.h"
#include "network/Client.h"
#include "utils/Message.h"

using namespace std;

// The address to connect to. Set this to localhost to use the audio interface tool.
std::string SERVER_ADDR = "netsys2.ewi.utwente.nl"; //127.0.0.1
// The port to connect to. 8954 for the simulation server
int SERVER_PORT = 8954;
// The frequency to connect on.
int FREQUENCY = 16000; //TODO: Set this to your group frequency!

using namespace std;

string convertToString(vector<char>* word)
{
	string line = "";
	for (auto letter : *word)
	{
		if (letter != ' ')
		{
			line += letter;
		}
	}
	return line;
}

void readInput(BlockingQueue< Message >* senderQueue, string* destination)
{
	while (true)
	{
		string bla;
		cin >> bla; //read input from stdin
		//cout << "Input: " << bla << endl;
		vector<char> a(bla.begin(), bla.end()); // put input in char vector
		Message sendMessage;
		if (a[0] == '@')
		{
			a.erase(a.begin());
			(*destination) = convertToString(&a);
		}
		else
		{
			if (a.size() > 2)
			{
				for (int i = a.size(); i < 32; i++)
				{
					a.push_back(' ');
				}
				sendMessage = Message(DATA, a);
			}
			else
			{
				for (int i = a.size(); i < 2; i++)
				{
					a.push_back(' ');
				}
				sendMessage = Message(DATA_SHORT, a);
			}
			senderQueue->push(sendMessage); // put char vector in the senderQueue
		}
	}
}

void sendBookkeeping(BlockingQueue<Message>* senderQueue, string* username, string* destination, vector<pair<string, string>>* forwarding_table)
{
	Message message;

	vector<char> info((*username).begin(), (*username).end());
	for (int i = info.size(); i < 32; i++)
	{
		info.push_back(' ');
	}

	message = Message(DATA, info);
	senderQueue->push(message);

	info.assign((*destination).begin(), (*destination).end());
	for (int i = info.size(); i < 32; i++)
	{
		info.push_back(' ');
	}

	message = Message(DATA, info);
	senderQueue->push(message);

	string size = to_string(forwarding_table->size());
	info.assign(size.begin(), size.end());
	for (int i = info.size(); i < 32; i++)
	{
		info.push_back(' ');
	}

	message = Message(DATA, info);
	senderQueue->push(message);

	string line = "";

	for (auto entry : (*forwarding_table))
	{
		line += entry.first;
		line += ",";
		line += entry.second;

		info.assign(line.begin(), line.end());
		for (int i = info.size(); i < 32; i++)
		{
			info.push_back(' ');
		}

		message = Message(DATA, info);
		senderQueue->push(message);

		line = "";
	}

	line = "BK_END";

	info.assign(line.begin(), line.end());
	for (int i = info.size(); i < 32; i++)
	{
		info.push_back(' ');
	}

	message = Message(DATA, info);
	senderQueue->push(message);
}

void tryToSend(BlockingQueue< Message >* senderQueue, BlockingQueue<Message>* waitingQueue, MessageType* state,
	string* username, string* destination, vector<pair<string, string>>* forwarding_table, bool* bookkeeping_sent)
{
	// if the channel is empty and there is at least one message waiting to be sent
	// move the message into senderQueue
	if ((*state) == FREE && waitingQueue->empty() == false)
	{
		if ((*bookkeeping_sent) == false)
		{
			sendBookkeeping(senderQueue, username, destination, forwarding_table);
			*bookkeeping_sent = true;
		}

		Message message;
		message = waitingQueue->pop();
		senderQueue->push(message);
	}
}

void addWord(string* line, vector<char>* word)
{
	for (auto letter : *word)
	{
		if (letter != ' ')
		{
			*line += letter;
		}
	}
	*line += ' ';
}

void handleBookkeeping(string* bookkeeping, string* data, string* username,
	vector<pair<string, string>>* forwardingQueue, vector<pair<string, string>>* forwarding)
{
	vector<char> placeholder((*bookkeeping).begin(), (*bookkeeping).end());


	string sender = "";
	string destination = "";
	string ent = "";
	int entries = 0;
	string first = "";
	string second = "";
	vector<pair<string, string>> forwarding_table = {};

	while (placeholder[0] != ' ')
	{
		sender += placeholder[0];
		placeholder.erase(placeholder.begin());
	}
	placeholder.erase(placeholder.begin());

	while (placeholder[0] != ' ')
	{
		destination += placeholder[0];
		placeholder.erase(placeholder.begin());
	}
	placeholder.erase(placeholder.begin());

	while (placeholder[0] != ' ')
	{
		ent += placeholder[0];
		placeholder.erase(placeholder.begin());
	}
	placeholder.erase(placeholder.begin());
	entries = stoi(ent);

	for (int i = 0; i < entries; i++)
	{
		while (placeholder[0] != ',')
		{
			first += placeholder[0];
			placeholder.erase(placeholder.begin());
		}
		placeholder.erase(placeholder.begin());

		while (placeholder[0] != ' ')
		{
			second += placeholder[0];
			placeholder.erase(placeholder.begin());
		}
		placeholder.erase(placeholder.begin());

		forwarding_table.push_back(make_pair(first, second));
	}

	bool found = false;
	if (*username == destination)
	{
		cout << sender << ": " << *data << endl;
	}
	for (auto entry : forwarding_table)
	{
		if (*username != destination)
		{
			if (entry.second == *username && entry.first == destination)
			{
				forwardingQueue->insert(forwardingQueue->begin(), make_pair(*bookkeeping, *data));
			}
		}

		for (auto line : *forwarding)
		{
			if (entry.second == line.second)
			{
				found = true;
			}
		}
		if (found == false)
		{
			forwarding->push_back(entry);
		}
		found = false;
	}


}

int main()
{
	/*Data stream structure
	* Line 1: The sender's name
	* Line 2: The name of the destination
	* Line 3: The number of lines in the forwarding table
	* Line 4 . . .: The forwarding table, one message per line
	* Line n-1: Flag for end of bookkeeping
	* Line n onwards: The actual data
	*/
	BlockingQueue< Message > receiverQueue; // Queue messages will arrive in
	BlockingQueue< Message > waitingQueue; // Queue for data to wait to transmit
	BlockingQueue< Message > senderQueue; // Queue for data to transmit
	vector<pair<string, string>> forwardingQueue; // Queue of messages to forward

	string line = "";
	string user = "";
	string sender = "";
	string destination = "";
	string bookkeeping = "";

	bool bookkeeping_sent = false;
	bool bookkeeping_recieved = false;

	vector<pair<string, string>> forwarding_table = {};

	// variable to keep track of whether or not the channel is available
	MessageType state = FREE;

	Client client = Client(SERVER_ADDR, SERVER_PORT, FREQUENCY, &senderQueue, &receiverQueue);
	client.startThread();

	thread inputHandler(readInput, &waitingQueue, &destination);

	cout << "Good morning! By what name would you like to be called? ";
	cin >> user;
	cout << endl;

	forwarding_table.push_back(make_pair(user, user));

	while (true)
	{
		// try to send any messages waiting to be sent
		tryToSend(&senderQueue, &waitingQueue, &state, &user, &destination, &forwarding_table, &bookkeeping_sent);

		// if we have recieved any messages, give a breakdown on what those messages are and update state
		if (receiverQueue.empty() == false)
		{
			Message temp = receiverQueue.pop(); // wait for a message to arrive
			//cout << "Received: " << temp.type << endl; // print received chars
			switch (temp.type)
			{
			case DATA:
				cout << "DATA: ";
				for (char c : temp.data)
				{
					cout << c << ",";
				}
				cout << endl;
				if (convertToString(&(temp.data)) == "BK_END")
				{
					bookkeeping_recieved = true;
				}
				else if (bookkeeping_recieved == false)
				{
					addWord(&bookkeeping, &(temp.data));
				}
				else if (bookkeeping_recieved == true)
				{
					addWord(&line, &(temp.data));
				}
				break;
			case DATA_SHORT:
				cout << "DATA_SHORT: ";
				for (char c : temp.data)
				{
					cout << c << ",";
				}
				cout << endl;
				if (convertToString(&(temp.data)) == "BK_END")
				{
					bookkeeping_recieved = true;
				}
				else if (bookkeeping_recieved == false)
				{
					addWord(&bookkeeping, &(temp.data));
				}
				else if (bookkeeping_recieved == true)
				{
					addWord(&line, &(temp.data));
				}
				break;
				break;
			case FREE:
				cout << "FREE" << endl;
				state = FREE;
				//cout << sender << ": " << line << endl;
				cout << "Bookkeeping: " << bookkeeping << endl;
				handleBookkeeping(&bookkeeping, &line, &user, &forwardingQueue, &forwarding_table);
				bookkeeping_sent = false;
				bookkeeping_recieved = false;
				line = "";
				bookkeeping = "";
				break;
			case BUSY:
				cout << "BUSY" << endl;
				state = BUSY;
				break;
			case SENDING:
				cout << "SENDING" << endl;
				break;
			case DONE_SENDING:
				cout << "DONE_SENDING" << endl;
				break;
			case END:
				cout << "END" << endl;
				break;
			case HELLO:
				cout << "HELLO" << endl;
				break;
			}
		}
	}
}
