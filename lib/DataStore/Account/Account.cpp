//
// Created by end on 2022/2/26.
//

#include "Account.h"

void CardList::push(Card &newCard) {
    if (!next) {
        next = new CardList(0, 0);
        next->card = newCard;
        std::swap(card, next->card);
    } else {
        auto *tmp = new CardList(0, 0);
        tmp->card = newCard;
        tmp->next = next;
        next = tmp;
        std::swap(card, next->card);
    }
}

Card &CardList::begin() {
    return card;
}

void CardList::clear() {
    CardList *p, *q = next;
    while (q) {
        p = q;
        q = q->next;
        delete p;
        next = q;
    }
    next = nullptr;
}

int CardList::size() {
    if (!next) {
        return 1;
    }
    CardList *p = next;
    int size = 2;
    while (p->next) {
        ++size;
        p = p->next;
    }
    return size;
}

std::string Account::to_string() const {
    std::string res;
    res.append(std::to_string(uid));
    res.append(" ");
    res.append(name);
    return res;
}
