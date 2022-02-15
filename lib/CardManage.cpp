//
// Created by 夏侯臻 on 2022/1/14.
//

#include "CardManage.h"

CardManage::CardManage()
{
    serialNumber = 12345;
    info = {};
    v = {};
}

//返回一个CardManage实例
CardManage *CardManage::getInstance()
{
    if (!instance)
    {
        instance = new CardManage();
    }
    return instance;
}

//系统具备正常状态的学号、姓名等信息的，即属于开户状态
void CardManage::openAccount(unsigned int uid, const char *name, const string &time = "")
{

    info.insert(make_pair(uid, std::move(name)));
    log("Manage", "学号:" + to_string(uid) + " 姓名" + name + " 开户:succeeded", time);
}

//删除学号等数据项，或进行标识，只有经过恢复开户后才能恢复到开户状态；
void CardManage::deleteAccount(unsigned int uid, const string &time = "")
{
    if (info.count(uid))
    {
        info.erase(uid);
        //将销户的所有卡号状态设置为禁用
        setLost(uid);
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + " 销户:succeeded", time);
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + " 销户:failed 备注:该用户未开号", time);
    }
}

//依据开户信息，初次分配唯一卡号；如果已经分配过卡号的，则属于补卡功能；
void CardManage::distribute(unsigned int uid, const string &time = "")
{
    if (info.count(uid) && !v.count(uid))
    {
        Card *card = new Card(uid, info[uid], ++serialNumber);
        list<Card *> *l = new list<Card *>({card});
        v.insert(make_pair(uid, l));
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 发卡:succeed", time);
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + " 发卡:failed", time);
    }
}

//设置当前学号最新分配的卡号的卡片为禁用的状态；
void CardManage::setLost(unsigned int uid, const string &time = "")
{
    if (!info.count(uid))
    {
        log("Manage", "学号:" + to_string(uid) + " 解挂:failed 备注:非系统用户", time);
        return;
    }
    Card *card = (*v[uid]->begin());
    if (card->condition)
    {
        card->condition = false;
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 挂失:succeed", time);
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 挂失:failed", time);
    }
}

//设置当前学号最新分配的卡号的卡片为正常的状态；
void CardManage::unsetLost(unsigned int uid, const string &time = "")
{
    if (!info.count(uid))
    {
        log("Manage", "学号:" + to_string(uid) + " 解挂:failed 备注:非系统用户", time);
        return;
    }
    Card *card = (*v[uid]->begin());
    if (!card->condition)
    {
        card->condition = true;
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 解挂:succeed", time);
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 解挂:failed", time);
    }
}

//为当前学号分配新的卡号，即发放新的校园卡；该学号关联的其他卡片同时全部处于挂失禁用状态；
void CardManage::reissue(unsigned int uid, const string &time = "")
{
    if (!info.count(uid))
    {
        log("Manage", "学号:" + to_string(uid) + " 补卡:failed 备注:非系统用户", time);
        return;
    }
    //最多只能补卡100次
    else if (v[uid]->size() >= 100)
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + " 补卡:failed 备注:补卡次数达到上限", time);
        return;
    }
    Card *card = new Card(uid, info[uid], ++serialNumber);
    //如果之前有卡,将之前卡的余额给新补的卡
    if (!v[uid]->empty())
    {
        card->recharge((*v[uid]->begin())->getBalance());
    }
    //将之前卡的状态设置为禁用状态
    setLost(uid);
    v[uid]->push_front(card);
    log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 补卡:succeeded", time);
}

//为该学号账户充值；账户余额上限999.99元；
void CardManage::recharge(unsigned int uid, unsigned int amount, const string &time = "")
{
    Card *card = (*v[uid]->begin()); //最新的卡
    if (!card->condition)            //当前卡为禁用状态
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 充值:failed", time);
    }
    else if (card->getBalance() + amount > BALANCECEILING)
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 充值:failed 备注:卡内余额达到上限", time);
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 充值前余额:" + to_string(card->getBalance()) + " 充值后余额:" + to_string(card->getBalance() + amount), time);
        card->recharge(amount);
    }
}

//查询和学号匹配的卡
bool CardManage::query(unsigned int uid)
{
}

//日志回溯
void CardManage::recall()
{
}

void CardManage::log(const char *title, const string &content, const string &time = "")
{
    if (!time.size())
    {
        FileManager::getInstance() << FileManager::toStandardLogString(title, content.c_str()) << FileManager::endl;
    }
    else
    {
        FileManager::getInstance() << FileManager::toStandardLogString(title, content.c_str(), time.c_str()) << FileManager::endl;
    }
}

//根据文件kh001.txt开户
void CardManage::openAccountByFile()
{
    vector<string> container;
    FileManager::getInstance().getStringDataSourceByLine(container);
    for (auto &info : container)
    {
        int uid = atoi(info.substr(0, UID_LENGTH).c_str());
        string name = info.substr(UID_LENGTH + 1, info.size() - UID_LENGTH - 2);
        openAccount(uid, name.c_str());
    }
}
//根据文件cz002.txt操作
void CardManage::operateByFile()
{
    vector<string> container;
    FileManager::getInstance().getStringDataSourceByLine(container, FileManager::CARD_MANAGE_NAME);
    for (auto &info : container)
    {
        int uid = atoi(info.substr(info.find_last_of(',') + 1, UID_LENGTH + 1).c_str());
        string time = info.substr(0, info.find(','));
        if (info.find("挂失") != string::npos)
        {
            setLost(uid, time);
        }
        else if (info.find("充值") != string::npos)
        {
            int amount = uid;
            uid = atoi(info.substr(info.find("充值") + 5, 10).substr().c_str());
            recharge(uid, amount, time);
        }
        else if (info.find("销户") != string::npos)
        {
            deleteAccount(uid, time);
        }
        else if (info.find("解挂") != string::npos)
        {
            unsetLost(uid, time);
        }
    }
}