//
// Created by 夏侯臻 on 2022/1/14.
//
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
        sprintf(buffer, format, args...);// expand parameter pack
        if (!time) { // time equals zero
            FileManager::getInstance() << FileManager::toStandardLogString(title, buffer);
        } else {
            FileManager::getInstance() << FileManager::toStandardLogString(title, buffer, time);
        }
    }

    /* default time is now */

    /// return card id if success, else zero
    [[nodiscard]] int openAccount(unsigned int uid, const std::string &name, const Time &time = 0);

    /// return cancellation result
    [[nodiscard]] bool account_cancellation(unsigned int uid, const Time &time = 0);

    /// 0 if not in sys, 1 if success, 2 if already lost
    [[nodiscard]] int setLost(unsigned int uid, const Time &time = 0);

    /// 0 if not in sys, 1 if success, 2 if not lost yet
    [[nodiscard]] int unsetLost(unsigned int uid, const Time &time = 0);

    /// return new card id if success, return zero if not in sys, -1 if Reached upper limit
    [[nodiscard]] int reissue(unsigned int uid, const Time &time = 0);

    /// return updated balance if success, return zero if not in sys, -1 if Reached upper limit
    [[nodiscard]] int recharge(unsigned int uid, int amount, const Time &time = 0);
}

#endif // CAMPUSCARDBACKEND_CARDMANAGE_H