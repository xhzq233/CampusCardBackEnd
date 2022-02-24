//
// Created by 夏侯臻 on 2022/2/22.
//

#include "Consume.h"

using namespace std;

void Consume::consume(const Window &window, const Card &card, const float &price) {
    ;
}


void Consume::consume(const Window &window, const Card &card, const float &price, const string& time) {
    auto account = DataStore::queryAccountByUid(card.cid);
    account->consume(price);
    DataStore::insertConsumption(window, new Consumption(card.cid, window, time, price));
}


void Consume::consume(const Consumption &log) {
    auto account = DataStore::queryAccountByUid(log.cid);
    account->consume(log.price);
}



bool Consume::passwd_is_correct(const Card &card) {
    return card.checkPassword(0);
}