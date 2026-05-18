#ifndef CARD_H
#define CARD_H
#include <string>

class Card {
private:
    std::string number;
    std::string pin;
    bool blocked;
    int failedAttempts;
public:
    Card(const std::string& num, const std::string& p);
    std::string getNumber() const;
    bool verifyPin(const std::string& input);
    bool isBlocked() const;
    void resetFailedAttempts();
    void block();
};

#endif
