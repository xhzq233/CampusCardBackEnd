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
    static const constexpr char MAX_REISSUE_TIMES = 100; // 最大补卡

    void log(const char *title, const char *content, const Time &time); //日志记录

    void openAccount(unsigned int uid, const std::string &name, const Time &time = 0); //开户

    void deleteAccount(unsigned int uid, const Time &time = 0);             //销户

    void distribute(unsigned int uid, const Time &time = 0);                //发卡

    void setLost(unsigned int uid, const Time &time = 0);                   //挂失

    void unsetLost(unsigned int uid, const Time &time = 0);                 //解挂

    void reissue(unsigned int uid, const Time &time = 0);                   //补卡

    void recharge(unsigned int uid, int amount, const Time &time = 0) noexcept;    //充值

    void recall();                                                                  //日志回溯

    inline void not_in_sys(const char *title, unsigned int &uid, const FileManager::Time &time) {
        auto buffer = new char[32];
        sprintf(buffer, "%d failed: NOT IN SYS", uid);
        CardManage::log(title, buffer, time);
    }

    inline void success(const char *title, const char *name, unsigned int &uid, const FileManager::Time &time) {
        auto buffer = new char[25];
        sprintf(buffer, "%d %s success", uid, name);
        CardManage::log(title, buffer, time);
    }
}

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H