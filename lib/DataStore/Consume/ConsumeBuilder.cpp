//
// Created by 夏侯臻 on 2022/2/22.
//

#include "ConsumeBuilder.h"

using namespace std;

void ConsumeBuilder::consume(const Window &window, const Card &card, const float &price) {
    ;
}

void ConsumeBuilder::consume(const Window &window, const Card &card, const float &price, string time) {
    auto account = DataStore::queryByUid(card.cid);
    account->consume(price);
    DataStore::insertConsume(window, new Consume(card.cid, window, time, price));
}

void ConsumeBuilder::consume(const Consume &log) {
    auto account = DataStore::queryByUid(log.cid);
    account->consume(log.price);
}

bool ConsumeBuilder::passwd_is_correct(const Card &card) {
    return card.checkPassword(0);
}