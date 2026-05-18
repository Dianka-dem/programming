#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <string>
#include <ctime>

struct Transaction {
    enum Type { DEPOSIT, WITHDRAW };
    Type type;
    double amount;
    time_t timestamp;
    std::string toString() const;
};

#endif
