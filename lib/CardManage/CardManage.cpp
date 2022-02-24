//
// Created by 夏侯臻 on 2022/1/14.
//

#include "CardManage.h"

using namespace std;

unsigned int CardManage::serialNumber = 12345;

//系统具备正常状态的学号、姓名等信息的，即属于开户状态
void CardManage::openAccount(unsigned int uid, const string &name, const string &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        DataStore::insertAccount(Account(uid, name));
        log("Manage", to_string(uid, name, " 开户:succeeded"), time);
    } else {
        log("Manage", to_string(uid, "非系统用户", " 开户:failed"), time);
    }
}

//删除学号等数据项，或进行标识，只有经过恢复开户后才能恢复到开户状态；
void CardManage::deleteAccount(unsigned int uid, const string &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account != DataStore::getAccounts().end()) {
        account->cards.clear();
        log("Manage", to_string(uid, account->name, " 销户:succeeded"), time);
        DataStore::getAccounts().erase(account);
    } else {
        log("Manage", to_string(uid, "非系统用户", "销户:failed 备注:该用户未开号"), time);
    }
}

//依据开户信息，初次分配唯一卡号；如果已经分配过卡号的，则属于补卡功能；
void CardManage::distribute(unsigned int uid, const string &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account != DataStore::getAccounts().end()) {
        Card card(uid, ++serialNumber);
        account->cards.push_front(card);
        log("Manage", to_string(uid, account->name, "发卡:succeeded"),
            time);
    } else {
        log("Manage", to_string(uid, "非系统用户", "发卡:failed"), time);
    }
}

//设置当前学号最新分配的卡号的卡片为禁用的状态；
void CardManage::setLost(unsigned int uid, const string &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        log("Manage", to_string(uid, "非系统用户", "挂失:failed"), time);
        return;
    }
    auto card = *account->cards.begin();
    if (card.condition) {
        card.condition = false;
        log("Manage", to_string(uid, account->name, "挂失:succeeded"),
            time);
    } else {
        log("Manage", to_string(uid, account->name, "挂失:failed"),
            time);
    }
}

//设置当前学号最新分配的卡号的卡片为正常的状态；
void CardManage::unsetLost(unsigned int uid, const string &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        log("Manage", to_string(uid, "非系统用户", "挂失:failed"), time);
        return;
    }
    auto card = *account->cards.begin();
    if (!card.condition) {
        card.condition = true;
        log("Manage", to_string(uid, account->name, "解挂:succeed"),
            time);
    } else {
        log("Manage", to_string(uid, account->name, "解挂:failed"),
            time);
    }
}

//为当前学号分配新的卡号，即发放新的校园卡；该学号关联的其他卡片同时全部处于挂失禁用状态；
void CardManage::reissue(unsigned int uid, const string &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        log("Manage", to_string(uid, "非系统用户", "补卡:failed"), time);
        return;
    }
        //最多只能补卡100次
    else if (account->cards.size() >= 100) {
        log("Manage", to_string(uid, "非系统用户", "补卡:failed 备注:补卡次数达到上限"), time);
        return;
    }
    Card card(uid, ++serialNumber);
    //如果之前有卡,将之前卡的余额给新补的卡
    //将之前卡的状态设置为禁用状态
    setLost(uid);
    account->cards.push_front(card);
    log("Manage", to_string(uid, "非系统用户", "补卡:succeeded"),
        time);
}

//为该学号账户充值；账户余额上限999.99元；
void CardManage::recharge(unsigned int uid, float amount, const string &time) {
    auto account = DataStore::queryAccountByUid(uid);
    if (account == DataStore::getAccounts().end()) {
        log("Manage", to_string(uid, "非系统用户", "充值:failed"), time);
        return;
    } else {
        auto card = *account->cards.begin();
        if (account->balance + amount > BALANCECEILING) {
            log("Manage",
                to_string(uid, "非系统用户", "充值:failed 备注:卡内余额达到上限"),
                time);
        } else {
            string content = "充值:succeeded 卡号: ";
            content.append(std::to_string(card.cid));
            content.append(" 充值前余额:");
            content.append(std::to_string(account->balance));
            content.append(" 充值后余额:");
            content.append(std::to_string(account->balance + amount));
            log("Manage", to_string(uid, account->name, content), time);
            account->recharge(amount);
        }
    }
}

//日志回溯
void CardManage::recall() {
}

void CardManage::log(const char *title, const string &content, const string &time) {
    if (time.empty()) {
        FileManager::getInstance() << FileManager::toStandardLogString(title, content.c_str()) << FileManager::endl;
    } else {
        FileManager::getInstance() << FileManager::toStandardLogString(title, content.c_str(), stol(time))
                                   << FileManager::endl;
    }
}

//根据文件cz002.txt操作
void CardManage::operateByFile() {
    vector<string> container;
    FileManager::getInstance().getStringDataSourceByLine(container, FileManager::CARD_MANAGE_NAME);
    for (auto &&info: container) {
        int uid = stoi(info.substr(info.find_last_of(',') + 1, UID_LENGTH + 1));
        string time = info.substr(0, info.find(','));
        if (info.find("挂失") != string::npos) {
            setLost(uid, time);
        } else if (info.find("充值") != string::npos) {
            float amount = stof(info.substr(info.find_last_of(',') + 1, UID_LENGTH + 1));
            uid = stoi(info.substr(info.find("充值") + 5, 10).substr());
            recharge(uid, amount, time);
        } else if (info.find("销户") != string::npos) {
            deleteAccount(uid, time);
        } else if (info.find("解挂") != string::npos) {
            unsetLost(uid, time);
        }
    }
}

string CardManage::to_string(unsigned int uid, const string &name, const string &info) {
    string content;
    content.append(std::to_string(uid));
    content.append(" ");
    content.append(name);
    content.append(" ");
    content.append(info);
    return content;
}

