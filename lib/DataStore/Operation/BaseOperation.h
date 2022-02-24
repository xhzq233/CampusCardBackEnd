//
// Created by end on 2022/2/24.
//

#ifndef CAMPUSCARDBACKEND_BASEOPERATION_H
#define CAMPUSCARDBACKEND_BASEOPERATION_H

#include <string>

class BaseOperation {
public:
    typedef unsigned long long Time;
    Time time;

    explicit BaseOperation(Time time) : time(time) {}

    [[nodiscard]] virtual std::string to_string() const {
        return std::to_string(time);
    }

    // delete copy methods
    BaseOperation(const BaseOperation &) = delete;

    // disable copied BaseOperation
    BaseOperation &operator=(const BaseOperation &) = delete;
};


#endif //CAMPUSCARDBACKEND_BASEOPERATION_H
