/*
 * @Description: 
 * 将Channel类对象的地址作为epoll_event.data.ptr,能让event携带的更多数据 
 * 封装了epoll 、socket、 events
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-02 14:34:10
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-26 15:23:21
 */
#include <sys/epoll.h>
// #include "InetAddress.h"
#include "Socket.h"
#include "MyDiagram.h"
#include <unordered_map>
#include <functional>

class Epoll;
class Channel 
{
private:
    int fd_ = -1; // Channel和fd是一对一关系
    Epoll *epoll_ = nullptr; // Channel和epoll是多对一关系,一个Channel只对应一个Epoll对象
    bool inEpoll_ = false; // 标记Channel是否在epoll树上；未添加则调用epoll_ctl(EPOLL_CTL_ADD)添加，否则调用EPOLL_CTL_MOD修改
    uint32_t events_ = 0; // fd_需要监视的事件，listenFd\ClientFd都需要监视读事件EPOLLIN,clientFd 还需要监视写事件EPOLLOUT
    uint32_t revents_ = 0; // epoll_wait返回的事件,已发生的事件
    // bool isListen_= false; // 客户端连上来的fd取值为false,listendFd取值为true
    std::function<bool()> readCallBack_; // fd_读事件的回调函数

public:
    Channel(Epoll *epoll, int fd);
    ~Channel();

    int fd();

    void useET();// 设置边缘触发模式，对于read操作需要不断的读取数据直到返回EAGAIN

    void useLT();// 设置水平触发模式，对于read操作只需要读取一次数据

    void enableRead();// 开启读事件

    void setInEpoll(); //将inEpoll_设置为true 

    void setREvents(uint32_t revents); // 设置revents_

    bool inEpoll(); //返回inEpoll_

    uint32_t events(); //返回events_

    uint32_t revents(); // 返回revents_
    
    int delFd(); // 删除fd_在epoll实例中的事件

    bool handleEvent(std::unordered_map<int, int> * clientIDMap); // 事件处理函数，epoll_wait() 返回的时候执行它
    bool onMessage(std::unordered_map<int, int> * clientIDMap);
    bool newConnection(Socket *servSock,std::unordered_map<int, int> * clientIDMap);
    void setReadCallBack(std::function<bool()> fn);
};