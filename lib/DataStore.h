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
    typedef const std::vector<WindowPosition> WindowPositions;
    typedef std::priority_queue<Account, std::less<Account>> Accounts;
    typedef std::vector<Consume> Consumes;


    template<typename type>
    static type &getEntity() {
        static_assert(std::is_base_of<list, T>::value, "T must inherit from list");
        static type entity;
        return entity;
    }

    static void init() {
        windows_init();
        accounts_init();
        consumes_init();
    }

    static void localize() {

    }

private:

    static void consumes_init();

    static WindowPositions windows_init();

    static void accounts_init();

    constexpr static const char WINDOW_QTY = 99;
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
