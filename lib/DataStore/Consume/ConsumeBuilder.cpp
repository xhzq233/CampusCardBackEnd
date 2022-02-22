//
// Created by 夏侯臻 on 2022/2/22.
//

#include "ConsumeBuilder.h"

//bool ConsumeBuilder::consume(const Window &window, const Card &card, const float &price) {
//    return false;
//}

void ConsumeBuilder::consume(Card &card, float price, int date, int time) {
    if (!card.condition) {
        return;
    }
    auto account = CardManage::queryByCid(card.cid);
    if (account->balance >= price) {
        int hour = time / 1000000;
        if (hour >= 7 && hour <= 9 || hour >= 11 && hour <= 13 || hour >= 17 || hour <= 19) {
            account->consume(price);
        } else if (passwd_is_correct(card)) {
            account->consume(price);
        } else {

        }
    } else { ;
    }
}

bool ConsumeBuilder::passwd_is_correct(Card &card) {
    return card.checkPassword(0);
}