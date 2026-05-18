#ifndef ATM_ENGINE_H
#define ATM_ENGINE_H
#include "card.h"
#include "account.h"
#include "journal.h"
#include "recovery.h"

class ATMEngine {
private:
    Card* currentCard;
    Account* currentAccount;
    Journal journal;
    Recovery recovery;
    bool authenticated;
public:
    ATMEngine(const std::string& journalFile, const std::string& recoveryFile);
    bool insertCard(Card* card);
    bool authenticate(const std::string& pin);
    void checkBalance();
    bool withdraw(double amount);
    void deposit(double amount);
    void showHistory();
    void logout();
    void recoverFromCrash(Account* account);
    bool isAuthenticated() const { return authenticated; }
};

#endif
