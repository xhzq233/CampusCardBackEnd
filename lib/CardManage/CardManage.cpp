//
// Created by 夏侯臻 on 2022/1/14.
//

#include "CardManage.h"

using namespace std;

inline void not_in_sys(const char *title, unsigned int &uid, const FileManager::Time &time) {
    CardManage::log(title, [&](auto buffer) {
        sprintf(buffer, "%d failed: NOT IN SYS", uid);
    }, time);
}

inline void success(const char *title, const char *name, unsigned int &uid, const FileManager::Time &time) {
    CardManage::log(title, [&](auto buffer) {
        sprintf(buffer, "%d %s succeeded", uid, name);
    }, time);
}

//系统具备正常状态的学号、姓名等信息的，即属于开户状态
unsigned int CardManage::openAccount(unsigned int uid, const string &name, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        Account newAccount(uid, name, ++serialNumber);
        DataStore::insertAccount(newAccount);
        return newAccount.cards.begin().cid;
        //log inside account constructor
    } else {
        not_in_sys("开户", uid, time);
        return 0;
    }
}

//删除学号等数据项，或进行标识，只有经过恢复开户后才能恢复到开户状态；
bool CardManage::deleteAccount(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account != DataStore::getAccounts().end()) {
        account->cards.clear();
        success("销户", account->name, uid, time);
        DataStore::getAccounts().erase(account);
        return true;
    } else {
        not_in_sys("销户", uid, time);
        return false;
    }
}

//设置当前学号最新分配的卡号的卡片为禁用的状态；
int CardManage::setLost(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        not_in_sys("挂失", uid, time);
        return 0;
    } else {
        auto &card = account->cards.begin();
        if (card.condition) {
            card.condition = false;
            success("挂失", account->name, uid, time);
            return 1;
        } else {
            CardManage::log("挂失", [&](auto buffer) {
                sprintf(buffer, "%d %s failed: Already lost", uid, account->name);
            }, time);
        }
        return -1;
    }
}

//设置当前学号最新分配的卡号的卡片为正常的状态；
int  CardManage::unsetLost(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        not_in_sys("解挂", uid, time);
        return 0;
    } else {
        auto &card = account->cards.begin();
        if (!card.condition) {
            card.condition = true;
            success("解挂", account->name, uid, time);
            return 1;
        } else {
            CardManage::log("解挂", [&](auto buffer) {
                sprintf(buffer, "%d %s failed: not lost yet", uid, account->name);
            }, time);
            return -1;
        }
    }
}

//为当前学号分配新的卡号，即发放新的校园卡；该学号关联的其他卡片同时全部处于挂失禁用状态；
int CardManage::reissue(unsigned int uid, const Time &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        not_in_sys("补卡", uid, time);
        return 0;
    }
        //最多只能补卡100次
    else if (account->cards.size() >= CardManage::MAX_REISSUE_TIMES) {
        // reference:
        // https://sites.google.com/site/wyylview/dong-tai-fen-pei-nei-cun-zai-ke-nengheap-corruption-detected-de-yuan-yin-zhi-yi-2
        CardManage::log("补卡", [&](auto buffer) {
            sprintf(buffer, "%d %s failed: Reached upper limit", uid, account->name);
        }, time);
        return -1;
    } else {
        Card card(uid, ++serialNumber);
        //将之前卡的状态设置为禁用状态
        account->cards.push(card);
        success("补卡", account->name, uid, time);
        return 1;
    }
}

//为该学号账户充值；账户余额上限999.99元；
int CardManage::recharge(unsigned int uid, int amount, const Time &time) noexcept {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        not_in_sys("充值", uid, time);
        return 0;
    } else {
        auto &card = account->cards.begin();
        if (account->balance + (float) amount > BALANCE_CEILING) {
            CardManage::log("充值", [&](auto buffer) {
                sprintf(buffer, "%d failed: Reached upper limit", uid);
            }, time);
            return -1;
        } else {
            log("充值", [&](auto buffer) {
                sprintf(buffer, "%d %s success (cid: %d elder %.2f new %.2f)", uid, account->name, card.cid,
                        account->balance,
                        account->balance + (float) amount);
            }, time);
            account->recharge((float) amount);
            return (int)(account->balance);
        }
    }
}

//日志回溯
void CardManage::recall() {
}

void CardManage::log(const char *title, const BufferCallBack &bufferCallBack, const Time &time) {
    static char buffer[90];
    bufferCallBack(buffer);
    if (!time) {
        FileManager::getInstance() << FileManager::toStandardLogString(title, buffer);
    } else {
        FileManager::getInstance() << FileManager::toStandardLogString(title, buffer, time);
    }
}