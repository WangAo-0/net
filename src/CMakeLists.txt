include_directories(../include) 
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")  ## 编译时添加参数  -lpthread
add_executable(startServer startServer.cpp ../include/InetAddress.cpp ../include/Socket.cpp )
add_executable(startGenerator startGenerator.cpp ../include/InetAddress.cpp ../include/Socket.cpp )


# target_link_libraries(main PRIVATE pthread)

#target_link_directories(main PRIVATE bplusUtils)

# 测试
# add_executable(testClient testClient.cpp)
# add_executable(testServer testServer.cpp)