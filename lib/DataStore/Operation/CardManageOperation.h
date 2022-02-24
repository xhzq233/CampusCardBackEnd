//
// Created by 夏侯臻 on 2022/2/24.
//

#ifndef CAMPUSCARDBACKEND_CARDMANAGEOPERATION_H
#define CAMPUSCARDBACKEND_CARDMANAGEOPERATION_H

#include "BaseOperation.h"

class CardManageOperation : public BaseOperation {
public:
    enum OperationName : char {
        Recharge = 0x01,
        SetLoss = 0x02,
        Uncouple = 0x04,
        Cancellation = 0x08,
    };
    OperationName operationName;
    typedef unsigned int Operator;
    Operator uid;

    CardManageOperation(Operator uid, OperationName name, Time time) : BaseOperation(time), uid(uid),
                                                                       operationName(name) {}

    // from strings
    explicit CardManageOperation(const std::vector<std::string> &strings) : CardManageOperation(

            std::stoi(
                    strings[2]),
            strings[1] == "充值" ? Recharge : strings[1] == "挂失" ? SetLoss : strings[1] == "解挂" ? Uncouple : Cancellation,
            std::stoull(strings[0])) {}


    [[nodiscard]] std::string to_string() const override {
        auto &&res = BaseOperation::to_string();
        res.push_back(',');
        switch (operationName) {
            case Recharge:
                res.append("充值");
                break;
            case SetLoss:
                res.append("挂失");
                break;
            case Uncouple:
                res.append("解挂");
                break;
            case Cancellation:
                res.append("销户");
                break;
        }
        res.push_back(',');
        res.append(std::to_string(uid));

        return res;
    }
};

#endif //CAMPUSCARDBACKEND_CARDMANAGEOPERATION_H
