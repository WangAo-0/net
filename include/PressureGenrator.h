/*
 * @Description:
 * @Author: oliver
 * @version:
 * @Date: 2024-01-02 16:54:02
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-02 03:26:19
 */
#include "MyDiagram.h"
#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define NUM_CLIENTS 50

int startPressureGenrator() {
  std::vector<int> clientSockets(NUM_CLIENTS);
  // std::vector<std::chrono::time_point<std::chrono::steady_clock>>
  // startTimes(NUM_CLIENTS);
  std::unordered_map<int, int> clientIDMap;
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    // 1. 创建客户端套接字
    clientSockets[i] = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSockets[i] == -1) {
      std::cerr << "Failed to create client socket " << i << std::endl;
      return -1;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // 2. 连接服务器
    if (connect(clientSockets[i], reinterpret_cast<sockaddr *>(&serverAddr),
                sizeof(serverAddr)) == -1) {
      std::cerr << "Failed to connect client socket " << i << std::endl;
      return -1;
    }

    // 客户端发送ID给服务器
    clientIDMap[clientSockets[i]] =
        i; // 将ID和客户端套接字的关系存储到unordered_map中

    // 3. 发送数据
    if (send(clientSockets[i], &i, sizeof(i), 0) == -1) {
      std::cerr << "Failed to send client ID\n";
      return -1;
    }
  }

  int epollFd = epoll_create1(0);
  if (epollFd == -1) {
    std::cerr << "Failed to create epoll instance\n";
    return -1;
  }

  // 可读：接收到服务器转发消息，并且读取了
  epoll_event event, events[NUM_CLIENTS];
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    // event.events = EPOLLOUT | EPOLLET; //监听可写事件
    event.events = EPOLLIN | EPOLLOUT | EPOLLET; //监听可读可写事件
    event.data.fd = clientSockets[i];
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSockets[i], &event) == -1) {
      std::cerr << "Failed to add client socket to epoll\n";
      return -1;
    }
  }
  // sleep(3);
  int num = 1;

  while (true) {
    int numEvents = epoll_wait(epollFd, events, NUM_CLIENTS, -1);
    if (numEvents == -1) {
      std::cerr << "Failed to wait for events\n";
      break;
    }

    for (int i = 0; i < numEvents; ++i) {
      if (events[i].events & EPOLLOUT) {
        if (num == 100) {
          return 0;
        }
        DiagramHeader header;
        header.diagramID = clientIDMap[events[i].data.fd] % 2 == 0
                               ? clientIDMap[events[i].data.fd] + 1
                               : clientIDMap[events[i].data.fd] - 1; // 目标ID
        header.diagramType = 0;
        std::string data = std::to_string(num++) +
                           " Hello, server! I'm client " +
                           std::to_string(clientIDMap[events[i].data.fd]) + ".";
        header.diagramLength = data.size();
        char buffer[sizeof(DiagramHeader) +
                    header.diagramLength]; // 计算所需的字符数组大小
        memcpy(buffer, &header,
               sizeof(DiagramHeader)); // 将报文数据拷贝到字符数组中
        memcpy(buffer + sizeof(header), data.c_str(),
               data.size()); // 将报文数据拷贝到字符数组中
        //使用 send() 函数发送字符数组给接收方
        send(events[i].data.fd, buffer, sizeof(buffer), 0);
        // startTimes[events[i].data.fd - clientSockets[0]] =
        // std::chrono::steady_clock::now();

        // epoll_event event;
        // event.events = EPOLLIN | EPOLLET; // 仅保留 EPOLLIN 监听
        // event.data.fd = events[i].data.fd;
        // epoll_ctl(epollFd, EPOLL_CTL_MOD, events[i].data.fd, &event);
      }

      if (events[i].events & EPOLLIN) {
        char headerBuffer[sizeof(DiagramHeader)];
        int recvBytes =
            recv(events[i].data.fd, headerBuffer, sizeof(DiagramHeader), 0);
        DiagramHeader receivedHeader;
        memcpy(&receivedHeader, headerBuffer, sizeof(DiagramHeader));
        // std::cout << receivedHeader.diagramID << std::endl;
        char dataBuffer[receivedHeader.diagramLength];
        if (recvBytes > 0) {
          // 继续接收数据部分
          recvBytes = recv(events[i].data.fd, dataBuffer,
                           receivedHeader.diagramLength, 0);
          if (recvBytes > 0) {
            // 接收完成
          } else if (recvBytes == 0) {
            // 连接关闭或接收完成
            break;
          } else {
            // 处理接收错误
            break;
          }
        } else if (recvBytes == 0) {
          // 连接关闭或接收完成
          break;
        } else {
          // 处理接收错误
          break;
        }

        // auto endTime = std::chrono::steady_clock::now();
        // std::cout<<events[i].data.fd<< "-----"<<clientSockets[0]<<std::endl;
        // auto startTime = startTimes[events[i].data.fd - clientSockets[0]];

        // auto duration =
        // std::chrono::duration_cast<std::chrono::microseconds>(
        //                     endTime - startTime)
        //                     .count();

        std::cout << "Client " << clientIDMap[events[i].data.fd]
                  << " Received message: ";
        std::cout.write(dataBuffer, sizeof(dataBuffer)) << std::endl;
        // << " with delay: " << duration << " microseconds" << std::endl;

        // epoll_event event;
        // event.events = EPOLLOUT | EPOLLET; // 仅保留 EPOLLIN 监听
        // event.data.fd = events[i].data.fd;
        // epoll_ctl(epollFd, EPOLL_CTL_MOD, events[i].data.fd, &event);

        // epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
      }
    }
  }

  for (int i = 0; i < NUM_CLIENTS; ++i) {
    close(clientSockets[i]);
  }

  return 0;
}
