/*
 * @Description:
 * @Author: oliver
 * @version:
 * @Date: 2024-01-05 10:35:50
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-05 10:55:20
 */
#include <sys/epoll.h>
#include <iostream>

class EpollAgent {

private:
    int epollFd;
    int MAX_EVENTS = 100;

public:
  EpollAgent(int numEvents);
  ~EpollAgent();
  void start();
  virtual void run();
  int addSocket(int fd);
  void removeSocket(int fd);
  void handleEvents();
};

EpollAgent::EpollAgent(int numEvents)  {
    this->MAX_EVENTS = numEvents;
    this->epollFd = epoll_create1(0);
    if (this->epollFd == -1) {
        std::cerr << "Failed to create epoll\n";
        exit(-1);
    }
}

EpollAgent::~EpollAgent() {}

void EpollAgent::start() {
    while (true) {
        this->handleEvents();
    }
}

void EpollAgent::run() {

}

int EpollAgent::addSocket(int fd) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    int ret = epoll_ctl(this->epollFd, EPOLL_CTL_ADD, fd, &event);
    if (ret == -1) {
        std::cerr << "Failed to add socket to epoll\n";
        exit(-1);
    }
    return ret;
}


void EpollAgent::removeSocket(int fd) {
    int ret = epoll_ctl(this->epollFd, EPOLL_CTL_DEL, fd, nullptr);
    if (ret == -1) {
        std::cerr << "Failed to remove socket from epoll\n";
        exit(-1);
    }
}

void EpollAgent::handleEvents() {
    struct epoll_event events[this->MAX_EVENTS];
    int numEvents = epoll_wait(this->epollFd, events, MAX_EVENTS, -1);
    if (numEvents == -1) {
        std::cerr << "Failed to wait for events\n";
        exit(-1);
    }
    for (int i = 0; i < numEvents; ++i) {
        if (events[i].events & EPOLLIN) {
            this->run(); // 处理事件,这里是阻塞的，可以改为多线程，每个线程处理一个事件
        }
    }
}