#include "journal.h"
#include <fstream>
#include <iostream>

Journal::Journal(const std::string& fname) : filename(fname) {}

void Journal::appendTransaction(const Transaction& t) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (file) {
        file.write(reinterpret_cast<const char*>(&t.type), sizeof(t.type));
        file.write(reinterpret_cast<const char*>(&t.amount), sizeof(t.amount));
        file.write(reinterpret_cast<const char*>(&t.timestamp), sizeof(t.timestamp));
    }
}

std::vector<Transaction> Journal::readAllTransactions() {
    std::vector<Transaction> transactions;
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        Transaction t;
        while (file.read(reinterpret_cast<char*>(&t.type), sizeof(t.type))) {
            if (file.read(reinterpret_cast<char*>(&t.amount), sizeof(t.amount)) &&
                file.read(reinterpret_cast<char*>(&t.timestamp), sizeof(t.timestamp))) {
                transactions.push_back(t);
            }
        }
    }
    return transactions;
}
