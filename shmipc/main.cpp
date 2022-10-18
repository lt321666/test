#include <iostream>
#include <mv_ipc_operator.h>
#include <stdio.h>
#include <stdlib.h>

#include <chrono>    // std::chrono::seconds
#include <iostream>  // std::cout
#include <thread>    // std::thread, std::this_thread::sleep_for
#include <mutex>
#include <condition_variable>
std::mutex mylock;
std::condition_variable condVar;

	MVIPCPublisher spub("test");
	MVIPCSubscriber ssub;
	static int time1 = 0;
	void callback(void* data, unsigned int data_size){
	//	    std::unique_lock<std::mutex> lock(mylock);
	//	    std::cout << "run" << std::endl;
			
			int* b = (int *)data;
			// if(time1 != *b){
				// std::cout << "error" << std::endl;
				// return;
			// }
//			time1++;
			std::cout << "pub: "<<*b << std::endl;
	//		condVar.notify_all();	
	}
void ssubfunc (void){
	while(1){
		{
			
		
//        std::cout << "run" << std::endl;
		ssub.subscribe("test",callback);

		}
	}
}
void spubfunc(void){
	while(1){
		static int time2 = 0;
		int *a ;
		*a = time2;
//		{       
		spub.publish(a,sizeof(a));		
		std::cout << "sub: "<<*a << std::endl;
		time2++;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
//		std::unique_lock<std::mutex> lock(mylock);
//		std::cout << "waiting" << std::endl;
//		condVar.wait(lock);
//		}

	}


}


int main(void){

	
	std::thread t1(ssubfunc);
	std::thread t2(spubfunc);
	
	t1.join();
	t2.join();
	return 0;	
	
}