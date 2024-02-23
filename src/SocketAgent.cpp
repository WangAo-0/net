/*
 * @Description:
 * @Author: oliver
 * @version:
 * @Date: 2024-01-05 05:59:47
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-05 10:26:22
 */
#include "../include/SocketAgent.h"

SocketAgent::SocketAgent(int port) {
  this->port = port;
  this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->socketFd == -1) {
    std::cerr << "Failed to create server socket\n";
    exit(-1);
  }
}

SocketAgent::SocketAgent(int port, bool nonBlock) {
  this->port = port;
  this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->socketFd == -1) {
    std::cerr << "Failed to create server socket\n";
    exit(-1);
  }
  if (nonBlock) {
    int flags = fcntl(this->socketFd, F_GETFL, 0);
    fcntl(this->socketFd, F_SETFL, flags | O_NONBLOCK);
  }
}

SocketAgent::~SocketAgent() { close(this->socketFd); }

int SocketAgent::sendOp(char *buffer, int len) {
  auto res = send(this->socketFd, buffer, len, 0);
  if (res == -1) {
    std::cerr << "Failed to send data to client\n";
    return -1;
  } else {
    std::cout << "Send data to client: " << buffer << std::endl;
    return 0;
  }
}
int SocketAgent::recvOp() {
  char buffer[1024];
  int len = recv(this->socketFd, buffer, sizeof(buffer), 0);
  if (len == -1) {
    std::cerr << "Failed to receive data from client\n";
    return -1;
  }
  std::cout << "Received data from client: " << buffer << std::endl;
  return 0;
}
int SocketAgent::closeOp() {
  close(this->socketFd);
  return 0;
};
