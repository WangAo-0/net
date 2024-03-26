#ifndef TCPSERVER_H_
#define TCPSERVER_H_
#include "EventLoop.h"
#define SERVER_PORT 8000

class TcpServer {
private:
  EventLoop *
      loop_; //  一个TcpServer可以有多个事件循环，现在是单线程，暂时只用一个事件循环

public:
  TcpServer(std::unordered_map<int, int> *clientIDMap,
            const std::string &ip = "127.0.0.1", const uint16_t port = 9999);
  ~TcpServer();

  void start(std::unordered_map<int, int> *map); // 运行事件循环
};

#endif