//
// Created by 夏侯臻 on 2022/1/14.
//

#ifndef CAMPUSCARDBACKEND_CAFE_H
#define CAMPUSCARDBACKEND_CAFE_H
#include <string>

class Cafe {
    unsigned int uid;   //唯一辨识id 表现为窗口号
    unsigned int position;  //位置
    Cafe(unsigned int uid, unsigned int position);
};


#endif //CAMPUSCARDBACKEND_CAFE_H
