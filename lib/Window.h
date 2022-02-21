//
// Created by 夏侯臻 on 2022/1/14.
//

#ifndef CAMPUSCARDBACKEND_WINDOW_H
#define CAMPUSCARDBACKEND_WINDOW_H
#include <string>

class Window {
    unsigned int uid;   //唯一辨识id 表现为窗口号
    unsigned int position;  //位置
    Window(unsigned int uid, unsigned int position);
};


#endif //CAMPUSCARDBACKEND_WINDOW_H
