//
// Created by 夏侯臻 on 2022/2/20.
//

#include "DataStore.h"
#include <thread>

using Accounts = DataStore::Accounts;
using Consumptions = DataStore::Consumptions;
using WindowPositions = DataStore::WindowPositions;
using CSV = FileManager::CSV;

Accounts &DataStore::accounts_init() {
    static Accounts res;
    CSV temp;
    FileManager::getInstance().getCSVDataSource(temp, 2, FileManager::OPEN_ACCOUNT_CSV_NAME);
    for (auto &&info: temp) {
        res.emplace_back(Account(info));
    }
    std::sort(res.begin(), res.end());
    return res;
}

//  sentence under here used on debug
//  std::thread::id main_thread_id = std::this_thread::get_id();
void subwork_of_init_consumes(int index, Consumptions *consumes) {
//    if (main_thread_id == std::this_thread::get_id())
//        std::cout << "This is the main thread.\n";
//    else
//        std::cout << "This is not the main thread.\n";

    CSV temp;
    FileManager().getCSVDataSource(temp, 4, FileManager::CONSUME_CSV(index + 1));

    //  stored data start index, it should be the initialized window position minus data size
    auto start_index = DataStore::getWindowPositions()[index] - temp.size();

    for (unsigned int i = start_index; i < start_index + temp.size(); ++i) {
        auto *consume = new Consumption(index + 1, temp[i - start_index]);
        (*consumes)[index][i % DataStore::MAXSIZE] = consume; // no more than MAXSIZE
    }
    // no longer to be sorted
}

#ifdef __WIN64
const unsigned int MAX_THREAD = std::thread::hardware_concurrency();
#endif //__WIN64

Consumptions &DataStore::consumes_init() {
    //99 x 60000
    static Consumption *res[WINDOW_QTY][MAXSIZE] = {nullptr};
    std::thread threads[FileManager::CONSUME_CSV_QTY];
    printf("Spawning threads...\n");
    for (int i = 0; i < FileManager::CONSUME_CSV_QTY; ++i)
        threads[i] = std::thread(subwork_of_init_consumes, i, &res);   // move-assign threads
    printf("Done spawning threads. Now waiting for them to join:\n");

    for (auto &thread: threads)
        thread.join();
    printf("All threads joined!\n");

    return res;
}

WindowPositions &DataStore::windows_init() {
    static WindowPosition windowPositions[WINDOW_QTY] = {0};
    FileManager::CSV container;
    ///预定好尺寸 99x2
    FileManager::getInstance().getCSVDataSource(container, WINDOW_QTY, 2,
                                                FileManager::CAFE_POSITION_CSV_NAME);
    ///下标为窗口号，值为data数组下标

    //why auto&&?
    //Reference:
    //https://stackoverflow.com/questions/13241108/why-does-a-range-based-for-statement-take-the-range-by-auto
    for (const auto &row: container)
        windowPositions[stoi(row[0]) - 1] = stoi(row[1]); // window ranged 1-99, but subscripts ranged 0-98

    return windowPositions;
}

void DataStore::localize() {

}

WindowPositions &DataStore::getWindowPositions() {
    static WindowPositions &windowPositions = windows_init();
    return windowPositions;
}

Consumptions &DataStore::getConsumptions() {
    static Consumptions &consumes = consumes_init();
    return consumes;
}

Accounts &DataStore::getAccounts() {
    static Accounts &accounts = accounts_init();
    return accounts;
}

void DataStore::insertAccount(const Account &data) {
    auto &accounts = getAccounts();
    int left = 0, right = (int) accounts.size() - 1, mid;
    //half search
    while (left <= right) {
        mid = (left + right) / 2;
        if (accounts[mid] < data) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    accounts.emplace(accounts.begin() + mid, data);
}

std::vector<Account>::iterator DataStore::queryAccountByUid(unsigned int uid) {
    auto &accounts = getAccounts();
    int left = 0, right = (int) accounts.size() - 1, mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (accounts[mid].uid > uid) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    if ((accounts.begin() + mid)->uid == uid) {
        return accounts.begin() + mid;
    } else {
        return accounts.end();
    }
}

std::vector<Account>::iterator DataStore::queryAccountByCid(unsigned int cid) {
    auto &accounts = DataStore::getAccounts();
    int left = 0, right = (int) accounts.size() - 1, mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (accounts[mid].cards.begin()->cid > cid) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    if ((accounts.begin() + mid)->cards.begin()->cid == cid) {
        return accounts.begin() + mid;
    } else {
        return accounts.end();
    }
}

void DataStore::insertConsumption(Window window, Consumption *data) {
    //error handle
    if (window == 0 || window > WINDOW_QTY) {
        printf("[ERROR] %u not in this range: 1 - 99", window);
        return;
    }

    auto &consumes_in_window = getConsumptions()[window - 1];
    auto &&position = getWindowPositions()[window - 1];
    // add self and assign self
    position = (position + 1) % MAXSIZE; // no more than MAXSIZE

    consumes_in_window[position] = data;
}


using DataQuery = DataStore;

std::regex DataQuery::customRegex2CommonRegexSyntax(std::string &regex) {
    regex.replace(regex.find('?'), 1, ".");
    regex.replace(regex.find('*'), 1, ".{2,}");
    return std::regex(regex);
}

DataQuery::Subscripts
DataQuery::query(FileManager::Strings &container, const std::regex &regex) {
    Subscripts res;
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (std::regex_match(container[i], regex))
            res.emplace_back(i);
    }
    return res;
}

DataQuery::Subscripts
DataQuery::query(FileManager::CSV &container, unsigned int columnIndex, const std::regex &regex) {

    Subscripts res;
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (std::regex_match(container[i][columnIndex], regex))
            res.emplace_back(i);
    }
    return res;
}

DataQuery::Subscripts DataStore::queryConsumption(Window window, unsigned int cid) {
    return DataStore::Subscripts();
}

DataQuery::Subscripts DataStore::queryConsumption(unsigned int cid) {
    return DataStore::Subscripts();
}

DataQuery::Subscripts DataStore::queryConsumptionByUid(unsigned int uid) {
    return DataStore::Subscripts();
}
