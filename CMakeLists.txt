cmake_minimum_required(VERSION 3.7)
project(penguin)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp src/universal_io.h src/process_communication/pipi_fifo/pipe.h src/process_communication/pipi_fifo/fifo.h src/process_communication/mmp/mmp_file.h src/process_communication/mmp/mmp_anonymouse.h src/process_communication/posix_ipc/message_queue.h)
add_executable(penguin ${SOURCE_FILES})