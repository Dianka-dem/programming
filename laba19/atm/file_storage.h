#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H
#include "card.h"
#include "account.h"
#include <memory>
#include <unordered_map>

class FileStorage {
private:
    std::string cardsFile;
    std::string accountsFile;
    std::unordered_map<std::string, std::unique_ptr<Card>> cards;
    std::unordered_map<std::string, std::unique_ptr<Account>> accounts;
public:
    FileStorage(const std::string& cf, const std::string& af);
    void load();
    void save();
    Card* getCard(const std::string& number);
    Account* getAccount(const std::string& cardNumber);
    void createDefaultData();
};

#endif
