#ifndef Included_BlockingQueue
#define Included_BlockingQueue

#include <queue>
#include <thread>
#include <string>
#include <list>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T> class BlockingQueue
{

private:
	queue<T> theQueue;
	mutex mutex_queue;		 //simply a door
	condition_variable cond; //block the calling thread until notified to resume

public:
	BlockingQueue(){
			//pthread_mutex_init(&mutex_queue, NULL);
			//pthread_cond_init(&cond, NULL);
		}

	T pop() {	//take out from queue
		//mutex_queue.lock();
		std::unique_lock<std::mutex> lk(mutex_queue);
		if (theQueue.empty())		//we need to wait if there is nothing in the queue! 
		{ 
				cond.wait(lk);
		}
		T ret = theQueue.front();
		theQueue.pop();
		lk.unlock();
		return ret;
	}

	void push(T object) { //insert in the queue
		mutex_queue.lock();
		theQueue.push(object);
		mutex_queue.unlock();
		cond.notify_one();		//unblocks one of the waiting threads
								//Unblocks one of the threads currently waiting for this condition.
								//If no threads are waiting, the function does nothing.
								//If more than one, it is unspecified which of the threads is selected.
	}


	bool empty()
	{
		return theQueue.empty();
	}

};

#endif