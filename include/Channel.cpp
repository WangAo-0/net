/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-05 15:50:27
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-05 15:52:12
 */

#include "Channel.h"    
#include <unistd.h>

void Channel::handleEvent(){
    if(isListen_){
        handleAccept();
    }else{
        handleRead();
    }
}

void Channel::handleRead(){
    char buf[1024];
    int n = read(fd_, buf, sizeof(buf));
    if(n > 0){
        write(fd_, buf, n);
    }else if(n == 0){
        close(fd_);
    }else{
        perror("read");
    }
}

void Channel::handleAccept(){
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientFd = accept(fd_, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if(clientFd == -1){
        perror("accept");
    }else{
        printf("accept a new connection: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        Channel* clientChannel = new Channel(clientFd);
        clientChannel->setListen(false);
        clientChannel->setFd(clientFd);
        clientChannel->handleEvent();
    }
}