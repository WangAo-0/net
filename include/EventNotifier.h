/*
 * @Description: 
 * @Author: oliver
 * @version: 
 * @Date: 2024-02-26 18:53:46
 * @LastEditors: oliver
 * @LastEditTime: 2024-02-26 19:05:36
 */

#include <iostream>
#include <functional>
#include <vector>

using CallbackFunction = std::function<void()>;

class EventNotifier {
public:
    // 注册回调函数
    void registerCallback(CallbackFunction callback) {
        callbacks.push_back(callback);
    }

    // 触发所有注册的回调函数
    void notify() {
        for (auto& callback : callbacks) {
            callback();
        }
    }

private:
    std::vector<CallbackFunction> callbacks;
};