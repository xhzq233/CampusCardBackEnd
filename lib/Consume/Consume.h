//
// Created by 夏侯臻 on 2022/2/22.
//

#ifndef CAMPUSCARDBACKEND_CONSUME_H
#define CAMPUSCARDBACKEND_CONSUME_H

#include "../DataStore/Consumption/Consumption.h"
#include "../CardManage/CardManage.h"


namespace Consume {
    void consume(const Window &window, const Card &card, const float &price, const std::string &time); //消费操作
    void consume(const Window &window, const Card &card, const float &price); //当日消费

    void consume(const Consumption &log); //通过消费记录消费
    void show(const Window &window, const std::string &time); //消费完后展示当日消费次数和数据
    void show(const Window &window);

    bool passwd_is_correct(const Card &card); //输入密码
}


#endif //CAMPUSCARDBACKEND_CONSUME_H
