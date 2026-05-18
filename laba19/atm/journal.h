#ifndef JOURNAL_H
#define JOURNAL_H
#include "transaction.h"
#include <string>
#include <vector>

class Journal {
private:
    std::string filename;
public:
    Journal(const std::string& fname);
    void appendTransaction(const Transaction& t);
    std::vector<Transaction> readAllTransactions();
};

#endif
