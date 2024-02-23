/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-01-05 10:32:01
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-05 10:32:55
 */
#include"../include/SocketAgent.h"

class ClientAgent : protected SocketAgent {
public:
  ClientAgent(int port) : SocketAgent(port){};
  ClientAgent(int port, bool nonBlock) : SocketAgent(port, nonBlock){};

  void initSocket() {
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(this->port);

    int ret = connect(this->socketFd, (struct sockaddr *)&serverAddr,
                      sizeof(serverAddr));
    if (ret == -1) {
      std::cerr << "Failed to connect server socket\n";
      exit(-1);
    }
  }

  ~ClientAgent();
};

