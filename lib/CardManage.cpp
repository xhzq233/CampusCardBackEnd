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
    if (instance == nullptr)
    {
        instance = new CardManage();
    }
    return instance;
}

//系统具备正常状态的学号、姓名等信息的，即属于开户状态
void CardManage::openAccount(unsigned int uid, const char *name)
{

    info.insert(make_pair(uid, std::move(name)));
    log("Manage", "学号:" + to_string(uid) + " 姓名" + name + " 开户:succeeded");
}

//删除学号等数据项，或进行标识，只有经过恢复开户后才能恢复到开户状态；
void CardManage::deleteAccount(unsigned int uid)
{
    if (info.count(uid) != 0)
    {
        info.erase(uid);
        //将销户的所有卡号状态设置为禁用
        setLost(uid);
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + " 销户:succeeded");
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + " 销户:failed 备注:该用户未开号");
    }
}

//依据开户信息，初次分配唯一卡号；如果已经分配过卡号的，则属于补卡功能；
void CardManage::distribute(unsigned int uid)
{
    if (info.count(uid) != 0 && v.count(uid) == 0)
    {
        Card *card = new Card(uid, info[uid], ++serialNumber);
        list<Card *> *l = new list<Card *>({card});
        v.insert(make_pair(uid, l));
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 发卡:succeed");
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + " 发卡:failed");
    }
}

//设置当前学号最新分配的卡号的卡片为禁用的状态；
void CardManage::setLost(unsigned int uid)
{
    if (info.count(uid) == 0)
    {
        log("Manage", "学号:" + to_string(uid) + " 解挂:failed 备注:非系统用户");
        return;
    }
    Card *card = (*v[uid]->begin());
    if (card->condition == true)
    {
        card->condition = false;
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 挂失:succeed");
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 挂失:failed");
    }
}

//设置当前学号最新分配的卡号的卡片为正常的状态；
void CardManage::unsetLost(unsigned int uid)
{
    if (info.count(uid) == 0)
    {
        log("Manage", "学号:" + to_string(uid) + " 解挂:failed 备注:非系统用户");
        return;
    }
    Card *card = (*v[uid]->begin());
    if (card->condition == false)
    {
        card->condition = true;
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 解挂:succeed");
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 解挂:failed");
    }
}

//为当前学号分配新的卡号，即发放新的校园卡；该学号关联的其他卡片同时全部处于挂失禁用状态；
void CardManage::reissue(unsigned int uid)
{
    if (info.count(uid) == 0)
    {
        log("Manage", "学号:" + to_string(uid) + " 补卡:failed 备注:非系统用户");
        return;
    }
    //最多只能补卡100次
    else if (v[uid]->size() >= 100)
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + " 补卡:failed 备注:补卡次数达到上限");
        return;
    }
    Card *card = new Card(uid, info[uid], ++serialNumber);
    //如果之前有卡,将之前卡的余额给新补的卡
    if (!v[uid]->empty())
    {
        card->balance = (*v[uid]->begin())->balance;
    }
    //将之前卡的状态设置为禁用状态
    setLost(uid);
    v[uid]->push_front(card);
    log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 补卡:succeeded");
}

//为该学号账户充值；账户余额上限999.99元；
void CardManage::recharge(unsigned int uid, unsigned int amount)
{
    Card *card = (*v[uid]->begin()); //最新的卡
    if (!card->condition)            //当前卡为禁用状态
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 充值:failed");
    }
    else if (card->balance + amount > BALANCECEILING)
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 充值:failed 备注:卡内余额达到上限");
    }
    else
    {
        log("Manage", "学号:" + to_string(uid) + " 姓名" + info[uid] + "卡号: " + to_string(card->cid) + " 充值前余额:" + to_string(card->balance) + " 充值后余额:" + to_string(card->balance + amount));
        card->balance += amount;
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
void CardManage::log(const char *title, const string &content)
{
    FileManager::getInstance() << FileManager::toStandardLogString(title, content);
}