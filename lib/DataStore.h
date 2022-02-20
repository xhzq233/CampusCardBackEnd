//
// Created by 夏侯臻 on 2022/2/20.
//

#ifndef CAMPUSCARDBACKEND_DATASTORE_H
#define CAMPUSCARDBACKEND_DATASTORE_H

#include <vector>
#include "Card.h"
#include "Cafe.h"

class DataStore {
private:
    typedef std::vector<Card> Cards;
    typedef std::vector<Cafe> Cafes;

    static Cards cards_init();

    static Cafes cafes_init();

    static Cards cards;
    static Cafes cafes;

public:
    static void init() {
        cards = cards_init();
        cafes = cafes_init();
    }
};

#endif //CAMPUSCARDBACKEND_DATASTORE_H
