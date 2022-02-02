cmake_minimum_required(VERSION 3.21)
project(CampusCardBackEnd)

set(CMAKE_CXX_STANDARD 17)

add_executable(CampusCardBackEnd main.cpp Card.cpp Card.h CardManage.cpp CardManage.h CardDataAnalyze.cpp CardDataAnalyze.h Cafe.cpp Cafe.h FileManager.cpp FileManager.h Consume.cpp Consume.h)
