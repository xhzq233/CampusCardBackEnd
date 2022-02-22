cmake_minimum_required(VERSION 3.21)
project(CampusCardBackEnd)

set(CMAKE_CXX_STANDARD 17)

add_executable(CampusCardBackEnd main.cpp lib/DataStore/Account/Card/Card.cpp lib/DataStore/Account/Card/Card.h lib/CardManage/CardManage.cpp lib/CardManage/CardManage.h lib/DataAnalyze/CardDataAnalyze.cpp lib/DataAnalyze/CardDataAnalyze.h lib/DataStore/Window.h lib/FileManager/FileManager.cpp lib/FileManager/FileManager.h lib/DataStore/Consume/Consume.cpp lib/DataStore/Consume/Consume.h lib/Utils.h lib/DataStore/DataStore.cpp lib/DataStore/DataStore.h lib/DataStore/Account/Account.cpp lib/DataStore/Account/Account.h lib/DataStore/Consume/ConsumeBuilder.cpp lib/DataStore/Consume/ConsumeBuilder.h lib/Dependencies.h)
