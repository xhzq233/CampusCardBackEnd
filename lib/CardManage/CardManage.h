//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CAMPUSCARDBACKEND_CARDMANAGE_H
#define CAMPUSCARDBACKEND_CARDMANAGE_H

#include "../DataStore/DataStore.h"
#include "../FileManager/FileManager.h"

namespace CardManage {
    typedef unsigned long long Time;
    static unsigned int serialNumber = 12345;                                    //流水号
    static const constexpr float BALANCE_CEILING = 999.99; //账户余额上限
    void log(const char *title, const std::string &content, const Time &time); //日志记录

    std::string to_string(unsigned int uid, const std::string &name, const std::string &info);

    void openAccount(unsigned int uid, const std::string &name, const Time &time = 0); //开户

    void deleteAccount(unsigned int uid, const Time &time = 0);             //销户

    void distribute(unsigned int uid, const Time &time = 0);                //发卡

    void setLost(unsigned int uid, const Time &time = 0);                   //挂失

    void unsetLost(unsigned int uid, const Time &time = 0);                 //解挂

    void reissue(unsigned int uid, const Time &time = 0);                   //补卡

    void recharge(unsigned int uid, float amount, const Time &time = 0);    //充值

    void recall();                                                                  //日志回溯

    void operateByFile();                                                           //批量操作

};

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H