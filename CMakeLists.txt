cmake_minimum_required(VERSION 3.5)
project(penguin)

set(CMAKE_CXX_STANDARD 14)
message(STATUS "LINKER:" ${CMAKE_LINK_LIBRARY_FLAG})

set(SOURCE_FILES 
    main.cpp 
    src/universal_io.h 
    src/process_communication/pipi_fifo/pipe.h 
    src/process_communication/pipi_fifo/fifo.h 
    src/process_communication/mmp/mmp_file.h 
    src/process_communication/mmp/mmp_anonymouse.h 
    src/process_communication/posix_ipc/message_queue.h
    src/process_communication/posix_ipc/sem.h
    src/process_communication/posix_ipc/share_memory.h
    src/process_communication/socket/unix_socket.h)

add_executable(penguin ${SOURCE_FILES})
target_link_libraries(penguin rt)
target_link_libraries(penguin pthread)
