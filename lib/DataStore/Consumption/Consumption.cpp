#include "Consumption.h"

using namespace std;
string Consumption::to_string() const {
    std::string res;
    res.append(::to_string(cid));    // cid
    res.append(",");
    res.append(::to_string(window)); // window
    res.append(",");
    res.append(::to_string(date));   // date
    res.append(",");
    res.append(::to_string(price));  // price
    return res;
}


