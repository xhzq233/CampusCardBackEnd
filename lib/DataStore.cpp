//
// Created by 夏侯臻 on 2022/2/20.
//

#include "DataStore.h"

using Accounts = DataStore::Accounts;
using Consumes = DataStore::Consumes;
using WindowPositions = DataStore::WindowPositions;

Accounts DataStore::accounts_init() {

    return Accounts();
}

Consumes DataStore::consumes_init() {

    return Consumes(MAXSIZE);
}

WindowPositions DataStore::windows_init() {
    std::vector<WindowPosition> res(WINDOW_QTY);

    FileManager::CSV container(WINDOW_QTY, {"", ""});
    ///预定好尺寸 99x2
    FileManager::getInstance().getCSVDataSource(container, Pair((unsigned int) WINDOW_QTY, (unsigned int) 2),
                                                FileManager::CAFE_POSITION_CSV_NAME);
    ///下标为窗口号，值为data数组下标
    for (auto &&row: container) {
        res[stoi(row[0])] = stoi(row[1]);
    }

    return res;
}

DataStore::index DataStore::halfFind() {
    return 0;
}
