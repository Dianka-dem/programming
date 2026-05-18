#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include "card.h"
#include "account.h"
#include "transaction.h"
#include "journal.h"
#include "recovery.h"
#include "atm_engine.h"
#include "file_storage.h"

void printLogo() {
    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════╗\n";
    std::cout << "  ║     💎 VIBE BANK ATM SYSTEM 💎      ║\n";
    std::cout << "  ║      Вайбкодинг режим: АКТИВИРОВАН   ║\n";
    std::cout << "  ╚══════════════════════════════════════╝\n";
}

void showMenu() {
    std::cout << "\n  ┌────────────────────────────────────┐\n";
    std::cout << "  │ 1️⃣  Проверить баланс               │\n";
    std::cout << "  │ 2️⃣  Снять наличные                 │\n";
    std::cout << "  │ 3️⃣  Пополнить счет                 │\n";
    std::cout << "  │ 4️⃣  История операций               │\n";
    std::cout << "  │ 5️⃣  Выйти                          │\n";
    std::cout << "  └────────────────────────────────────┘\n";
    std::cout << "  Выберите опцию: ";
}

int main() {
    printLogo();
    
    FileStorage storage("cards.dat", "accounts.dat");
    storage.load();
    
    Journal journal("journal.bin");
    Recovery recovery("account_state.dat");
    ATMEngine atm("journal.bin", "account_state.dat");
    
    std::string cardNumber;
    std::cout << "\n💳 Введите номер карты: ";
    std::cin >> cardNumber;
    
    Card* card = storage.getCard(cardNumber);
    if (!card) {
        std::cout << "❌ Карта не найдена!\n";
        return 1;
    }
    
    if (card->isBlocked()) {
        std::cout << "🔒 Карта заблокирована!\n";
        return 1;
    }
    
    if (!atm.insertCard(card)) {
        std::cout << "❌ Ошибка при чтении карты\n";
        return 1;
    }
    
    int attempts = 0;
    bool authSuccess = false;
    
    while (attempts < 3 && !authSuccess) {
        std::string pin;
        std::cout << "🔐 Введите PIN: ";
        std::cin >> pin;
        
        if (atm.authenticate(pin)) {
            authSuccess = true;
            std::cout << "✅ Добро пожаловать в VIBE BANK!\n";
        } else {
            attempts++;
            std::cout << "❌ Неверный PIN. Осталось попыток: " << (3 - attempts) << "\n";
            if (attempts >= 3) {
                std::cout << "🔒 Карта заблокирована!\n";
                return 1;
            }
        }
    }
    
    if (!authSuccess) return 1;
    
    Account* account = storage.getAccount(cardNumber);
    if (!account) {
        std::cout << "❌ Ошибка загрузки счета\n";
        return 1;
    }
    
    atm.recoverFromCrash(account);
    
    int choice;
    do {
        showMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                atm.checkBalance();
                break;
            case 2: {
                double amount;
                std::cout << "💰 Сумма для снятия (макс 10 000 руб): ";
                std::cin >> amount;
                atm.withdraw(amount);
                break;
            }
            case 3: {
                double amount;
                std::cout << "💰 Сумма для пополнения: ";
                std::cin >> amount;
                atm.deposit(amount);
                break;
            }
            case 4:
                atm.showHistory();
                break;
            case 5:
                std::cout << "👋 Спасибо, что пользуетесь VIBE BANK!\n";
                break;
            default:
                std::cout << "❌ Неверный выбор\n";
        }
    } while (choice != 5);
    
    storage.save();
    std::cout << "\n✨ Вайбкодинг завершён! Баланс защищён вибрацией ✨\n";
    
    return 0;
}
