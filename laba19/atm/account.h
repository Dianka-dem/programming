#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "transaction.h"
#include <vector>
#include <string>

class Account {
private:
    double balance;
    double dailyWithdrawn;
    std::string lastDailyReset;
    std::vector<Transaction> recentTransactions;
public:
    Account(double initialBalance = 0);
    double getBalance() const;
    bool withdraw(double amount, bool withFee = true);
    void deposit(double amount);
    double calculateFee(double amount) const;
    bool checkDailyLimit(double amount) const;
    void addTransaction(const Transaction& t);
    std::vector<Transaction> getLastNTransactions(int n) const;
    void resetDailyIfNeeded();
    void setBalance(double b);
};

#endif
