#include "atm_engine.h"
#include <iostream>
#include <iomanip>

ATMEngine::ATMEngine(const std::string& journalFile, const std::string& recoveryFile)
    : journal(journalFile), recovery(recoveryFile), authenticated(false), currentCard(nullptr), currentAccount(nullptr) {}

bool ATMEngine::insertCard(Card* card) {
    if (!card || card->isBlocked()) return false;
    currentCard = card;
    return true;
}

bool ATMEngine::authenticate(const std::string& pin) {
    if (!currentCard) return false;
    if (currentCard->verifyPin(pin)) {
        authenticated = true;
        return true;
    }
    return false;
}

void ATMEngine::checkBalance() {
    if (!authenticated || !currentAccount) return;
    std::cout << "\n💰 Ваш баланс: " << std::fixed << std::setprecision(2) 
              << currentAccount->getBalance() << " руб\n";
}

bool ATMEngine::withdraw(double amount) {
    if (!authenticated || !currentAccount) return false;
    
    // Вся логика теперь в account.cpp, здесь только вызываем
    bool result = currentAccount->withdraw(amount, true);
    
    if (result) {
        journal.appendTransaction(Transaction{Transaction::WITHDRAW, amount, std::time(nullptr)});
        recovery.saveAccountState(*currentAccount);
    }
    
    return result;
}

void ATMEngine::deposit(double amount) {
    if (!authenticated || !currentAccount) return;
    
    currentAccount->deposit(amount);
    journal.appendTransaction(Transaction{Transaction::DEPOSIT, amount, std::time(nullptr)});
    recovery.saveAccountState(*currentAccount);
}

void ATMEngine::showHistory() {
    if (!authenticated || !currentAccount) return;
    
    auto transactions = currentAccount->getLastNTransactions(5);
    std::cout << "\n📜 Последние 5 операций:\n";
    if (transactions.empty()) {
        std::cout << "  Операций пока нет\n";
    }
    for (const auto& t : transactions) {
        std::cout << "  " << t.toString() << "\n";
    }
}

void ATMEngine::logout() {
    authenticated = false;
    currentCard = nullptr;
    currentAccount = nullptr;
}

void ATMEngine::recoverFromCrash(Account* account) {
    currentAccount = account;
    recovery.recover(*currentAccount, journal);
}
