cmake_minimum_required(VERSION 3.16)
project(sec_ipc_project)

set(CMAKE_C_FLAGS "-w -rdynamic -fPIC -g")


#add_library(seclib STATIC zmq_basic.c)
add_library(seclib SHARED zmq_basic.c)


add_executable(proc1 proc1.c)
target_link_libraries(proc1 seclib zmq czmq)

add_executable(proc2 proc2.c)
target_link_libraries(proc2 seclib zmq czmq)


add_executable(secure_proc1 secure_proc1.c)
target_link_libraries(secure_proc1 seclib zmq czmq)

add_executable(secure_proc2 secure_proc2.c)
target_link_libraries(secure_proc2 seclib zmq czmq)



