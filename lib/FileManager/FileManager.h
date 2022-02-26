//
// Created by 夏侯臻 on 2022/1/14.
//

#ifndef CAMPUSCARDBACKEND_FILEMANAGER_H
#define CAMPUSCARDBACKEND_FILEMANAGER_H

#include "../Dependencies.h"

/*
 * Singleton FileManager
 * */
class FileManager {
public:

    //linux or unix
#ifndef __WIN64
    /* 默认数据路径 */
    constexpr static const char DEFAULT_DATA_PATH[] = "../data/";
    /* 默认log路径 */
    constexpr static const char DEFAULT_LOG_PATH[] = "../log/";
    /* xf.csv基于位置分成了多个CSV文件，格式为W{\d}.csv */
    constexpr static const char CONSUME_CSV_DICTIONARY[] = "xf/";
    /* xf.csv总文件 */
    constexpr static const char CONSUME_CSV_NAME[] = "xf/xf.csv";

    //windows amd64
#else
    /* 默认数据路径 */
    constexpr static const char DEFAULT_DATA_PATH[] = "..\\data\\";
    /* 默认log路径 */
    constexpr static const char DEFAULT_LOG_PATH[] = "..\\log\\";
    /* xf.csv基于位置分成了多个CSV文件，格式为W{\d}.csv */
    constexpr static const char CONSUME_CSV_DICTIONARY[] = "xf\\";
    /* xf.csv总文件 */
    constexpr static const char CONSUME_CSV_NAME[] = "xf\\xf.csv";
#endif //__WIN64

    /* 批量开户申请数据文件（kh001.txt）*/
    constexpr static const char OPEN_ACCOUNT_NAME[] = "kh001.txt";
    /* 批量卡片操作申请数据文件（cz002.txt）*/
    constexpr static const char CARD_MANAGE_NAME[] = "cz002.txt";
    /* 食堂窗口收费记录保存的当前位置数据文件（wz003.txt）*/
    constexpr static const char WINDOW_POSITION_NAME[] = "wz003.txt";
    /* 批量消费申请的数据文件（xf014.txt）*/
    constexpr static const char CONSUME_NAME[] = "xf014.txt";
    /* 对应上面的转换的CSV文件 */
    constexpr static const char OPEN_ACCOUNT_CSV_NAME[] = "kh.csv";
    // recharge , 4 columns
    constexpr static const char CARD_RECHARGE_CSV_NAME[] = "cz.csv";
    // manage , 3 columns
    constexpr static const char CARD_MANAGE_CSV_NAME[] = "cz3.csv";
    constexpr static const char CAFE_POSITION_CSV_NAME[] = "wz.csv";

    /* W{\d}.csv 文件个数 */
    constexpr static const char CONSUME_CSV_QTY = 58;

    /* .log file max length */
    constexpr static const unsigned short MAX_LINE_PER_LOG = 1U << 14;

    /*
     * Get WZ CSV file path based on location ,
     * the reason why I separate xf.csv ,
     * first is making file MultiThread able ,
     * second is it can optimize subsequent operation
     * */
    static std::string CONSUME_CSV(unsigned int position);

    typedef const std::function<void(std::fstream &stream)> &StreamCallBack;
    typedef std::vector<std::vector<std::string>> CSV;
    typedef std::vector<std::string> Strings;
    typedef unsigned long long Time;
private:
    /* type of std::ios::openmode, different os have different type */
#ifdef __WIN64
    typedef std::ios_base::openmode openmode;
#else
    typedef unsigned int openmode;
#endif

    /*
     * use lambda function simply wrap the process of prepare IOStream.
     * note that stream need to be referenced to avoid unnecessary copied memory.
     * */
    bool prepareIOStream(StreamCallBack func, const std::string &path, const std::string &source,
                         openmode mode = std::ios::in);

    // init shared instance
    // that function called if and only if getInstance() called
    // and only called once during the whole program lifetime
    static FileManager &shared_init();

public:
    // multi thread
    // not multi thread please use getInstance instead
    FileManager() = default;

    ~FileManager() = default;

    /*
     * instance initialized time，used on naming log data
     * notice that default constructor not have startUpTime
     * only shared instance have this value
     * */
    std::string startUpTime;

    // delete copy methods
    FileManager(const FileManager &) = delete;

    // disable copied FileManager
    FileManager &operator=(const FileManager &) = delete;

    /*
     * get the only instance of [FileManager]
     * In **C++11**, it is thread safe. According to the standard, §6.7 [stmt.dcl] p4:
     * If control enters the declaration concurrently while the variable is being initialized,
     * the concurrent execution shall wait for completion of the initialization.
     * */
    static FileManager &getInstance();

    /* default logger */
    static std::ofstream &getLogger();

    /* IO */
    std::fstream IOStream;

    /*
     * datasource split by '\n'
     * - Parameters:
     *   - source: select the data source given above. DEFAULT is OPEN_ACCOUNT_NAME.
     *   - path: select the storing-data path. DEFAULT is DEFAULT_DATA_PATH.
     * - Return type: array of string
     * */
    bool getStringDataSourceByLine(Strings &container, const std::string &source = OPEN_ACCOUNT_NAME,
                                   const std::string &path = DEFAULT_DATA_PATH);

    /*
     * return the csv data which be like [[data]].
     * the first dimension is columns.
     * the second dimension is rows.
     * DEFAULT storing-data path is DEFAULT_DATA_PATH.
     * use getCSVDataSource(unsigned int rows, unsigned int columns) instead if size already known
     * */
    bool getCSVDataSource(CSV &container, unsigned int columns, const std::string &source,
                          const std::string &path = DEFAULT_DATA_PATH);

    /*
     * use pre defined size
     * */
    bool getCSVDataSource(CSV &container, unsigned int rows, unsigned int columns,
                          const std::string &source,
                          const std::string &path = DEFAULT_DATA_PATH);

    /* 向指定路径写入一行string，返回是否成功 */
    bool writeStringByLine(const std::string &content, const std::string &source,
                           const std::string &path,
                           openmode mode = std::ios::app);

    /* 向指定路径写入一串string，返回是否成功 */
    bool
    writeStrings(const Strings &container, const std::string &source, const std::string &path,
                 openmode mode = std::ios::app);

    /*
     * 向指定路径写入csv文件，
     * notice that csv file type should end with .csv
     * container format defined above
     * 返回是否成功 */
    bool writeCSVData(const CSV &container, const std::string &sourceName, const std::string &path = DEFAULT_DATA_PATH);

    /* log的简便形式 */
    friend void operator<<(FileManager &o, const std::string &content);

    static std::string toStandardLogString(const char *title, const char *content);
    static std::string toStandardLogString(const char *title, const std::string &content);
    /* 自定义时间的StandardLog */
    static std::string toStandardLogString(const char *title, const char *content, const Time &time);
    static std::string toStandardLogString(const char *title, const std::string &content, const Time &time);
    /* literally */
    static void append_standard_time(std::string &container, const Time &time);


    //now -> Time
    static Time nowTime();

    void refreshStartUpTime();
};

#endif //CAMPUSCARDBACKEND_FILEMANAGER_H
