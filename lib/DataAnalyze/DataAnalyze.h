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
    std::vector<unsigned int> fuzzyQueryOnUid(std::string &str); //模糊查询

    struct priority_value {
        unsigned int student_id;
        unsigned char count;

        bool operator<(const priority_value &rhs) const {
            return count < rhs.count;
        }
    };

    typedef std::array<priority_value, 5> k_min_students_res;

    float accumulatedConsumption(unsigned int uid, Time begin, Time end); //统计一个账户在指定时间范围内所有的消费记录总额

    [[nodiscard]] k_min_students_res analyze(unsigned int uid); //分析该名同学和哪些同学一起排队购餐

    std::pair<Window ,unsigned int> mostFrequentWindowOfSomeone(unsigned int uid);
};


#endif //CAMPUSCARDBACKEND_DATAANALYZE_H
