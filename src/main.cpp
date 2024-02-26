/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-02-26 19:09:42
 * @LastEditors: oliver
 * @LastEditTime: 2024-02-26 19:10:01
 */
#include "../include/EventNotifier.h"
void myCallbackFunction() {
    std::cout << "Callback function is called!" << std::endl;
}
int main() {
    EventNotifier notifier;

    // 将回调函数注册到对象中
    notifier.registerCallback(myCallbackFunction);

    // 触发回调函数
    notifier.notify();

    return 0;
}