//
// Created by 夏侯臻 on 2022/2/22.
//

#include "Consume.h"

using namespace std;

void Consume::consume(const Window &window, const Card &card, const float &price) {
    ;
}

void Consume::consume(const Window &window, const Card &card, const float &price, const string &time) {
    auto account = DataStore::queryByUid(card.cid);
    account->consume(price);
    DataStore::insertConsume(window, new ConsumeLog(card.cid, window, time, price));
}

void Consume::consume(const ConsumeLog &log) {
    auto account = DataStore::queryByUid(log.cid);
    account->consume(log.price);
}

void Consume::consumeByFile() {
    for (auto &&logs: DataStore::getConsumes()) {
        for (auto &&log: logs) {
            consume(*log);
        }
    };
}

bool Consume::passwd_is_correct(const Card &card) {
    return card.checkPassword(0);
}