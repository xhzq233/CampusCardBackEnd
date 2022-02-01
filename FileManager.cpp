//
// Created by 夏侯臻 on 2022/1/14.
//


#include "FileManager.h"

FileManager &FileManager::getInstance() {
    static FileManager instance;
    return instance;
}

std::vector<std::string> FileManager::getStringDataSourceByLine(const std::string &source, const std::string &path) {
    std::ifstream infile(path+source,std::ios::in);
    if(!infile.is_open())
        //read failed
        return {};

    std::string buffer;
    std::vector<std::string> res;
    while (std::getline(infile,buffer)){
        res.push_back(buffer);
    }
    return res;
}
