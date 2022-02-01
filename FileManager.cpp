//
// Created by 夏侯臻 on 2022/1/14.
//

#include "FileManager.h"

FileManager &FileManager::getInstance() {
    static FileManager instance;
    return instance;
}
