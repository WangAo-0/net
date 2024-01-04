/*
 * @Description: 启动服务器端，并启动压力发生器
 * @Author: oliver
 * @version: 1.0
 * @Date: 2023-12-26 20:50:59
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-03 07:02:19
 */

#include"../include/PressureGenrator.h"
#include"../include/RelayServer.h"
#include<iostream>
#include<thread>

using namespace std;

int main() {
    // 创建两个线程并分别传入不同的函数名
    
    startPressureGenrator();
    
    return 0;
}