//
// Created by 夏侯臻 on 2022/2/22.
//

#include "ConsumeBuilder.h"

using namespace std;

bool ConsumeBuilder::consume(const Window &window, const Card &card, const float &price) {
    return false;
}

bool ConsumeBuilder::consume(const Window &window, const Card &card, const float &price, string time) {
    DataStore::insertConsume(window, new Consume(card.cid, window, time, price));
}
void ConsumeBuilder::consume(const Consume log) {
   auto account = DataStore::queryByUid(log.cid);
   account->consume(log.price);
}
bool ConsumeBuilder::passwd_is_correct(const Card &card) {
    return card.checkPassword(0);
}