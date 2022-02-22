//
// Created by 夏侯臻 on 2022/2/22.
//

#include "ConsumeBuilder.h"

bool ConsumeBuilder::consume(const Window &window, const Card &card, const float &price) {
    return false;
}

bool ConsumeBuilder::consume(const Window &window, const Card &card, const float &price, int date, int time) {
    return false;
}

bool ConsumeBuilder::passwd_is_correct(const Card &card) {
    //TODO: Mark@username-end
    return card.checkPassword(0);
}