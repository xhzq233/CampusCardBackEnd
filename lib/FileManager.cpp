//
// Created by 夏侯臻 on 2022/1/14.
//


#include "FileManager.h"

FileManager::FileManager() {
    time_t now = time(nullptr);
    startUpTime = ctime(&now);
}

FileManager &FileManager::getInstance() {
    static FileManager instance;
    return instance;
}

bool FileManager::prepareIOStream(const std::function<void(std::fstream &)> &func, const std::string &path,
                                  const char mode) {
    IOStream.open(path, mode);
    if (!IOStream.is_open()) {
        //read failed
        IOStream.close();
        return false;
    }
    func(IOStream);

    //confirm IOStream steam closed
    IOStream.close();
    return true;
}

bool FileManager::getStringDataSourceByLine(std::vector<std::string> &container, const std::string &source,
                                            const std::string &path) {
    IOStream.open(path + source, std::ios::in);
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
    IOStream.open(path + source, std::ios::in);
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

bool FileManager::logs(std::vector<std::string> &container) {
    return writeStrings(container, startUpTime, DEFAULT_LOG_PATH);
}

bool FileManager::log(std::string &content) {
    return writeStringByLine(content, startUpTime, DEFAULT_LOG_PATH);
}

bool FileManager::writeStringByLine(std::string &content, const std::string &source, const std::string &path) {
    return prepareIOStream([&](std::fstream &stream) {
        stream << content << std::endl;
    }, path + source, std::ios::app);
}

bool
FileManager::writeStrings(std::vector<std::string> &container, const std::string &source, const std::string &path) {
    return prepareIOStream([&](std::fstream &stream) {
        for (const auto &content: container)
            stream << content << std::endl;
    }, path + source, std::ios::app);
}

bool FileManager::writeCSVData(std::vector<std::vector<std::string>> &container, const std::string &sourceName,
                               const std::string &path) {
    std::vector<std::string> transform(container.size());
    for (const auto &row: container)
        transform.emplace_back(std::reduce(row.begin(), row.end(), "", [](const std::string &r,
                                                                          const std::string &l) -> std::string {
            std::string s(r);
            s.append(",");
            s.append(l);
            return s;
        }));//Complexity: O(last - first) applications of binary_op.

    return writeStrings(transform, sourceName, path);
}
