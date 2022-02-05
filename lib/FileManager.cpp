//
// Created by 夏侯臻 on 2022/1/14.
//


#include "FileManager.h"

FileManager::FileManager() {
    auto now = time(nullptr);
    auto *tm = std::localtime(&now) ;
    char buf[25];

    strftime(buf, sizeof(buf), "%Y-%m-%d-%X", tm);

    startUpTime.append(buf);
}

FileManager &FileManager::getInstance() {
    static FileManager instance;
    return instance;
}

bool FileManager::prepareIOStream(streamCallBack func, const std::string &path,
                                  const std::string &source,
                                  const char mode) {
    IOStream.open(path + source, mode);
    if (!IOStream.is_open()) {/* check the param [path] , auto mkdir if not exist */
        IOStream.close();
        system(("mkdir " + path).c_str());
        IOStream.open(path + source, mode);
        if (!IOStream.is_open()) {
            //read failed
            IOStream.close();
            return false;
        }
    }
    func(IOStream);

    //confirm IOStream steam closed
    IOStream.close();
    return true;
}

bool FileManager::getStringDataSourceByLine(std::vector<std::string> &container, const std::string &source,
                                            const std::string &path) {
    return prepareIOStream([&](std::fstream &stream) {

        std::string buffer;

        while (std::getline(stream, buffer))
            container.emplace_back(buffer);

    }, path, source);
}

std::string FileManager::CONSUME_CSV(unsigned int position) {
    std::string res(CONSUME_CSV_DICTIONARY);
    res.append("W");
    res.append(std::to_string(position));
    res.append(".csv");
    return res;
}

bool FileManager::getCSVDataSource(std::vector<std::vector<std::string>> &container, const std::string &source,
                                   const std::string &path) {
    return prepareIOStream([&](std::fstream &stream) {

        std::string rowBuffer;//a csv row implemented in string
        std::string metadata;//metadata in a csv row
        std::vector<std::string> row;

        while (std::getline(stream, rowBuffer)) {

            std::stringstream buf(rowBuffer);//turn to a stream type

            while (std::getline(buf, metadata, ','))//csv use ',' as the separator
                row.emplace_back(metadata);

            container.emplace_back(row);
            row.clear();
        }

    }, path, source);
}

bool FileManager::logs(std::vector<std::string> &container) {
    std::string logDataName = startUpTime + ".log";
    return writeStrings(container, logDataName, DEFAULT_LOG_PATH);
}

bool FileManager::log(const std::string &content) {
    std::string logDataName = startUpTime + ".log";
    return writeStringByLine(content, logDataName, DEFAULT_LOG_PATH);
}

bool FileManager::writeStringByLine(const std::string &content, const std::string &source, const std::string &path,
                                    char mode) {
    return prepareIOStream([&](std::fstream &stream) {
        stream << content << std::endl;
    }, path, source, mode);
}

bool
FileManager::writeStrings(std::vector<std::string> &container, const std::string &source, const std::string &path,
                          char mode) {
    return prepareIOStream([&](std::fstream &stream) {
        for (const auto &content: container)
            stream << content << std::endl;
    }, path, source, mode);
}

bool FileManager::writeCSVData(std::vector<std::vector<std::string>> &container, const std::string &sourceName,
                               const std::string &path) {
    std::vector<std::string> transform(container.size());//init with defined size
    for (int i = 0; i < container.size(); ++i)
        transform[i] = std::reduce(container[i].begin() + 1, container[i].end(), container[i][0],
                                   [](const std::string &r,//init 参数需要显示转换为string
                                      const std::string &l) -> std::string {
                                       std::string s(r);
                                       s.append(",");
                                       s.append(l);
                                       return s;
                                   });//Complexity: O(last - first) applications of binary_op.


    return writeStrings(transform, sourceName, path, std::ios::out | std::ios::trunc);
}

FileManager &operator<<(FileManager &o, const std::string &content) {
    o.stringLogBuf.append(content);
    return o;
}

void operator<<(FileManager &o, const char c) {

    if (o.stringLogBuf.empty()) return; // return directly if empty

    if (c == FileManager::endl) {
        o.log(o.stringLogBuf);//TODO: err handle
    }
    o.stringLogBuf.clear();//输出完后清空
}

std::string FileManager::toStandardLogString(const char *title, const std::string &content){
    time_t now = time(nullptr);
    std::string res{ctime(&now)};
    res.insert(res.begin(), '[');
    res.append(" : ");
    res.append(title);
    res.append(" ] ");
    res.append(content);
    return res;
}
