/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-01-05 11:38:24
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-05 11:40:28
 */
// EpollAgent.h

#ifndef EPOLL_AGENT_H
#define EPOLL_AGENT_H

#include <sys/epoll.h>
#include <vector>

class EpollAgent {
public:
    EpollAgent();
    ~EpollAgent();

    void addSocket(int socketFd);
    void removeSocket(int socketFd);
    void waitForEvents();
    void handleEvents(int numEvents);

private:
    int epollFd;
    std::vector<epoll_event> events;

    // 其他 EpollAgent 相关功能
};

#endif // EPOLL_AGENT_H
