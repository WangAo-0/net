#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#include "Epoll.h"
#include "Channel.h"
#include <unordered_map>

class EventLoop {
private:
  //每个事件循环只有一个Epoll
  Epoll *ep_;

public:
  EventLoop(); // 创建一个Epoll对象
  ~EventLoop(); // 销毁Epoll对象

  void run(std::unordered_map<int, int> *clientIDMap); // 运行事件循环
  Epoll * ep();
};

#endif