#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#define SERVER_PORT 8000
#define MAX_EVENTS 100

class Agent {
public:
  virtual void handle(int clientSock,
                      std::unordered_map<int, int> &clientIDMap) = 0;
};

class RelayAgent : public Agent {
public:
  void handle(int clientSock,
              std::unordered_map<int, int> &clientIDMap) override {
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = recv(clientSock, buffer, sizeof(buffer), 0)) > 0) {
      send(clientSock, buffer, bytesRead, 0);
    }

    close(clientSock);
    clientIDMap.erase(clientSock);
  }
};

class RelayServer {
private:
  int serverSocket;
  std::unordered_map<int, int> clientIDMap;
  Agent *relayAgent;

public:
  RelayServer() : serverSocket(-1), relayAgent(nullptr) {}

  bool start() {
    sockaddr_in serverAddr;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
      std::cerr << "Failed to create server socket\n";
      return false;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr),
             sizeof(serverAddr)) == -1) {
      std::cerr << "Failed to bind server socket\n";
      close(serverSocket);
      return false;
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
      std::cerr << "Failed to listen for connections\n";
      close(serverSocket);
      return false;
    }

    relayAgent = new RelayAgent();
    return true;
  }

  void run() {
    socklen_t clientAddrLen = sizeof(sockaddr_in);

    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
      std::cerr << "Failed to create epoll instance\n";
      return;
    }

    epoll_event event, events[MAX_EVENTS];
    event.events = EPOLLIN;
    event.data.fd = serverSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
      std::cerr << "Failed to add server socket to epoll\n";
      return;
    }

    while (true) {
      int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
      if (numEvents == -1) {
        std::cerr << "Failed to wait for events\n";
        return;
      }
      for (int i = 0; i < numEvents; ++i) {
        if (events[i].data.fd == serverSocket) {
          int clientSocket = accept(serverSocket, nullptr, &clientAddrLen);
          if (clientSocket == -1) {
            std::cerr << "Failed to accept connection\n";
            continue;
          }
          std::cout << "Client connected\n";

          event.events = EPOLLIN | EPOLLET;
          event.data.fd = clientSocket;
          if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
            std::cerr << "Failed to add client socket to epoll\n";
            continue;
          }

          clientIDMap[clientSocket] = clientSocket;
        } else if (events[i].events & EPOLLIN) {
          relayAgent->handle(events[i].data.fd, clientIDMap);
        }
      }
    }
  }

  ~RelayServer() {
    close(serverSocket);
    delete relayAgent;
  }
};

int main() {
  RelayServer server;
  if (server.start()) {
    server.run();
  } else {
    std::cerr << "Failed to start the server\n";
  }

  return 0;
}
