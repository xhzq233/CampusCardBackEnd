//
// Created by 夏侯臻 on 2022/1/14.
//


#include "FileManager.h"

FileManager &FileManager::getInstance() {
    // only call once here
    static FileManager &instance = shared_init();
    return instance;
}

const std::string &FileManager::refreshedStartUpTime() {
    static unsigned int count = 0;
    static std::string startUpTime;
    startUpTime.clear();
    time_t now = time(nullptr);
    auto *tm = std::localtime(&now);
    char buf[30];
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S-", tm);
    startUpTime.append(buf);
    startUpTime.append(std::to_string(count));
    count++;
    return startUpTime;
}

FileManager &FileManager::shared_init() {
    static FileManager instance;
    std::ifstream ifs(DEFAULT_LOG_PATH);
    if (!ifs.is_open()) {/* check the param [path] , auto mkdir if not exist */
        ifs.close();
        system((std::string("mkdir ") + DEFAULT_LOG_PATH).c_str());
    }
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
            printf("%s read failed\n", (path + source).c_str());
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
                                       s.push_back(',');
                                       s.append(l);
                                       return s;
                                   });//Complexity: O(last - first) applications of binary_op.


    return writeStrings(transform, sourceName, path, std::ios::out | std::ios::trunc);
}

void operator<<(FileManager &o, const std::string &content) {
    if (content.empty()) return; // return directly if empty

    static unsigned int length = 0;

    FileManager::getLogger() << content << '\n';

    length++;

    if (length > FileManager::MAX_LINE_PER_LOG) {
        FileManager::getLogger(true); //remake
        length = 0;
    }
}

std::string FileManager::toStandardLogString(const char *title, const char *content) {
    time_t now = time(nullptr);
    auto *tm = std::localtime(&now);
    char buf[23];
    strftime(buf, sizeof(buf), "%Y-%m-%d-%X:00", tm);

    char buffer[100];
    sprintf(buffer, "[%s : %s] %s", buf, title, content);
    return {buffer};
}

std::string FileManager::toStandardLogString(const char *title, const std::string &content) {

    time_t now = time(nullptr);
    auto *tm = std::localtime(&now);
    char buf[23];
    strftime(buf, sizeof(buf), "%Y-%m-%d-%X:00", tm);

    char buffer[100];
    sprintf(buffer, "[%s : %s] %s", buf, title, content.c_str());
    // std::string.c_str() is not in heap so don't need to free
    return {buffer};
}

FileManager::Time FileManager::nowTime() {
    time_t now = time(nullptr);
    tm *t = localtime(&now);
    unsigned long long time = 0;
    time += t->tm_sec * 100;
    time += t->tm_min * 10'000;
    time += t->tm_hour * 1'000'000;
    time += static_cast<unsigned long long>(t->tm_mday) * 100'000'000;
    time += static_cast<unsigned long long>(t->tm_mon + 1) * 10'000'000'000;
    time += static_cast<unsigned long long>(t->tm_year + 1900) * 1'000'000'000'000;
    return time;
}

std::string FileManager::toStandardLogString(const char *title, const char *content, const Time &time) {
    std::string res;
    res.push_back('[');
    append_standard_time(res, time);
    res.append(" : ");
    res.append(title);
    res.append("] ");
    res.append(content);

    return res;
}

void FileManager::append_standard_time(std::string &container, const Time &time) {
    char buf[23];
    typedef unsigned char sub_time;

    sprintf(buf, "%d-%02d-%02d-%02d:%02d:%02d:%02d",
            (unsigned short) (time / 1'000'000'000'000),
            (sub_time) (time / 1'000'000'000'0 % 100),
            (sub_time) (time / 10'000'000'0 % 100),
            (sub_time) (time / 100'000'0 % 100),
            (sub_time) (time / 1'000'0 % 100),
            (sub_time) (time / 10'0 % 100),
            (sub_time) (time % 100));
    container.append(buf);
}

std::ofstream &FileManager::getLogger(bool reset) {
    static std::ofstream logger(DEFAULT_LOG_PATH + FileManager::refreshedStartUpTime() + ".log", std::ios::trunc);
    if (reset) {
        logger.close();
        logger.open(
                FileManager::DEFAULT_LOG_PATH + FileManager::refreshedStartUpTime() + ".log",
                std::ios::trunc);
    }
    return logger;
}
