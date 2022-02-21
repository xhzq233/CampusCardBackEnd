//
// Created by 夏侯臻 on 2022/2/21.
//

#include "Account.h"


Account::Account(unsigned int uid, std::string name) : uid(uid), name(std::move(name)), balance(0), cards({}) {}

Account::~Account() = default;


void Account::consume(float price) {
    this->balance -= price;
}


void Account::recharge(float amount) {
    this->balance += amount;
}

