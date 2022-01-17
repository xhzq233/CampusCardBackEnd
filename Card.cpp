//
// Created by 夏侯臻 on 2022/1/14.
//

#include "Card.h"
Card::Card(int uid, string name, int serialNumber)
{
    this->uid = uid;
    this->name = name;
    this->balance = 0;
    this->date = defaultDate;
    int checkNode = 0;
    //校验码,卡号校验码的计算规则：前6位数字相加的和再模10，得到一个0-9的数，然后用9减去这个数，就是最后一位校验码
    for (int i = 0; i < 5; ++i)
    {
        checkNode += serialNumber % 10;
        serialNumber /= 10;
    }
    checkNode = 9 - (checkNode + 3) % 10;
    this->cid = (3000000 + serialNumber) * 10 + checkNode;
}
void Card::recharge(int amount)
{
    if (this->balance + amount > balanceCeiling)
    {
        ;
    }
    else
    {
        this->balance += amount;
    }
}