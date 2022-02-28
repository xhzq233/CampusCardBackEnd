//
// Created by 夏侯臻 on 2022/2/22.
//

#ifndef CAMPUSCARDBACKEND_CONSUME_H
#define CAMPUSCARDBACKEND_CONSUME_H

#include "../DataStore/Operation/Consumption.h"
#include "../CardManage/CardManage.h"


namespace Consume {
    typedef unsigned long long Time;
    static constexpr const Time GAP_TIME = 2'00'00'00;

    void baseConsume(Account &account, const Window &window, const Card &card, const float &price,
                     const Time &time); // 基础的消费行为

    void consume(const Window &window, const Card &card, const float &price, const Time &time); //指定时间消费

    void consume(const Window &window, unsigned int uid, const float &price, const Time &time); //指定时间消费

    void consume(const Window &window, const Card &card, const float &price); //当日消费

    void consume(const Window &window, unsigned int uid, const float &price); //当日消费

    void consume(const Consumption &log); //通过消费记录消费

    void show(const Window &window, const Time &time); //消费完后展示指定时间消费次数和数据

    void show(const Window &window); //消费完后展示当日消费次数和数据

    void log(const Time &time, unsigned int cid, Window window, float price, const char *info);

    bool checkPasswd(const Card &card); //输入密码
}


#endif //CAMPUSCARDBACKEND_CONSUME_H
