#ifndef My_Diagram
#define My_Diagram
/*
 * @Description: 报文结构体
 * @Author: oliver
 * @version: 
 * @Date: 2024-01-02 16:48:14
 * @LastEditors: oliver
 * @LastEditTime: 2024-01-02 21:44:04
 */

#include<stdlib.h>
#include <string>

// 报文头部
struct DiagramHeader {
    int diagramID;  // 报文ID，使用四个字节表示
    u_int8_t diagramType;  // 报文类型，使用一个字节表示
    u_int32_t diagramLength;  // 报文数据长度，使用四个字节表示
};

// // 报文
// struct MyDiagram
// {   
//     DiagramHeader diagramHeader;
//     std::string diagramData; // 数据部分
// }; 


#endif My_Diagram