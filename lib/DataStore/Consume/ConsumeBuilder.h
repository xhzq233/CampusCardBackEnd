//
// Created by 夏侯臻 on 2022/2/22.
//

#ifndef CAMPUSCARDBACKEND_CONSUMEBUILDER_H
#define CAMPUSCARDBACKEND_CONSUMEBUILDER_H

#include "Consume.h"
#include "../../CardManage/CardManage.h"


namespace ConsumeBuilder {
    bool consume(const Window &window, const Card &card, const float &price, std::string time); //消费操作
    bool consume(const Window &window, const Card &card, const float &price);

    bool passwd_is_correct(const Card &card); //输入密码
}


#endif //CAMPUSCARDBACKEND_CONSUMEBUILDER_H
