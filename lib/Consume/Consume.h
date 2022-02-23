//
// Created by 夏侯臻 on 2022/2/22.
//

#ifndef CAMPUSCARDBACKEND_CONSUME_H
#define CAMPUSCARDBACKEND_CONSUME_H

#include "../DataStore/ConsumeLog/Consumption.h"
#include "../CardManage/CardManage.h"


namespace ConsumeBuilder {
    void consume(const Window &window, const Card &card, const float &price, std::string time); //消费操作
    void consume(const Window &window, const Card &card, const float &price);

    void consume(const Consumption &log);

    bool passwd_is_correct(const Card &card); //输入密码
}


#endif //CAMPUSCARDBACKEND_CONSUME_H
