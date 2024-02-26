/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-02-26 14:52:15
 * @LastEditors: oliver
 * @LastEditTime: 2024-02-26 15:34:05
 */
#include"Socket.h"
class Accepter
{
private:
    Socket acceptSocket_;//封装socket的一些接口
public:
    Accepter();
    ~Accepter();
    void listen();
};