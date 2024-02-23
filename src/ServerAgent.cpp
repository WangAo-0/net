/*
 * @Description:
 * @Author: oliver
 * @version:
 * @Date: 2024-01-05 09:17:03
 * @LastEditors: oliver
 * @LastEditTime: 2024-02-23 22:22:07
 */

#include "../include/SocketAgent.h"

class ServerAgent : protected SocketAgent {

public:
  ServerAgent(int port) : SocketAgent(port){};
  ServerAgent(int port, bool nonBlock) : SocketAgent(port, nonBlock){};

  void initSocket() {
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(this->port);
    
    // 将地址绑定到socketFd上
    int ret = bind(this->socketFd, (struct sockaddr *)&serverAddr,
                   sizeof(serverAddr));
    if (ret == -1) {
      std::cerr << "Failed to bind server socket\n";
      exit(-1);
    }

    // 开启监听
    ret = listen(this->socketFd, SOMAXCONN);
    if (ret == -1) {
      std::cerr << "Failed to listen server socket\n";
      exit(-1);
    }
  }

  int acceptClient();

  ~ServerAgent();
};

int ServerAgent::acceptClient() {
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  int clientSocket =
      accept(this->socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
  if (clientSocket == -1) {
    std::cerr << "Failed to accept client socket\n";
    exit(-1);
  }
  return clientSocket;
}

