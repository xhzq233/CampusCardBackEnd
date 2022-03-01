#ifndef CAMPUSCARDBACKEND_CARDMANAGE_H
#define CAMPUSCARDBACKEND_CARDMANAGE_H

#include "../DataStore/DataStore.h"
#include "../FileManager/FileManager.h"

namespace CardManage {
    typedef unsigned long long Time;

    static const constexpr float BALANCE_CEILING = 999.99; //账户余额上限
    static const constexpr char MAX_REISSUE_TIMES = 100; // 最大补卡

    template<typename... Args>
    inline void log(const Time &time, const char *title, const char *format, Args &&...args) {
        static char buffer[90];
        sprintf(buffer, format, args...);
        if (!time) {
            FileManager::getInstance() << FileManager::toStandardLogString(title, buffer);
        } else {
            FileManager::getInstance() << FileManager::toStandardLogString(title, buffer, time);
        }
    }

    unsigned int openAccount(unsigned int uid, const std::string &name, const Time &time = 0); //开户

    bool deleteAccount(unsigned int uid, const Time &time = 0);             //销户

    int setLost(unsigned int uid, const Time &time = 0);                   //挂失

    int unsetLost(unsigned int uid, const Time &time = 0);                 //解挂

    int reissue(unsigned int uid, const Time &time = 0);                   //补卡

    int recharge(unsigned int uid, int amount, const Time &time = 0) noexcept;    //充值

    void recall();
}

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H