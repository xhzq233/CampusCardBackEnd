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
    time_t now = time(nullptr);
    auto *tm = std::localtime(&now);
    char buf[30];
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S", tm);
    instance.startUpTime.append(buf);
    return instance;
}

bool FileManager::prepareIOStream(StreamCallBack func, const std::string &path,
                                  const std::string &source,
                                  const openmode mode) {
#ifndef __WIN64
    IOStream.open(path + source, mode);
    if (!IOStream.is_open()) {/* check the param [path] , auto mkdir if not exist */
        IOStream.close();
        system(("mkdir " + path).c_str());
        IOStream.open(path + source, mode);
        if (!IOStream.is_open()) {
            //read failed
            IOStream.close();
            printf("%s read failed\n", (path + source).c_str());
            return false;
        }
    }
#else // windows amd64
    std::string file = path + source;
    IOStream.open(file, mode);
    if (!IOStream.is_open()) {/* check the param [path] , auto mkdir if not exist */
        IOStream.close();
        system(("mkdir " + path.substr(0, path.size() - 1)).c_str());
        IOStream.open(file, mode);
        if (!IOStream.is_open()) {
            //read failed
            IOStream.close();
//            std::cout << file << " read failed" << std::endl;
            printf("%s load failed.", file.c_str());
            return false;
        }
    }
#endif //__WIN64

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

bool FileManager::getCSVDataSource(CSV &container, unsigned int rows, unsigned int columns,
                                   const std::string &source, const std::string &path) {
    // decrease copied data as possible
    // use pre defined size
    container.resize(rows, std::vector<std::string>(columns, ""));
    return prepareIOStream([&](std::fstream &stream) {

        std::string rowBuffer;//a csv row implemented in string
        unsigned int col = 0;
        for (int i = 0; i < rows; ++i) {
            std::getline(stream, rowBuffer);

            for (auto &c: rowBuffer) {
                if (c == ',' || c == '\0' || c == '\r') {
                    col++;
                } else {
                    container[i][col].push_back(c);
                }
            }
            col = 0;
        }
    }, path, source);
}

bool FileManager::getCSVDataSource(CSV &container, unsigned int columns,
                                   const std::string &source, const std::string &path) {
    return prepareIOStream([&](std::fstream &stream) {

        std::string rowBuffer;//a csv row implemented in string

        unsigned int row = 0;
        unsigned int col = 0;
        while (std::getline(stream, rowBuffer)) {
            // decrease copied data as possible
            container.emplace_back(Strings(columns, ""));
            for (auto &c: rowBuffer) {
                if (c == ',' || c == '\0' || c == '\r') {
                    col++;
                } else {
                    container[row][col].push_back(c);
                }
            }
            col = 0;
            row++;
        }

    }, path, source);
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
    FileManager::getLoggerBuffer().append(content);
    return o;
}

void operator<<(FileManager &o, const char c) {

    if (FileManager::getLoggerBuffer().empty()) return; // return directly if empty

    if (c == FileManager::endl) {
        FileManager::getLogger() << FileManager::getLoggerBuffer() << '\n';
        FileManager::getLoggerBuffer().clear();//输出完后清空
    }
}

std::string FileManager::toStandardLogString(const char *title, const char *content) {
    time_t now = time(nullptr);
    std::string res;
    res.push_back('[');
    auto *tm = std::localtime(&now);
    char buf[25];
    strftime(buf, sizeof(buf), "%Y-%m-%d-%X", tm);
    res.append(buf);
    res.append(" : ");
    res.append(title);
    res.push_back(']');
    res.append(content);
    return res;
}

std::string FileManager::toStandardLogString(const char *title, const char *content, const Time &time) {
    std::string res;
    res.push_back('[');
    append_standard_time(res, time);
    res.append(" : ");
    res.append(title);
    res.push_back(']');
    res.append(content);
    return res;
}

void FileManager::append_standard_time(std::string &container, const Time &time) {
    char buf[22];
    sprintf(buf, "%llu-%llu-%llu-%llu-%llu:%llu:%llu", time / 1'000'000'000'000, time / 1'000'000'000'0 % 100,
            time / 10'000'000'0 % 100,
            time / 100'000'0 % 100, time / 1'000'0 % 100, time / 10'0 % 100,
            time % 100);
    container.append(buf);
}

std::ofstream &FileManager::getLogger() {
    //lambda return
    static std::ofstream &logger = [&]() -> std::ofstream & {
        static std::ofstream o;
        o.open(DEFAULT_LOG_PATH + getInstance().startUpTime + ".log", std::ios::app);
        return o;
    }();
    return logger;
}

std::string &FileManager::getLoggerBuffer() {
    static std::string res;
    return res;
}
