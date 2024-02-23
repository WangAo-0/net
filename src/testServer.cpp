/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-01-04 23:27:09
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-05 00:37:25
 */

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
int main(){
    // 创建一个服务器套接字
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create server socket\n";
        return -1;
    }
    // 创建一个服务器地址
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // 绑定服务器套接字的ip和端口号
    if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr),
            sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to bind server socket\n";
        return -1;
    }
    // 监听服务器套接字
    if (listen(serverSocket, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen for connections\n";
        return -1;
    }
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    while (true)
    {
        client_socket = accept(serverSocket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1)
        {
            std::cerr << "Failed to accept client socket\n";
            return -1;
        }
        char buffer[1024];
        int len = recv(client_socket, buffer, sizeof(buffer), 0);
        if (len == -1) {
            std::cerr << "Failed to receive data from client\n";
            return -1;
        }
        std::cout << "Received data from client: " << buffer << std::endl;
        sleep(10);
        std::string str = "hello, Are ur there?";
        auto res = send(client_socket, &str, sizeof(str), 0);
        if(res == -1){
            std::cerr << "Failed to send data to client\n";
            return -1;
        }else{
            std::cout << "Send data to client: " << str << std::endl;
        }

        int send_buffer_size;
        socklen_t optlen = sizeof(send_buffer_size);
        if (getsockopt(client_socket,  SOL_SOCKET, SO_SNDBUF, &send_buffer_size, &optlen) == -1) {
            std::cerr << "Error in getsockopt" << std::endl;
            close(client_socket);
            return -1;
        }

        std::cout << "发送缓冲区大小为: " << send_buffer_size << " 字节" << std::endl;
        
        char buffer2[1024];
        auto r = recv(client_socket, buffer2, sizeof(buffer2), 0);
        if (r == 0) {
            std::cerr << "clien is closed\n";
            return -1;
        }else if ( r == -1) {
            std::cerr << "Failed to receive data from client2\n";
            return -1;
        }
        std::cout << "Received data from client2: " << buffer2 << std::endl;
    }
}