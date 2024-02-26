<!--
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2023-12-26 20:47:32
 * @LastEditors: oliver
 * @LastEditTime: 2024-02-24 03:55:24
-->
# net
中继服务器
1. 压力发生器创建多个client ,中继服务器接受多个client请求（创建、发送）。
2. 中继服务器根据最先建立连接时保存的map，判断消息该传递给哪个client。

封装socket:(1) 定义 
