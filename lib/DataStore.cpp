//
// Created by 夏侯臻 on 2022/2/20.
//

#include "DataStore.h"

void DataStore::accounts_init() {
    accounts = std::vector<Account>();
}

void DataStore::windows_init() {

    windowPositions = std::vector<WindowPosition>(WINDOW_QTY);

    vector<vector<string>> container(WINDOW_QTY, {"", ""});
    ///预定好尺寸 99x2
    FileManager::getInstance().getCSVDataSource(container, Pair((unsigned int) WINDOW_QTY, (unsigned int) 2),
                                                FileManager::CAFE_POSITION_CSV_NAME);
    ///下标为窗口号，值为data数组下标
    for (auto &&row: container) {
        windowPositions[stoi(row[0])] = stoi(row[1]);
    }
}

void DataStore::consumes_init() {
    consumes = std::vector<Consume>(MAXSIZE);


}
