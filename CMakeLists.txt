cmake_minimum_required(VERSION 3.21)
project(CampusCardBackEnd)

set(CMAKE_CXX_STANDARD 17)

add_executable(CampusCardBackEnd main.cpp lib/DataStore/Account/Card.h lib/CardManage/CardManage.cpp lib/CardManage/CardManage.h lib/DataAnalyze/DataAnalyze.h lib/DataStore/Window.h lib/FileManager/FileManager.cpp lib/FileManager/FileManager.h lib/DataStore/Operation/Consumption.h lib/DataStore/DataStore.cpp lib/DataStore/DataStore.h lib/DataStore/Account/Account.h lib/CardManage/Consume/Consume.cpp lib/CardManage/Consume/Consume.h lib/Dependencies.h lib/DataStore/Operation/BaseOperation.h lib/DataStore/Operation/CardManageOperation.h lib/DataStore/Operation/RechargeOperation.h lib/Utils/ThreadPool.h lib/Utils/CircularArray.h lib/Utils/MergeSort.h lib/DataAnalyze/DataAnalyze.cpp main.h)
