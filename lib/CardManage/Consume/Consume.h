//
// Created by 夏侯臻 on 2022/2/22.
//

#ifndef CAMPUSCARDBACKEND_CONSUME_H
#define CAMPUSCARDBACKEND_CONSUME_H

#include "../../DataStore/Operation/Consumption.h"
#include "../CardManage.h"


namespace Consume {
    typedef unsigned long long Time;
    static constexpr const Time GAP_TIME = 2'00'00'00;

    int consume(const Window &window, unsigned int cid, const float &price, const Time &time); //指定时间消费

    int consume(const Window &window, unsigned int cid, const float &price); //当日消费

    void consume(CircularArray<Consumption *> *container,const Consumption * consumption); //通过消费记录消费

    void show(const Window &window, const Time &time); //消费完后展示指定时间消费次数和数据

    void show(const Window &window); //消费完后展示当日消费次数和数据

    bool checkPasswd(const Card &card); //输入密码

    inline void log(const Time &time, unsigned int cid, Window window, float price, const char *info) {
        static char content[72];
        sprintf(content, "%d window %d price %.2f %s", cid, window, price, info);
        FileManager::getInstance() << FileManager::toStandardLogString("消费", content, time);
    }
}


#endif //CAMPUSCARDBACKEND_CONSUME_H
