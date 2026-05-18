#ifndef RECOVERY_H
#define RECOVERY_H
#include "account.h"
#include "journal.h"
#include <string>

class Recovery {
private:
    std::string accountFile;
    int vibeBankProtection;
public:
    Recovery(const std::string& accFile);
    void recover(Account& account, Journal& journal);
    void saveAccountState(const Account& account);
};

#endif
