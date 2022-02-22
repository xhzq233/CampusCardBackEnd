//
// Created by 夏侯臻 on 2022/2/22.
//

#ifndef CAMPUSCARDBACKEND_CONSUMEBUILDER_H
#define CAMPUSCARDBACKEND_CONSUMEBUILDER_H

#include "Consume.h"
#include "../Account/Card/Card.h"

namespace ConsumeBuilder {
    static bool consume(Card &card, float price, int date, int time); //消费操作
    static bool consume(Card &card, float price);
    static bool passwd_is_correct(Card &card); //输入密码
};


#endif //CAMPUSCARDBACKEND_CONSUMEBUILDER_H
