/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-01-04 23:24:27
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-05 00:14:19
 */
#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

int main() {
  while (true) {
    // 创建一个客户端套接字
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
      std::cerr << "Failed to create client socket\n";
      return -1;
    }
    // 创建一个服务器地址
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8000);  
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    // 连接服务器
    if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr),
                sizeof(serverAddr)) == -1) {
      std::cerr << "Failed to connect client socket\n";
      return -1;
    }
    // 发送数据
    char data[] = "Hello, world!";
    if (send(clientSocket, data, sizeof(data), 0) == -1) {
      std::cerr << "Failed to send client ID\n";
      return -1;
    }

    // 关闭套接字
    sleep(20);
    close(clientSocket);
    return 0;
  }
  return 0;
}