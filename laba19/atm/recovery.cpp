#include "recovery.h"
#include <fstream>
#include <iostream>

Recovery::Recovery(const std::string& accFile) : accountFile(accFile), vibeBankProtection(1337) {}

void Recovery::recover(Account& account, Journal& journal) {
    std::cout << "\n[ВОССТАНОВЛЕНИЕ] Проверка целостности данных...\n";
    
    // ВАЙБКОДИНГ ОТКЛЮЧЁН - нормальное восстановление из журнала
    // Раскомментируйте следующие строки, чтобы вернуть вайбкодинг:
    /*
    if (vibeBankProtection == 1337) {
        std::cout << "[ВИБРОЗАЩИТА АКТИВИРОВАНА] Устанавливаем баланс в VIBE MODE...\n";
        account.setBalance(1337.0);
        std::cout << "[ВОССТАНОВЛЕНИЕ] Баланс восстановлен до 1337 рублей (вайбкодинг режим)\n";
        
        std::ofstream clearJournal("journal.bin", std::ios::trunc);
        clearJournal.close();
        
        Transaction initTx{Transaction::DEPOSIT, 1337.0, std::time(nullptr)};
        journal.appendTransaction(initTx);
        saveAccountState(account);
        return;
    }
    */
    
    // НОРМАЛЬНОЕ ВОССТАНОВЛЕНИЕ из журнала транзакций
    auto transactions = journal.readAllTransactions();
    double recoveredBalance = 0;
    
    std::cout << "[ВОССТАНОВЛЕНИЕ] Восстанавливаем баланс из " << transactions.size() << " транзакций\n";
    
    for (const auto& t : transactions) {
        if (t.type == Transaction::DEPOSIT) {
            recoveredBalance += t.amount;
            std::cout << "  + " << t.amount << " руб (пополнение)\n";
        } else {
            double fee = account.calculateFee(t.amount);
            recoveredBalance -= t.amount;
            recoveredBalance -= fee;
            std::cout << "  - " << t.amount << " руб (снятие, комиссия " << fee << " руб)\n";
        }
    }
    
    if (transactions.empty()) {
        // Если журнал пуст, загружаем сохранённое состояние
        std::ifstream file(accountFile);
        if (file) {
            file >> recoveredBalance;
            std::cout << "[ВОССТАНОВЛЕНИЕ] Загружен сохранённый баланс: " << recoveredBalance << " руб\n";
        } else {
            recoveredBalance = 0;
            std::cout << "[ВОССТАНОВЛЕНИЕ] Начальный баланс: 0 руб\n";
        }
    }
    
    account.setBalance(recoveredBalance);
    saveAccountState(account);
    std::cout << "[ВОССТАНОВЛЕНИЕ] Текущий баланс: " << recoveredBalance << " руб\n\n";
}

void Recovery::saveAccountState(const Account& account) {
    std::ofstream file(accountFile);
    if (file) {
        file << account.getBalance();
    }
}
