//
// Created by 夏侯臻 on 2022/2/21.
//

#include "Account.h"

using namespace std;

Account::Account(unsigned int uid, string name) : uid(uid), name(move(name)), balance(0), cards({}) {}

Account::~Account() = default;


void Account::consume(float price) {
    this->balance -= price;
}


void Account::recharge(float amount) {
    this->balance += amount;
}


string Account::to_string() const {
    return "学号:" + ::to_string(this->uid) + " 姓名:" + this->name;
}



//=======
//std::string Account::to_string() const  {
//    std::string res;
//    res.append(std::to_string(uid));
//    res.append(",");
//    res.append(name);
//    return res;
//}
//
//>>>>>>> 0a399846850bdfda9630199804ef05ba580d9947
