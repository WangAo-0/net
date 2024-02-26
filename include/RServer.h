/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-02-26 13:08:24
 * @LastEditors: oliver
 * @LastEditTime: 2024-02-26 13:28:59
 */
class RServer
{
private:
    int epollFd_; 
public:
    RServer(int epollFd): epollFd_(epollFd){};
    ~RServer();
};

RServer::~RServer()
{
    
}
