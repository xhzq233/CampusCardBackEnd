//
// Created by 夏侯臻 on 2022/1/14.
//

#ifndef CAMPUSCARDBACKEND_CONSUMPTION_H
#define CAMPUSCARDBACKEND_CONSUMPTION_H

#include "../Window.h"
#include "../../FileManager/FileManager.h"
#include "BaseOperation.h"

/*
 * 运行食堂应用模块，可以设定当前时间和消费窗口，对指定的校园卡进行收费
 * 每次消费支付后，收费设备显示当天该窗口累计交易次数和累计收费金额
 * 窗口参数设置：
 *    设定实验程序的当前日期和时间，设定当前消费窗口号
 *    为后续的食堂窗口购餐消费确定，一个确定的场景操作日志记录该项操作和设置的数据情况
 * 窗口消费：
 *    在确定的程序时间和食堂窗口场景下，输入消费的卡号和消费金额，系统检查该校园卡是否正常的卡，检查余额是否足够，然后进行实际的余额扣减
 *    如果本次消费成功，则生成并保存具有完整信息项的消费记录，并显示当前该窗口在当天的累计消费次数和消费金额
 *    操作日志记录该项消费操作、卡号、余额的变化和消费是否成功等信息
 *
 * # Notice
 *  + 食堂窗口的收费设备仅仅能够读取到校园卡卡号信息
 *  + 如果该校园卡属于余额不足、密码错误、挂失或禁用状态，则不能进行消费，消费申请无效
 *  + 食堂窗口收费设备单独记录本窗口的数据
 *  + 数据最多保存6万条，循环覆盖保存，程序开始运行的前一次保存位置可以通过配置文件进行读入与保存
 * */
class Consumption : BaseOperation {
public:
    //卡号
    unsigned int cid;
    // 消费窗口
    Window window;
    // 消费金钱
    float price;

    Consumption(unsigned int cid, Window window, Time time, float price) : BaseOperation(time),
                                                                           window(window),
                                                                           price(price), cid(cid) {
        FileManager::getInstance() << FileManager::toStandardLogString("消费", this->to_string().c_str())
                                   << FileManager::endl;
    }

    /// comparable
    bool operator>(const Consumption &right) const {
        return time > right.time;
    }

    bool operator<(const Consumption &right) const {
        return time < right.time;
    }

    // from strings
    explicit Consumption(Window window, const std::vector<std::string> &strings) : Consumption(std::stoi(strings[0]),
                                                                                               window,
                                                                                               std::stoull(strings[1] +
                                                                                                           strings[2]),
                                                                                               std::stof(strings[3])) {}

    // to string
    // cid + window + date + price
    [[nodiscard]] std::string to_string() const override {
        auto &&res = BaseOperation::to_string();
        res.append(std::to_string(cid));    // cid
        res.push_back(',');
        res.append(std::to_string(window)); // window
        res.push_back(',');
        res.append(std::to_string(price));  // price
        return res;
    }
};

#endif // CAMPUSCARDBACKEND_CONSUMPTION_H
