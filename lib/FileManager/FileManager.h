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
    /* xf.csv总文件 */
    constexpr static const char CONSUME_CSV_NAME[] = "xf/xf.csv";
    /* W{\d}.csv 文件个数 */
    constexpr static const char CONSUME_CSV_QTY = 58;

    /*
     * Get WZ CSV file path based on location ,
     * the reason why I separate xf.csv ,
     * first is making file MultiThread able ,
     * second is it can optimize subsequent operation
     * */
    static std::string CONSUME_CSV(unsigned int position);

    /* 终止符0x11 */
    constexpr static const char endl = 0x11;

    typedef const std::function<void(std::fstream &stream)> &StreamCallBack;
    typedef std::vector<std::vector<std::string>> CSV;
    typedef std::vector<std::string> Strings;
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

    //暂时储存log 的buffer
    std::string stringLogBuf;

    unsigned int bufferLength = 0x00;
    static constexpr const unsigned int MAX_BUFFER_LENGTH = 1000;

    // init shared instance
    // that function called if and only if getInstance() called
    // and only called once during the whole program lifetime
    static FileManager &shared_init();

public:
    // multi thread
    // not multi thread please use getInstance instead
    FileManager() = default;

    ~FileManager();

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

    /* IO管理 */
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
     * use Pair<unsigned int, unsigned int> size instead if size already known
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

    /* literally */
    bool log(const std::string &content);

    bool logs(const Strings &container);

    /* log的简便形式 */
    friend FileManager &operator<<(FileManager &o, const std::string &content);

    /* 终止符0x11 which defined above */
    friend void operator<<(FileManager &o, char);

    static std::string toStandardLogString(const char *title, const char *content);

    /* 自定义时间的StandardLog */
    static std::string toStandardLogString(const char *title, const char *content, const time_t &now);

    /* literally */
    static void append_standard_time(std::string &container, const time_t &now);

    class DataQuery {
    public:
        /**
         * #模糊匹配的格式中，？代表一个字符或一个汉字，*表示多个字符或多个汉字，或代表空；
         * 汉字：[\u4e00-\u9fa5]
         * 将?替换成 .
         * 将*替换成 .{2,}
        * */
        static std::regex customRegex2CommonRegexSyntax(std::string &regex);

        // 数组下标
        typedef std::vector<unsigned int> Subscripts;

        // query on specified multi string，return Subscripts matched
        static Subscripts query(Strings &container, const std::regex &regex);

        // query on CSV data specified column, return Subscripts matched
        static Subscripts query(CSV &container, unsigned int columnIndex, const std::regex &regex);
    };

};

#endif //CAMPUSCARDBACKEND_FILEMANAGER_H
