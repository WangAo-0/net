// #ifndef TCPSERVER_H_
// #define TCPSERVER_H_
// #include "EventLoop.h"

// class TcpServer
// {
// private:
//     EventLoop *loop_; //
//     一个TcpServer可以有多个事件循环，现在是单线程，暂时只用一个事件循环

// public:
//     TcpServer(const std::string &ip, const uint16_t port);
//     ~TcpServer();
// };

// #endif

#include "TcpServer.h"

TcpServer::TcpServer(std::unordered_map<int, int> *clientIDMap,const std::string &ip, const uint16_t port)
    : loop_(new EventLoop){
  // 这里要用堆对象，因为要在事件循环中使用
  Socket *serverSocket = new Socket(createNonblocking());
  serverSocket->setReuseAddr(true);
  serverSocket->setReusePort(true);
  serverSocket->setTcpNoDelay(true);
  serverSocket->setKeepAlive(true);

  InetAddress serverAddr(INADDR_ANY, SERVER_PORT);
  serverSocket->bindAddress(serverAddr);
  serverSocket->listen();

  // 讲EPOLL和事件循环封装成一个类

  Channel *serverChannel = new Channel(loop_->ep(), serverSocket->fd());
  serverChannel->setReadCallBack(std::bind( &Channel::newConnection, serverChannel, serverSocket, clientIDMap));
  // 采用水平触发
  serverChannel->enableRead();
}
TcpServer::~TcpServer() { delete loop_; }

void TcpServer::start(std::unordered_map<int,int> *map) { loop_->run(map);}
