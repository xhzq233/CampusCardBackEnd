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
//    const auto &position = DataStore::getWindowPositions()[index];
    auto size = temp.size();
//    int start_index = (int) position - size + 1;
//    if (start_index < 0) // position - temp.size() + 1 less than zero
//        start_index = (int) DataStore::MAXSIZE - start_index;
    for (unsigned int i = 0; i < size; ++i) {
        (*consumes)[index][i % DataStore::MAXSIZE] = new Consumption(index + 1,
                                                                     temp[i]);
        // no more than MAXSIZE
    }
    // no longer to be sorted
}

#ifdef __WIN64
const unsigned int MAX_THREAD = std::thread::hardware_concurrency();
#endif //__WIN64

Consumptions &DataStore::consumes_init() {
    //99 x 60000
    static Consumptions res{nullptr};
    std::thread threads[FileManager::CONSUME_CSV_QTY];
    for (int i = 0; i < FileManager::CONSUME_CSV_QTY; ++i)
        threads[i] = std::thread(subwork_of_init_consumes, i, &res);   // move-assign threads
    for (auto &thread: threads)
        thread.join();
    printf("All threads joined!\n");

    auto &sc = DataStore::sortedConsumptions();
    unsigned int num = 0;
    for (auto & re : res) {
        for (const auto &item: re) {
            if (!item) { // quit loop if meet null, in this case data ranged from zero
                break;   // but if ranged from window position , em-mm
            }
            sc[num] = item;
            ++num;
        }
    }
    std::sort(sc, sc + DATA_NUM, [](Consumption *l, Consumption *r) -> bool {
        return (*l) < (*r);
    });
    return res;
}

WindowPositions &DataStore::windows_init() {
    static WindowPositions windowPositions{0};
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
    //half search
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
        // result has been found
        return accounts.begin() + mid;
    } else {
        // result is not found
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
        // result has been found
        return accounts.begin() + mid;
    } else {
        // result is not found
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
    //error handle
    if (window == 0 || window > WINDOW_QTY) {
        printf("[ERROR] %u not in this range: 1 - 99", window);
        return {};
    }
    Subscripts res;
    const auto &consumptions_in_window = getConsumptions()[window - 1];
    WindowPosition windowPosition = getWindowPositions()[window - 1];
    //individualization
    if (cid == consumptions_in_window[windowPosition]->cid) {
        res.emplace_back(windowPosition);
    }
    // if windowPosition equal to zero,
    if (!windowPosition) windowPosition = MAXSIZE;
    // Retrieve backward from this windowPosition
    for (unsigned int i = windowPosition - 1; i != windowPosition; --i) {
        if (!consumptions_in_window[i]) break;//meet nullptr ,loop close
        if (cid == consumptions_in_window[i]->cid)
            res.emplace_back(i);
        if (i == 0) i = MAXSIZE;
    }
    return res;
}

DataQuery::Subscripts DataStore::queryConsumption(unsigned int cid) {
    return {};
}

DataQuery::Subscripts DataStore::queryConsumptionByUid(unsigned int uid) {
    return {};
}
