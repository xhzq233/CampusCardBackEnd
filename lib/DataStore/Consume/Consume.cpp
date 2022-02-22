#include "Consume.h"

std::string Consume::to_string() const {
    std::string res;
    res.append(std::to_string(cid));    // cid
    res.append(",");
    res.append(std::to_string(window)); // window
    res.append(",");
    res.append(date);                       // date
    res.append(",");
    res.append(std::to_string(price));  // price
    return res;
}


