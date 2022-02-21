//
// Created by 夏侯臻 on 2022/2/20.
//

#ifndef CAMPUSCARDBACKEND_DATASTORE_H
#define CAMPUSCARDBACKEND_DATASTORE_H

#include <vector>
#include "Card.h"
#include "Cafe.h"
#include "FileManager.h"
#include "Account.h"

class DataStore {
private:
    typedef std::vector<Card> Cards;
    typedef std::vector<Cafe> Cafes;
    typedef std::vector<Account> Accounts;

    static Cards cards_init();

    static Cafes cafes_init();

    static Accounts accounts_init();

public:

    static Cards cards;
    static Accounts accounts;
    static Cafes cafes;

    static void init() {
        cards = cards_init();
        cafes = cafes_init();
        accounts = accounts_init();
    }

    static void store() {

    }
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
