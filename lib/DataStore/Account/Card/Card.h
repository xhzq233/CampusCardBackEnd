//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CARD_H
#define CARD_H

#include "../../../Dependencies.h"

#define DEFAULT_DATE 20240715   //默认的卡号有效期
#define UID_LENGTH 10           //学号长度
#define DEFAULT_PASSWORD 123456 //默认的密码

class Card {
private:
    unsigned int password; //密码

public:
    unsigned int date;                                //有效日期
    unsigned int cid;                                 //卡号
    unsigned int uid;                                 //学号
    bool condition;                                   //卡的状态
    Card(unsigned int uid, unsigned int serialNumber);        // card的构造函数
    void changePassword();                            //修改密码

    //验证密码
    [[nodiscard]] inline bool checkPassword(unsigned int passwd) const {
        return this->password == passwd;
    }


    // from strings
    explicit Card(const std::vector<std::string> &strings) : Card(std::stoul(strings[0]), std::stoul(strings[1])) {}

    // to string
    [[nodiscard]] std::string to_string() const;
};

#endif // CARD_H