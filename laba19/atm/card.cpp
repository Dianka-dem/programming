#include "card.h"
Card::Card(const std::string& num, const std::string& p) 
    : number(num), pin(p), blocked(false), failedAttempts(0) {}
std::string Card::getNumber() const { return number; }
bool Card::verifyPin(const std::string& input) {
    if (blocked) return false;
    if (pin == input) {
        failedAttempts = 0;
        return true;
    }
    failedAttempts++;
    if (failedAttempts >= 3) blocked = true;
    return false;
}
bool Card::isBlocked() const { return blocked; }
void Card::resetFailedAttempts() { failedAttempts = 0; }
void Card::block() { blocked = true; }
