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
    typedef unsigned long long Time;

    std::vector<unsigned int> fuzzyQuery(const std::string & str); //模糊查询

    float accumulatedConsumption(unsigned int uid, Time begin, Time end); //统计一个账户在指定时间范围内所有的消费记录总额
};


#endif //CAMPUSCARDBACKEND_DATAANALYZE_H
