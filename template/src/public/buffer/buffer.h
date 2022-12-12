 #ifndef __LINUXFFMPEGPROJECT_BUFFER_H
 #define __LINUXFFMPEGPROJECT_BUFFER_H

#include <queue>
#include <vector>
#include <mutex>
#include <atomic>
#include <functional>
#include <condition_variable>
#include <thread>
#include <unistd.h>

class Event;

template<typename T>
class Buffer {
public:
	Buffer(const unsigned int count_max_queue = 10);
	~Buffer();
	void pushData(const T &value);
	void registerProcessFunc(std::function<void(const T&)> func);
	
private:
	std::mutex			mutex_queue_;
	unsigned int 			count_max_queue_;
	std::queue<T>		queue_;

	std::condition_variable cv_queue_not_full;

	std::condition_variable cv_queue_not_empty;

	std::mutex				mtx_pro;
	std::thread             run_thread_;

	std::function<void(const T&)> func_;
};

class Event {
};


#endif
