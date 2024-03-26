// #ifndef EVENT_LOOP_H
// #define EVENT_LOOP_H
// #include "Epoll.h"
// class EventLoop {
// private:
//   //每个事件循环只有一个Epoll
//   Epoll *ep_;

// public:
//   EventLoop(); // 创建一个Epoll对象
//   ~EventLoop(); // 销毁Epoll对象

//   void run(); // 运行事件循环
// };

// #endif

#include "EventLoop.h"

// 创建一个Epoll对象
EventLoop::EventLoop():ep_(new Epoll){}  

// 销毁Epoll对象
EventLoop::~EventLoop(){
    delete ep_;
} 

Epoll * EventLoop::ep(){
    return ep_;
}

void EventLoop::run(std::unordered_map<int, int> *clientIDMap) {
  while (true) {
    std::vector<Channel *> channels = ep_->loop(-1);
    for (auto ch : channels) {
      ch->handleEvent(clientIDMap);
    }
  }
}