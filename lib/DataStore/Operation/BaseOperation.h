//
// Created by end on 2022/2/24.
//

#ifndef CAMPUSCARDBACKEND_BASEOPERATION_H
#define CAMPUSCARDBACKEND_BASEOPERATION_H

#include "../../FileManager/FileManager.h"

class BaseOperation {
public:
    using Time = FileManager::Time;
    Time time;

    explicit BaseOperation(Time time) : time(time) {}

    [[nodiscard]] virtual std::string to_string() const {
        return std::to_string(time);
    }

    /// comparable
    bool operator>(const BaseOperation &right) const {
        return time > right.time;
    }

    bool operator<(const BaseOperation &right) const {
        return time < right.time;
    }

    /// used on check data integrity
    [[nodiscard]] virtual unsigned long long hash_value() const {
        return time;
    }
};


#endif //CAMPUSCARDBACKEND_BASEOPERATION_H
