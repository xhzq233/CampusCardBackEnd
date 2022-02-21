cmake_minimum_required(VERSION 3.21)
project(CampusCardBackEnd)

set(CMAKE_CXX_STANDARD 17)

add_executable(CampusCardBackEnd main.cpp lib/Card.cpp lib/Card.h lib/CardManage.cpp lib/CardManage.h lib/CardDataAnalyze.cpp lib/CardDataAnalyze.h lib/Window.cpp lib/Window.h lib/FileManager.cpp lib/FileManager.h lib/Consume.cpp lib/Consume.h lib/Utils.h lib/DataStore.cpp lib/DataStore.h lib/Account.cpp lib/Account.h)
