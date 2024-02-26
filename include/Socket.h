/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-02-26 13:32:23
 * @LastEditors: oliver
 * @LastEditTime: 2024-02-26 14:50:54
 */
class Socket
{
private:
    const int sockfd_;
public:
    explicit Socket(int sockfd): sockfd_(sockfd){};
    ~Socket();
    int fd() const { return sockfd_; }
    
    void bindAddress(struct sockaddr *addr);//bind函数
     
    void listen();//listen函数 

    int accept(struct sockaddr  *peeraddr);//accept函数，得到对端的InetAddress类

    int connect(struct sockaddr *addr);//connect函数

    int sendOp(const void *buf, int len);

    int recvOp(void *buf, int len);
};