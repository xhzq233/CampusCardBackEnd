cmake_minimum_required(VERSION 3.21)
project(CampusCardBackEnd)

set(CMAKE_CXX_STANDARD 17)

add_executable(CampusCardBackEnd main.cpp lib/DataStore/Account/Card/Card.cpp lib/DataStore/Account/Card/Card.h lib/DataStore/Operation/CardManage/CardManage.cpp lib/DataStore/Operation/CardManage/CardManage.h lib/DataAnalyze/CardDataAnalyze.cpp lib/DataAnalyze/CardDataAnalyze.h lib/DataStore/Window.h lib/FileManager/FileManager.cpp lib/FileManager/FileManager.h lib/DataStore/Consumption/Consumption.cpp lib/DataStore/Consumption/Consumption.h lib/DataStore/DataStore.cpp lib/DataStore/DataStore.h lib/DataStore/Account/Account.cpp lib/DataStore/Account/Account.h lib/DataStore/Operation/Consume/Consume.cpp lib/DataStore/Operation/Consume/Consume.h lib/Dependencies.h lib/DataStore/Operation/BaseOperation/BaseOperation.cpp lib/DataStore/Operation/BaseOperation/BaseOperation.h)
