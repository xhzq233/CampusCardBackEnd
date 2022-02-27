//
// Created by end on 2022/2/26.
//

#include "Account.h"

void list::push(Card &newCard) {
    if (!next) {
        next = new list(0, 0);
        next->card = newCard;
        std::swap(card, next->card);
    } else {
        list *tmp = new list(0, 0);
        tmp->card = newCard;
        tmp->next = next;
        next = tmp;
        std::swap(card, next->card);
    }
}

Card &list::begin() {
    return card;
}

void list::clear() {
    list *p, *q = next;
    while (q) {
        p = q;
        q = q->next;
        delete p;
        next = q;
    }
    next = nullptr;
}

int list::size() {
    if (!next) {
        return 1;
    }
    list *p = next;
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
