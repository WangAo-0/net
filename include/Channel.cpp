/*
 * @Description:
 * @Author: oliver
 * @version:
 * @Date: 2024-03-05 15:50:27
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-26 15:21:51
 */

#include "Channel.h"
#include "Epoll.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
// class Channel
// {
// private:
//     int fd_ = -1; // Channel和fd是一对一关系
//     Epoll *epoll_ = nullptr; //
//     Channel和epoll是多对一关系,一个Channel只对应一个Epoll对象 bool inEpoll_ =
//     false; //
//     标记Channel是否在epoll树上；未添加则调用epoll_ctl(EPOLL_CTL_ADD)添加，否则调用EPOLL_CTL_MOD修改
//     uint32_t events_ = 0; //
//     fd_需要监视的事件，listenFd\ClientFd都需要监视读事件EPOLLIN,clientFd
//     还需要监视写事件EPOLLOUT uint32_t revents_ = 0; //
//     epoll_wait返回的事件,已发生的事件
// public:
//     Channel(Epoll *epoll, int fd);
//     ~Channel();

//     int fd() const {
//         return fd_;
//     }

//     void useET();//
//     设置边缘触发模式，对于read操作需要不断的读取数据直到返回EAGAIN

//     void useLT();// 设置水平触发模式，对于read操作只需要读取一次数据

//     void enableRead();// 开启读事件

//     void setInEpoll(); //将inEpoll_设置为true

//     void setREvents(uint32_t revents); // 设置revents_

//     bool inEpoll(); //返回inEpoll_

//     uint32_t events(); //返回events_

//     uint32_t revents(); // 返回revents_

// };

Channel::Channel(Epoll *epoll, int fd) : epoll_(epoll), fd_(fd) {}
Channel::~Channel() {
} // 不要销毁epoll_对象，也不能关闭fd_,因为他们不是Channel的所有者
int Channel::delFd() {
  if (inEpoll_) {
    epoll_->delFd(fd_);
    inEpoll_ = false;
  }
  return 0;
}

int Channel::fd() { return fd_; }

void Channel::useET() { // 采用边缘触发
  events_ |= EPOLLET;
}

void Channel::useLT() { // 默认是水平触发，清除边缘触发就是水平触发
  events_ &= ~EPOLLET;
}

void Channel::enableRead() { // 让epoll_wait监听读事件
  events_ |= EPOLLIN;
  epoll_->updateChannel(this);
}

void Channel::setInEpoll() { inEpoll_ = true; }

void Channel::setREvents(uint32_t revents) { revents_ = revents; }

bool Channel::inEpoll() { return inEpoll_; }

uint32_t Channel::events() { return events_; }

uint32_t Channel::revents() { return revents_; }

bool Channel::handleEvent(std::unordered_map<int, int> *clientIDMap) {
  if (revents_ & EPOLLRDHUP) {
    std::cout << "Client " << fd_ << " disconnected\n";
    close(fd_);
    return false;
  } else if (revents_ & (EPOLLIN | EPOLLPRI)) {
    readCallBack_();
  } else if (revents_ & EPOLLOUT) { // 有数据需要写，暂时没有代码，以后再说

  } else { // 其他事件，都视为错误
  }
  return true;
}

bool Channel::newConnection(Socket *servSock,
                            std::unordered_map<int, int> *clientIDMap) {
  // sockaddr_in peerAddr;
  // socklen_t peerAddrLen = sizeof(peerAddr);
  // clientSocket = accept4(serverSocket, reinterpret_cast<sockaddr
  // *>(&peerAddr), &peerAddrLen,SOCK_NONBLOCK); //
  // 改用accept4创建非阻塞套接字
  InetAddress peerAddr;

  // 这里clientSocket只能new出来, 不能直接定义，否则析构函数会关闭fd
  // TODO 这里new出来的对象没有delete，会造成内存泄漏
  Socket *clientSocket = new Socket(servSock->accept(peerAddr));

  if (clientSocket->fd() == -1) {
    return false;
  }
  std::cout << "accept client (fd=" << clientSocket->fd()
            << ", ip = " << peerAddr.ip() << ":" << peerAddr.port() << " ) ok."
            << std::endl;
  // 对连接上来的客户端采用边缘触发
  Channel *clientChannel = new Channel(epoll_, clientSocket->fd());
  clientChannel->setReadCallBack(
      std::bind(&Channel::onMessage, clientChannel, clientIDMap));
  clientChannel->useET();
  clientChannel->enableRead();

  // 接收客户端发送的第一个消息作为ID
  int clientID;
  int recvBytes = recv(clientSocket->fd(), &clientID, sizeof(clientID), 0);
  if (recvBytes <= 0) {
    std::cerr << "Failed to receive client ID\n";
    clientChannel->delFd();
    close(clientChannel->fd());
    return false;
  }

  // 将客户端ID和套接字建立关联
  std::cout << "Client ID: " << clientID << std::endl;
  std::cout << "Client fd: " << clientSocket->fd() << std::endl;
  // 将ID和客户端套接字的关系存储到unordered_map中

  (*clientIDMap)[clientID] = clientSocket->fd();
  return true;
}

bool Channel::onMessage(std::unordered_map<int, int> *clientIDMap) {
  // 监听到客户端的套接字，说明客户端有消息发送过来,转发消息
  // 先获取header
  char headerBuffer[sizeof(DiagramHeader)];
  recv(fd_, headerBuffer, sizeof(DiagramHeader), 0);
  // ToDo 出错处理
  DiagramHeader receivedHeader;
  memcpy(&receivedHeader, headerBuffer, sizeof(DiagramHeader));
  char dataBuffer[receivedHeader.diagramLength];
  int recvBytes = recv(fd_, dataBuffer, receivedHeader.diagramLength, 0);
  // 获取到的数据 std::cout.write(dataBuffer, sizeof(dataBuffer)) <<
  // std::endl;
  if (recvBytes <= 0) {
    if (recvBytes == 0) {
      std::cout << "Client disconnected\n" << std::endl;
    } else {
      std::cerr << "Error in receiving data\n" << std::endl;
    }
    delFd();
    close(fd_); // Todo ,关闭对应的Socket。关闭客户端套接字
    return false;
  }

  // 将消息转为字符数组
  char buffer[sizeof(DiagramHeader) + receivedHeader.diagramLength];
  memcpy(buffer, &receivedHeader, sizeof(DiagramHeader));
  memcpy(buffer + sizeof(DiagramHeader), dataBuffer,
         receivedHeader.diagramLength);
  // 获取消息中的目的ID，根据目的ID获取对应的客户端套接字并发送消息
  if (clientIDMap->find(receivedHeader.diagramID) == clientIDMap->end()) {
    std::cout << "Client disconnected\n" << std::endl;
    delFd();
    close(fd_);
    return false;
  } else {
    send((*clientIDMap)[receivedHeader.diagramID], buffer, sizeof(buffer), 0);
    std::cout << "Server send diagram to client " << receivedHeader.diagramID
              << std::endl;
  }
  return true;
}

void Channel::setReadCallBack(std::function<bool()> fn) { readCallBack_ = fn; }