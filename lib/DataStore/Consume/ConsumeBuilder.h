//
// Created by 夏侯臻 on 2022/2/22.
//

#ifndef CAMPUSCARDBACKEND_CONSUMEBUILDER_H
#define CAMPUSCARDBACKEND_CONSUMEBUILDER_H

#include "Consume.h"
#include "../../CardManage/CardManage.h"

namespace ConsumeBuilder {
    static void consume(Card &card, float price, int date, int time); //消费操作
    static bool consume(Card &card, float price);
    static bool passwd_is_correct(Card &card); //输入密码
};
//=======
//#include "../Account/Card/Card.h"
//#include "../DataStore.h"
//
//namespace ConsumeBuilder {
//    bool consume(const Window& window, const Card &card,const float& price, int date, int time); //消费操作
//    bool consume(const Window& window, const Card &card,const float& price);
//
//    bool passwd_is_correct(const Card &card); //输入密码
//}
//>>>>>>> d2f4c8eee9ddb6bf75a808ec8a5119ad449fc78b


#endif //CAMPUSCARDBACKEND_CONSUMEBUILDER_H
