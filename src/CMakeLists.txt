include_directories(../include) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")  ## 编译时添加参数  -lpthread
add_executable(startServer startServer.cpp)
add_executable(startGenerator startGenerator.cpp)
# target_link_libraries(main PRIVATE pthread)

#target_link_directories(main PRIVATE bplusUtils)