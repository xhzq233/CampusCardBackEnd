//
// Created by 夏侯臻 on 2022/1/14.
//


#include "FileManager.h"

FileManager &FileManager::getInstance() {
    static FileManager instance;
    return instance;
}

bool FileManager::getStringDataSourceByLine(std::vector<std::string> &container, const std::string &source,
                                            const std::string &path) {
    std::cout<<path+source<<std::endl;
    std::ifstream infile(path+source,std::ios::in);
    if(!infile.is_open())
        //read failed
        return false;

    std::string buffer;
    while (std::getline(infile,buffer)){
        container.push_back(buffer);
    }
    return true;
}
