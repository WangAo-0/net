#include "EventLoop.h"
#include "TcpServer.h"
// #include "Socket.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#define SERVER_PORT 8000
#define MAX_EVENTS 100

EventLoop loop;

int startServer() {
  // 使用unordered_map来存储ID和客户端套接字的关系
  std::unordered_map<int, int> clientIDMap;
  
  TcpServer tcpsever(&clientIDMap);
  tcpsever.start(&clientIDMap);
  return 0;
}
