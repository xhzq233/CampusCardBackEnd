//
// Created by 夏侯臻 on 2022/2/5.
//

#ifndef CAMPUSCARDBACKEND_UTILS_H
#define CAMPUSCARDBACKEND_UTILS_H

template<typename A, typename B>
class Pair {
public:
    const A first;
    const B second;

    Pair(A first, B second) : first(first), second(second) {};
};

#endif //CAMPUSCARDBACKEND_UTILS_H
