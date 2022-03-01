#ifndef CAMPUSCARDBACKEND_DATAANALYZE_H
#define CAMPUSCARDBACKEND_DATAANALYZE_H

#include "../Dependencies.h"
#include "../DataStore/DataStore.h"
/*
 * 汇总分析模块
 *
 * 按时间递增顺序汇总各食堂窗口收费设备的消费记录
 * 提供模糊的学号、姓名信息进行查询
 * 对指定的学号统计给定起止日期内的累计消费金额
 * */
namespace DataAnalyze {
    using Time = DataStore::Time;

    /* return student uid matched */
    std::vector<unsigned int> fuzzyQueryOnUid(const std::regex &re); //模糊查询

    /* return student uid matched */
    std::vector<unsigned int> fuzzyQueryOnName(const std::regex &re); //模糊查询

    /**
    * #模糊匹配的格式中，？代表一个字符或一个汉字，*表示多个字符或多个汉字，或代表空；
    * 汉字：[\u4e00-\u9fa5]
    * 将?替换成 .
    * 将*替换成 .{2,}
   * */
    static std::regex customRegex2CommonRegexSyntax(std::string &regex) {
        regex.replace(regex.find('?'), 1, ".");
        regex.replace(regex.find('*'), 1, ".{2,}");
        return std::regex(regex);
    }


    float accumulatedConsumption(unsigned int uid, Time begin, Time end); //统计一个账户在指定时间范围内所有的消费记录总额
};


#endif //CAMPUSCARDBACKEND_DATAANALYZE_H
