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
    IOStream.open(path + source);
    if (!IOStream.is_open()) {
        //read failed
        IOStream.close();
        return false;
    }

    std::string buffer;
    while (std::getline(IOStream, buffer))
        container.emplace_back(buffer);

    //confirm IOStream steam closed
    IOStream.close();
    return true;
}

std::string FileManager::CONSUME_CSV(unsigned int position) {
    std::string res(CONSUME_CSV_DICTIONARY);
    return res + "/W" + std::to_string(position) + ".csv";
}

bool FileManager::getCSVDataSource(std::vector<std::vector<std::string>> &container, const std::string &source,
                                   const std::string &path) {
    IOStream.open(path + source);
    if (!IOStream.is_open()) {
        //read failed
        IOStream.close();
        return false;
    }

    std::string rowBuffer;//a csv row implemented in string
    std::string metadata;//metadata in a csv row
    std::vector<std::string> row;

    while (std::getline(IOStream, rowBuffer)) {

        std::stringstream buf(rowBuffer);//turn to a stream type

        while (std::getline(buf, metadata, ','))//csv use ',' as the separator
            row.emplace_back(metadata);

        container.emplace_back(row);
        row.clear();
    }

    //confirm IOStream steam closed
    IOStream.close();
    return true;
}