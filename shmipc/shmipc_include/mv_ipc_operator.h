#ifndef MV_IPC_OPERATOR_H
#define MV_IPC_OPERATOR_H

#include <memory>
#include <string>
#include <functional>

/**
 * @brief Maxvision Inter-Process Communication Publisher
 * 
 */
class MVIPCPublisher {
public:
    MVIPCPublisher(const std::string& topic);
    ~MVIPCPublisher();
    // MVIPCPublisher(const MVIPCPublisher& f) = delete; // copy constructor
    // MVIPCPublisher& operator=(const MVIPCPublisher& f) = delete; // assignment function
    class MVIPCPublisherImpl;
    /**
     * @brief The interface of message sending.
     * 
     * @param data the struct based on EventInfo in event_type_define.h
     * @param data_size sizeof(struct)
     */
    void publish(void* data, unsigned int data_size);
private:
    std::unique_ptr<MVIPCPublisherImpl> p_impl_;
};

typedef void (*mv_ipc_subscriber_callback)(void* data, unsigned int data_size);

/**
 * @brief Maxvision Inter-Process Communication Publisher
 * 
 */
class MVIPCSubscriber {
public:
    MVIPCSubscriber();
    ~MVIPCSubscriber();
    // MVIPCSubscriber(const MVIPCSubscriber& f) = delete; // copy constructor
    // MVIPCSubscriber& operator=(const MVIPCSubscriber& f) = delete; // assignment function
    class MVIPCSubscriberImpl;
    /**
     * @brief this subscriber need to refresh in a while loop, the default value is 1000 us (1ms)
     * 
     * @param us must bigger than 0, otherwise set failed.
     */
    void setRefreshTime(unsigned int us);
    /**
     * @brief subscribe the topic and set the callback deal with 
     * 
     * @param topic 
     * @param miscb 
     */
    bool subscribe(const std::string& topic, mv_ipc_subscriber_callback miscb);
    bool subscribe(const std::string& topic, const std::function<void(void* data, unsigned int data_size)>& cppcb);
    void unsubscribe();

private:
    std::unique_ptr<MVIPCSubscriberImpl> p_impl_;
};

#endif