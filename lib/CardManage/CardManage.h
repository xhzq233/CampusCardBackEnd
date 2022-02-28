//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CAMPUSCARDBACKEND_CARDMANAGE_H
#define CAMPUSCARDBACKEND_CARDMANAGE_H

#include "../DataStore/DataStore.h"
#include "../FileManager/FileManager.h"

namespace CardManage {
    typedef unsigned long long Time;
    typedef std::function<void(char *)> BufferCallBack;

    static const constexpr float BALANCE_CEILING = 999.99; //账户余额上限
    static const constexpr char MAX_REISSUE_TIMES = 100; // 最大补卡

    void log(const char *title, const char *content, const Time &time); //日志记录

    unsigned int openAccount(unsigned int uid, const std::string &name, const Time &time = 0); //开户

    bool deleteAccount(unsigned int uid, const Time &time = 0);             //销户

    int setLost(unsigned int uid, const Time &time = 0);                   //挂失

    int unsetLost(unsigned int uid, const Time &time = 0);                 //解挂

    int reissue(unsigned int uid, const Time &time = 0);                   //补卡

    int recharge(unsigned int uid, int amount, const Time &time = 0) noexcept;    //充值

    void recall();                                                                  //日志回溯
    void log(const char *title, const BufferCallBack &bufferCallBack, const Time &time);
}

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H