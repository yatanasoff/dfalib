cmake_minimum_required(VERSION 2.8)
project(dfa_testmod_ui LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Core Quick Widgets REQUIRED)
set(SOURCES
    main.cpp
    runner.cpp
    wrapper.cpp
    resultmodel.cpp
    machineid.cpp
    authenticationmanager.cpp
    )
set(HEADERS
    runner.h
    wrapper.h
    resultmodel.h
    machineid.h
    authenticationmanager.h
    )
set(RESOURCES
    qml.qrc
    )
set(Qt5Quick_DIR /opt/qt5/5.13.0/gcc_64/lib/cmake/Qt5Quick)
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})
include_directories(../../include)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick Qt5::Widgets Qt5::Network)
target_link_libraries(${PROJECT_NAME} regex dfalib)
#for jwt
target_link_libraries(${PROJECT_NAME}  -lssl -lcrypto )

