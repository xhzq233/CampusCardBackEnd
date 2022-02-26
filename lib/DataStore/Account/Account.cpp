//
// Created by end on 2022/2/26.
//

#include "Account.h"

list::~list() {
    delete card;
}

void list::push(Card *newCard) {
    if (!next) {
        next = new list;
        next->card = newCard;
    } else {
        list *tmp = new list;
        tmp->card = newCard;
        tmp->next = next;
        next = tmp;
    }
}

Card *list::begin() {
    return next ? next->card : card;
}

void list::clear() {
    list *p, *q = this->next;
    while (q) {
        p = q;
        q = q->next;
        delete p;
        this->next = q;
    }
}

int list::size() {
    if (!next) {
        return 0;
    }
    list *p = next;
    int size = 1;
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
