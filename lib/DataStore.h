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

class DataStore {
private:
    typedef std::vector<Window> Windows;
    typedef std::vector<Account> Accounts;
    typedef std::vector<Consume> Consumes;

    static void consumes_init();

    static void windows_init();

    static void accounts_init();

public:

    static Accounts accounts;
    static Windows windows;
    static Consumes consumes;

    static void init() {
        windows_init();
        accounts_init();
        consumes_init();
    }

    static void store() {

    }
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
