//
// Created by 夏侯臻 on 2022/2/21.
//

#include "Account.h"

Account::Account(unsigned int uid, const std::string &name) : uid(uid), name(std::move(name)), balance(0), cards({}) {}

Account::~Account() {

}

inline
void Account::consume(float price) {
    this->balance -= price;
}

inline
void Account::recharge(float amount) {
    this->balance += amount;
}

