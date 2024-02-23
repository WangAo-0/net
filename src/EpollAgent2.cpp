/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-01-05 11:38:41
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-05 11:40:30
 */
// EpollAgent.cpp

#include "../include/EpollAgent2.h"
#include <unistd.h>

EpollAgent::EpollAgent() : epollFd(epoll_create1(0)) {
    // 初始化 epoll 实例
}

EpollAgent::~EpollAgent() {
    close(epollFd);
}

void EpollAgent::addSocket(int socketFd) {
    epoll_event event;
    event.events = EPOLLIN | EPOLLET; // 设置监听可读事件和边沿触发模式
    event.data.fd = socketFd;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &event);
}

void EpollAgent::removeSocket(int socketFd) {
    epoll_ctl(epollFd, EPOLL_CTL_DEL, socketFd, nullptr);
}

void EpollAgent::waitForEvents() {
    int numEvents = epoll_wait(epollFd, events.data(), events.size(), -1);
    if (numEvents == -1) {
        // 处理错误
    }
    handleEvents(numEvents);
}

void EpollAgent::handleEvents( int numEvents) {
    for (int i = 0; i < numEvents; ++i) {
        // 处理 events[i] 中的事件
        // ...
    }
}
