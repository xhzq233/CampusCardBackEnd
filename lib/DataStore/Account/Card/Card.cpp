//
// Created by 夏侯臻 on 2022/1/14.
//

#include "Card.h"

Card::Card(unsigned int serialNumber, unsigned int uid) : uid(uid), date(DEFAULT_DATE), condition(true),
                                                          password(DEFAULT_PASSWORD) {
    unsigned int checkNode = 0;
    //校验码,卡号校验码的计算规则：前6位数字相加的和再模10，得到一个0-9的数，然后用9减去这个数，就是最后一位校验码
    for (int i = 0; i < 5; ++i) {
        checkNode += serialNumber % 10;
        serialNumber /= 10;
    }
    checkNode = 9 - (checkNode + 3) % 10;
    this->cid = (3000000 + serialNumber) * 10 + checkNode;
}

void Card::changePassword() {
    if (this->condition) {
        std::cin >> this->password;
    }
}

void Card::consume(float d) {

}

float Card::getBalance() {
    return 0;
}

std::string Card::to_string() const {
    std::string res;
    res.append(std::to_string(uid));
    res.append(",");
    res.append(std::to_string(cid));
    return res;
}