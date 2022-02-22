//
// Created by 夏侯臻 on 2022/1/14.
//


#include "FileManager.h"


FileManager &FileManager::getInstance() {
    // only call once here
    static FileManager &instance = shared_init();
    return instance;
}

FileManager &FileManager::shared_init() {
    static FileManager instance;
    append_standard_time(instance.startUpTime, time(nullptr));
    return instance;
}

bool FileManager::prepareIOStream(StreamCallBack func, const std::string &path,
                                  const std::string &source,
                                  const openmode mode) {
    IOStream.open(path + source, mode);
    if (!IOStream.is_open()) {/* check the param [path] , auto mkdir if not exist */
        IOStream.close();
        system(("mkdir " + path).c_str());
        IOStream.open(path + source, mode);
        if (!IOStream.is_open()) {
            //read failed
            IOStream.close();
            std::cout << path + source << " read failed" << std::endl;
            return false;
        }
    }
    func(IOStream);

    //confirm IOStream steam closed
    IOStream.close();
    return true;
}

bool FileManager::getStringDataSourceByLine(Strings &container, const std::string &source,
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

// 1937 rows used time: 4.27837 ms.
bool FileManager::getCSVDataSource(CSV &container,const Pair<unsigned int, unsigned int>& size,
                                   const std::string &source, const std::string &path) {
    // decrease copied data as possible
    container.resize(size.first, std::vector<std::string>());
    return prepareIOStream([&](std::fstream &stream) {

        std::string rowBuffer;//a csv row implemented in string
        std::string metadata;//metadata in a csv row

        for (int i = 0; i < size.first; ++i) {
            std::getline(stream, rowBuffer);
            std::stringstream buf(rowBuffer);//turn to a stream type
            container[i].reserve(size.second);
            while (std::getline(buf, metadata, ','))//csv use ',' as the separator
                container[i].emplace_back(metadata);
        }
    }, path, source);
}

// 1937 rows used time: 4.51425 ms.
bool FileManager::getCSVDataSource(CSV &container, unsigned int columnQty,
                                   const std::string &source, const std::string &path) {
    return prepareIOStream([&](std::fstream &stream) {

        std::string rowBuffer;//a csv row implemented in string
        std::string metadata;//metadata in a csv row
        unsigned int row = 0;
        while (std::getline(stream, rowBuffer)) {
            // decrease copied data as possible
            container.emplace_back(Strings(columnQty, ""));
            std::stringstream buf(rowBuffer);//turn to a stream type
            for (int i = 0; i < columnQty; ++i) {
                std::getline(buf, metadata, ',');
                container[row][i].append(metadata);
            }
            row++;
        }

    }, path, source);
}

bool FileManager::logs(Strings &container) {
    std::string logDataName(startUpTime + ".log");
    return writeStrings(container, logDataName, DEFAULT_LOG_PATH);
}

bool FileManager::log(const std::string &content) {
    std::string logDataName(startUpTime + ".log");
    return writeStringByLine(content, logDataName, DEFAULT_LOG_PATH);
}

bool FileManager::writeStringByLine(const std::string &content, const std::string &source, const std::string &path,
                                    const openmode mode) {
    return prepareIOStream([&](std::fstream &stream) {
        stream << content << std::endl;
    }, path, source, mode);
}

bool FileManager::writeStrings(const Strings &container, const std::string &source, const std::string &path,
                               const openmode mode) {
    return prepareIOStream([&](std::fstream &stream) {
        for (const auto &content: container)
            stream << content << std::endl;
    }, path, source, mode);
}

bool FileManager::writeCSVData(const CSV &container, const std::string &sourceName,
                               const std::string &path) {
    Strings transform(container.size());//init with defined size
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

std::string FileManager::toStandardLogString(const char *title, const char *content) {
    time_t now = time(nullptr);
    std::string res;
    res.append("[");
    append_standard_time(res, now);
    res.append(" : ");
    res.append(title);
    res.append("] ");
    res.append(content);
    return res;
}

std::string FileManager::toStandardLogString(const char *title, const char *content, const time_t &now) {
    std::string res;
    res.append("[");
    append_standard_time(res, now);
    res.append(" : ");
    res.append(title);
    res.append("] ");
    res.append(content);
    return res;
}

void FileManager::append_standard_time(std::string &container, const time_t &now) {
    auto *tm = std::localtime(&now);
    char buf[25];
    strftime(buf, sizeof(buf), "%Y-%m-%d-%X", tm);
    container.append(buf);
}

using DataQuery = FileManager::DataQuery;

std::regex DataQuery::customRegex2CommonRegexSyntax(std::string &regex) {
    regex.replace(regex.find('?'), 1, ".");
    regex.replace(regex.find('*'), 1, ".{2,}");
    return std::regex(regex);
}

DataQuery::Subscripts
DataQuery::query(FileManager::Strings &container, const std::regex &regex) {
    Subscripts res;
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (std::regex_match(container[i], regex))
            res.emplace_back(i);
    }
    return res;
}

DataQuery::Subscripts
DataQuery::query(FileManager::CSV &container, unsigned int columnIndex, const std::regex &regex) {

    Subscripts res;
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (std::regex_match(container[i][columnIndex], regex))
            res.emplace_back(i);
    }
    return res;
}
