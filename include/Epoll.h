/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-02 03:43:17
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-02 04:21:06
 */
#include <sys/epoll.h>
#include <vector>

class Epoll
{
private:
    static const int MAXFDS = 100000;
    int epollFd_ = -1;
    epoll_event events_[MAXFDS];
public:
    Epoll(); // 创建一个epoll实例
    ~Epoll(); // 关闭epoll实例

    void addFd(int fd, uint32_t events); // 添加一个文件描述符到epoll实例(红黑树)
    void modFd(int fd, uint32_t events); // 修改一个文件描述符在epoll实例中的事件
    std::vector<epoll_event> loop (int timeout = -1);
};
