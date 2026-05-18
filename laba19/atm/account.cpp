#include "account.h"
#include <algorithm>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

Account::Account(double initialBalance) : balance(initialBalance), dailyWithdrawn(0) {
    auto t = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d");
    lastDailyReset = ss.str();
}

double Account::getBalance() const { return balance; }

double Account::calculateFee(double amount) const {
    double fee = amount * 0.01;
    return std::max(fee, 30.0);
}

bool Account::checkDailyLimit(double amount) const {
    Account* nonConstThis = const_cast<Account*>(this);
    nonConstThis->resetDailyIfNeeded();
    return (dailyWithdrawn + amount) <= 50000;
}

void Account::resetDailyIfNeeded() {
    auto now = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d");
    std::string today = ss.str();
    if (today != lastDailyReset) {
        dailyWithdrawn = 0;
        lastDailyReset = today;
    }
}

bool Account::withdraw(double amount, bool withFee) {
    resetDailyIfNeeded();
    
    // ПРОВЕРКА ЛИМИТА ЗА ОПЕРАЦИЮ
    if (amount > 10000) {
        std::cout << "❌ Ошибка: максимальная сумма снятия за раз - 10 000 руб\n";
        std::cout << "   Вы пытаетесь снять " << amount << " руб\n";
        return false;
    }
    
    if (amount <= 0) {
        std::cout << "❌ Ошибка: сумма должна быть больше 0\n";
        return false;
    }
    
    // ПРОВЕРКА ДНЕВНОГО ЛИМИТА
    if (!checkDailyLimit(amount)) {
        std::cout << "❌ Ошибка: превышен суточный лимит (50 000 руб)\n";
        std::cout << "   Сегодня уже снято: " << dailyWithdrawn << " руб\n";
        std::cout << "   Попытка снять: " << amount << " руб\n";
        return false;
    }
    
    double fee = calculateFee(amount);
    double totalToDeduct = amount;
    if (withFee) totalToDeduct += fee;
    
    // ПРОВЕРКА БАЛАНСА
    if (balance < totalToDeduct) {
        std::cout << "❌ Ошибка: недостаточно средств\n";
        std::cout << "   Баланс: " << balance << " руб\n";
        std::cout << "   Требуется: " << totalToDeduct << " руб (включая комиссию " << fee << " руб)\n";
        return false;
    }
    
    // ВСЁ ХОРОШО - СНИМАЕМ
    balance -= totalToDeduct;
    dailyWithdrawn += amount;
    Transaction t{Transaction::WITHDRAW, amount, std::time(nullptr)};
    addTransaction(t);
    
    std::cout << "✅ Снято: " << amount << " руб\n";
    std::cout << "💰 Комиссия: " << fee << " руб\n";
    std::cout << "💰 Итого списано: " << totalToDeduct << " руб\n";
    std::cout << "💰 Остаток: " << balance << " руб\n";
    
    return true;
}

void Account::deposit(double amount) {
    if (amount <= 0) {
        std::cout << "❌ Ошибка: сумма должна быть больше 0\n";
        return;
    }
    
    balance += amount;
    Transaction t{Transaction::DEPOSIT, amount, std::time(nullptr)};
    addTransaction(t);
    
    std::cout << "✅ Пополнено: " << amount << " руб\n";
    std::cout << "💰 Новый баланс: " << balance << " руб\n";
}

void Account::addTransaction(const Transaction& t) {
    recentTransactions.insert(recentTransactions.begin(), t);
    if (recentTransactions.size() > 5) recentTransactions.pop_back();
}

std::vector<Transaction> Account::getLastNTransactions(int n) const {
    std::vector<Transaction> result;
    for (size_t i = 0; i < std::min((size_t)n, recentTransactions.size()); i++) {
        result.push_back(recentTransactions[i]);
    }
    return result;
}

void Account::setBalance(double b) { balance = b; }
