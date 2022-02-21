//
// Created by 夏侯臻 on 2022/2/20.
//

#ifndef CAMPUSCARDBACKEND_DATASTORE_H
#define CAMPUSCARDBACKEND_DATASTORE_H

#include <vector>
#include "Card.h"
#include "Window.h"
#include "FileManager.h"
#include "Account.h"
#include "Consume.h"
#include <queue>

class DataStore {
public:
    typedef std::vector<WindowPosition> WindowPositions;
    typedef std::priority_queue<Account, std::less<Account>> Accounts;
    typedef std::vector<Consume> Consumes;


    static Accounts &getAccounts() {
        static Accounts accounts = accounts_init();
        return accounts;
    }

    static Consumes &getConsumes() {
        static Consumes consumes = consumes_init();
        return consumes;
    }

    static const WindowPositions &getWindowPositions() {
        static const WindowPositions windowPositions = windows_init();
        return windowPositions;
    }

    static void localize() {

    }

private:

    static Consumes consumes_init();

    static WindowPositions windows_init();

    static Accounts accounts_init();

    constexpr static const char WINDOW_QTY = 99;
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
