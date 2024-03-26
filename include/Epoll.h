/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-02 03:43:17
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-26 13:51:13
 */
#include <sys/epoll.h>
#include <vector>

class Channel; // 前置声明Channel类

class Epoll
{
private:
    static const int MAXFDS = 100000;
    int epollFd_ = -1;
    epoll_event events_[MAXFDS];
public:
    Epoll(); // 创建一个epoll实例
    ~Epoll(); // 关闭epoll实例

    // int addFd(int fd, uint32_t events); // 添加一个文件描述符到epoll实例(红黑树)

    int updateChannel(Channel *channel); // 把channel添加/更新到红黑树上，channel中有fd,也有需要监视的事件
    
    int modFd(int fd, uint32_t events); // 修改一个文件描述符在epoll实例中的事件
    int delFd(int fd); // 删除一个文件描述符在epoll实例中的事件
   // std::vector<epoll_event> loop (int timeout = -1); // 开启epoll_wait,返回发生的事件  
     std::vector<Channel*> loop (int timeout = -1); // 开启epoll_wait,返回Channel*指针  
    
};
