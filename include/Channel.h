/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-03-02 14:34:10
 * @LastEditors: oliver
 * @LastEditTime: 2024-03-05 21:23:22
 */
class Channel
{
private:
    int fd_;
    bool isListen_; // true表示是服务器端监听套接字，false表示是客户端连接上来的套接字 
public:
    Channel(int fd, bool isListen = false ):fd_(fd),isListen_(isListen){};
    ~Channel();

    int fd() const {
        return fd_;
    }

    bool isListen() const {
        return isListen_;
    }

    void setFd(int fd){
        fd_ = fd;
    }

    void setListen(bool isListen){
        isListen_ = isListen;
    }

    void handleEvent(); // 处理事件

    void handleRead(); // 处理读事件
        
};