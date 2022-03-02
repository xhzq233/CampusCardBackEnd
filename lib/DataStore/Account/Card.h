//
// Created by 夏侯臻 on 2022/1/14.
//
#ifndef CARD_H
#define CARD_H

#include "../../Dependencies.h"


class Card {
    // card expired after this date
    constexpr static const unsigned int DEFAULT_EXPIRE_DATE = 20240715U;
    // default passwd before changed
    constexpr static const unsigned int DEFAULT_PASSWORD = 123456U; //默认的密码

private:
    unsigned int password; //密码

public:
    /* expire time */
    unsigned int date;

    /* card id */
    unsigned int cid;

    /* student id */
    unsigned int uid;

    /* true represents valid, false represents invalid */
    bool condition;

    Card(unsigned int uid, unsigned int serialNumber) : uid(uid), date(DEFAULT_EXPIRE_DATE), condition(true),
                                                        password(DEFAULT_PASSWORD) {
        unsigned int checkNode = 0, tmp = serialNumber;
        //校验码,卡号校验码的计算规则：前6位数字相加的和再模10，得到一个0-9的数，然后用9减去这个数，就是最后一位校验码
        for (int i = 0; i < 5; ++i) {
            checkNode += tmp % 10;
            tmp /= 10;
        }
        checkNode = 9 - (checkNode + 3) % 10;
        this->cid = (300000 + serialNumber) * 10 + checkNode;
    }

    //验证密码
    [[nodiscard]] bool checkPassword(unsigned int passwd) const {
        return this->password == passwd;
    }

    // to string
    [[nodiscard]] std::string to_string() const {
        return std::to_string(cid);
    }
};

class CardList {
private:
    Card card;
    CardList *next;
public:

    explicit CardList(const Card &newCard) : card(newCard), next(nullptr) {};

    ~CardList() = default;

    void push(const Card &newCard) {
        head()->next = new CardList(newCard);
    }

    [[nodiscard]] CardList *head() {
        auto pointer = this;
        while (pointer->next)
            pointer = pointer->next;
        return pointer;
    }

    [[nodiscard]] Card &begin() {
        return head()->card;
    }

    void clear() {
        CardList *cur_list = next;
        while (cur_list) {
            auto temp = cur_list;
            cur_list = cur_list->next;
            delete temp;
        }
        next = nullptr;
    }

    [[nodiscard]] unsigned int size() const {
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

    // to string
    [[nodiscard]] std::string to_string() const {
        std::string res(card.to_string());
        res.push_back('\n');
        while (next) {
            res.append(next->card.to_string());
        }
        return res;
    }

    std::vector<unsigned int> getAllCid() {
        std::vector<unsigned int> res;
        CardList *curr = this;
        res.emplace_back(curr->card.cid);
        while (curr->next) {
            res.emplace_back(curr->card.cid);
            curr = curr->next;
        }
        return res;
    }
};

#endif // CARD_H