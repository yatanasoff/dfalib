cmake_minimum_required(VERSION 2.8)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11 ")

set(HEADERS
        ../../include/dfalib/dfa.h
        ../../include/dfalib/dnalangparser.h
        ../../include/dfalib/stringutils.h
        ../../include/dfalib/grammargenerator.h
        ../../include/dfalib/parserequest.h
        ../../include/dfalib/algorithms.h
        )


set (SOURCES
        dfa.cpp
#        algorithms.cpp
        parserequest.cpp
        stringutils.cpp
        dnalangparser.cpp
        grammargenerator.cpp
        )

include_directories(../../include/)
add_library(dfalib ${SOURCES} ${HEADERS})


