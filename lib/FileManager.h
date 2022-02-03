//
// Created by 夏侯臻 on 2022/1/14.
//

#ifndef CAMPUSCARDBACKEND_FILEMANAGER_H
#define CAMPUSCARDBACKEND_FILEMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <numeric>
#include <functional>

/*
 * Singleton FileManager
 * */
class FileManager {
public:
    /* 默认数据路径 */
    constexpr static const char DEFAULT_DATA_PATH[] = "../data/";
    /* 默认log路径 */
    constexpr static const char DEFAULT_LOG_PATH[] = "../log/";

    /* 批量开户申请数据文件（kh001.txt）*/
    constexpr static const char OPEN_ACCOUNT_NAME[] = "kh001.txt";
    /* 批量卡片操作申请数据文件（cz002.txt）*/
    constexpr static const char CARD_MANAGE_NAME[] = "cz002.txt";
    /* 食堂窗口收费记录保存的当前位置数据文件（wz003.txt）*/
    constexpr static const char CAFE_POSITION_NAME[] = "wz003.txt";
    /* 批量消费申请的数据文件（xf014.txt）*/
    constexpr static const char CONSUME_NAME[] = "xf014.txt";
    /* 测试数据文件（chk10.txt）*/
    constexpr static const char TEST_DATA_NAME[] = "chk10.txt";

    /* 对应上面的转换的CSV文件 */
    constexpr static const char OPEN_ACCOUNT_CSV_NAME[] = "kh.csv";
    constexpr static const char CARD_MANAGE_CSV_NAME[] = "cz.csv";
    constexpr static const char CAFE_POSITION_CSV_NAME[] = "wz.csv";
    /* xf.csv基于位置分成了多个CSV文件，格式为W{\d}.csv */
    constexpr static const char CONSUME_CSV_DICTIONARY[] = "xf/";

    /* 根据position获取wz CSV文件路径 */
    static std::string CONSUME_CSV(unsigned int position);

private:
    //make these constructors not accessible
    FileManager();

    /* instance 初始化的时间，用于给log data取名 */
    std::string startUpTime;

    ~FileManager() = default;

    /*
     * use lambda function simply wrap the process of prepare IOStream.
     * note that stream need to be referenced to avoid unnecessary copied memory.
     * */
    bool
    prepareIOStream(const std::function<void(std::fstream &)> &func, const std::string &path, const std::string &source,
                    char mode = std::ios::in);

public:
    //delete these copy methods
    FileManager(const FileManager &) = delete;

    FileManager &operator=(const FileManager &) = delete;

    /*
     * get the only instance of [FileManager]
     * In **C++11**, it is thread safe. According to the standard, §6.7 [stmt.dcl] p4:
     * If control enters the declaration concurrently while the variable is being initialized,
     * the concurrent execution shall wait for completion of the initialization.
     * */
    static FileManager &getInstance();

    /* IO管理 */
    std::fstream IOStream;

    /*
     * datasource split by '\n'
     * - Parameters:
     *   - source: select the data source given above. DEFAULT is OPEN_ACCOUNT_NAME.
     *   - path: select the storing-data path. DEFAULT is DEFAULT_DATA_PATH.
     * - Return type: array of string
     * */
    bool getStringDataSourceByLine(std::vector<std::string> &container, const std::string &source = OPEN_ACCOUNT_NAME,
                                   const std::string &path = DEFAULT_DATA_PATH);

    /*
     * return the csv data which be like [[data]].
     * the first dimension is columns.
     * the second dimension is rows.
     * DEFAULT storing-data path is DEFAULT_DATA_PATH.
     * */
    bool getCSVDataSource(std::vector<std::vector<std::string>> &container, const std::string &source,
                          const std::string &path = DEFAULT_DATA_PATH);

    /* 向指定路径写入一行string，返回是否成功 */
    bool writeStringByLine(const std::string &content, const std::string &source,
                           const std::string &path,
                           char mode = std::ios::app);

    /* 向指定路径写入一串string，返回是否成功 */
    bool writeStrings(std::vector<std::string> &container, const std::string &source,
                      const std::string &path,
                      char mode = std::ios::app);

    /*
     * 向指定路径写入csv文件，
     * container格式如上。
     * 返回是否成功 */
    bool writeCSVData(std::vector<std::vector<std::string>> &container, const std::string &sourceName,
                      const std::string &path = DEFAULT_DATA_PATH);

    /* literally */
    bool log(const std::string &content);

    bool logs(std::vector<std::string> &container);
};


#endif //CAMPUSCARDBACKEND_FILEMANAGER_H
