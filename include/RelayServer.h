#include "MyDiagram.h"
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

int startServer() {
  int serverSocket, clientSocket;
  sockaddr_in serverAddr, clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  // 1. 创建服务器套接字
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cerr << "Failed to create server socket\n";
    return -1;
  }

  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(SERVER_PORT);

  // 2. 绑定服务器套接字的ip和端口号
  if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr),
           sizeof(serverAddr)) == -1) {
    std::cerr << "Failed to bind server socket\n";
    return -1;
  }

  // 3. 监听服务器套接字
  if (listen(serverSocket, SOMAXCONN) == -1) {
    std::cerr << "Failed to listen for connections\n";
    return -1;
  }
  
  int epollFd = epoll_create1(0);
  if (epollFd == -1) {
    std::cerr << "Failed to create epoll instance\n";
    return -1;
  }

  epoll_event event, events[MAX_EVENTS];
  event.events = EPOLLIN ;
  event.data.fd = serverSocket;

  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
    std::cerr << "Failed to add server socket to epoll\n";
    return -1;
  }

  std::unordered_map<int, int> clientIDMap; // 使用unordered_map来存储ID和客户端套接字的关系
  while (true) {
    int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
    if (numEvents == -1) {
      std::cerr << "Failed to wait for events\n";
      return -1;  // ToDo 出错处理
    }
    for (int i = 0; i < numEvents; ++i) {
      if (events[i].data.fd == serverSocket) {
        clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
          std::cerr << "Failed to accept connection\n";
          continue;
        }
        std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        //将客户端套接字加入到epoll中，监听该套接字上的可读事件，只要可读就一直读
        event.events = EPOLLIN | EPOLLET;
        event.data.fd = clientSocket;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
          std::cerr << "Failed to add client socket to epoll\n";
          return -1;
        }

        // 接收客户端发送的第一个消息作为ID
        int clientID;
        int recvBytes = recv(clientSocket, &clientID, sizeof(clientID), 0);
        if (recvBytes <= 0) {
          std::cerr << "Failed to receive client ID\n";
          close(clientSocket);
          epoll_ctl(epollFd, EPOLL_CTL_DEL, clientSocket, nullptr);
          continue;
        }

        // 将客户端ID和套接字建立关联
        std::cout << "Client ID: " << clientID << std::endl;
        std::cout << "Client fd: " << clientSocket << std::endl;
        clientIDMap[clientID] = clientSocket; // 将ID和客户端套接字的关系存储到unordered_map中
      } else if(events[i].events & EPOLLIN){
        // 监听到客户端的套接字，说明客户端有消息发送过来,转发消息
  
        // 先获取header
        char headerBuffer[sizeof(DiagramHeader)];
        int recvBytes = recv(events[i].data.fd, headerBuffer, sizeof(DiagramHeader), 0);
        // ToDo 出错处理
        DiagramHeader receivedHeader;
        memcpy(&receivedHeader, headerBuffer, sizeof(DiagramHeader));
        char dataBuffer[receivedHeader.diagramLength];
        recvBytes = recv(events[i].data.fd, dataBuffer, receivedHeader.diagramLength, 0);
        // 获取到的数据 std::cout.write(dataBuffer, sizeof(dataBuffer)) << std::endl;
        if (recvBytes <= 0) {
          if (recvBytes == 0) {
            std::cout << "Client disconnected\n"<<std::endl;
          } else {
            std::cerr << "Error in receiving data\n"<<std::endl;
          }
          close(events[i].data.fd); // 关闭客户端套接字
          epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, nullptr); // 从epoll中删除该套接字
          continue;
        }

        // 将消息转为字符数组
        char buffer[sizeof(DiagramHeader) + receivedHeader.diagramLength];
        memcpy(buffer, &receivedHeader, sizeof(DiagramHeader));
        memcpy(buffer + sizeof(DiagramHeader), dataBuffer,
               receivedHeader.diagramLength);
        // 获取消息中的目的ID，根据目的ID获取对应的客户端套接字并发送消息
        if (clientIDMap.find(receivedHeader.diagramID) == clientIDMap.end()) {
          std::cout << "Client disconnected\n"<<std::endl;
          close(events[i].data.fd);
          epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
          continue;
        } else {
          send(clientIDMap[receivedHeader.diagramID], buffer, sizeof(buffer), 0);
          std::cout << "Server send diagram to client "
                    << receivedHeader.diagramID << std::endl;
        }
      }
    }
  }

  close(serverSocket);
  close(epollFd);

  return 0;
}
