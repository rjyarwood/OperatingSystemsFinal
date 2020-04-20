cmake_minimum_required(VERSION 3.15)
project(5806final)

set(CMAKE_CXX_STANDARD 14)

add_executable(5806final main.cpp VDIFile.cpp displayBuffers.cpp displayBuffers.cpp.h VDIFile.h uuid2ascii.cpp PartitionFile.h PartitionFile.cpp PartitionEntry.h PartitionEntry.cpp ext2Files.h ext2Files.cpp)