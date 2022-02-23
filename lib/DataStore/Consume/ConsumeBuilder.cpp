//
// Created by 夏侯臻 on 2022/2/22.
//

#include "ConsumeBuilder.h"

using namespace std;

bool ConsumeBuilder::consume(const Window &window, const Card &card, const float &price) {
    return false;
}

bool ConsumeBuilder::consume(const Window &window, const Card &card, const float &price, string time) {
    DataStore::insertConsume(window, new Consume(card.cid, window, move(time), price));
}
void ConsumeBuilder::consume(const Consume log) {

}
bool ConsumeBuilder::passwd_is_correct(const Card &card) {
    return card.checkPassword(0);
}