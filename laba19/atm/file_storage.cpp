#include "file_storage.h"
#include <fstream>
#include <iostream>

FileStorage::FileStorage(const std::string& cf, const std::string& af) 
    : cardsFile(cf), accountsFile(af) {}

void FileStorage::createDefaultData() {
    // Стартовые карты для примера
    cards["1234-5678-9012-3456"] = std::make_unique<Card>("1234-5678-9012-3456", "1234");
    cards["9876-5432-1098-7654"] = std::make_unique<Card>("9876-5432-1098-7654", "4321");
    accounts["1234-5678-9012-3456"] = std::make_unique<Account>(5000.0);
    accounts["9876-5432-1098-7654"] = std::make_unique<Account>(10000.0);
    save();
}

void FileStorage::load() {
    std::ifstream file(cardsFile);
    if (!file) {
        createDefaultData();
        return;
    }
    
    // Загружаем карты
    std::string num, pin;
    while (file >> num >> pin) {
        cards[num] = std::make_unique<Card>(num, pin);
    }
    file.close();
    
    // Загружаем счета
    std::ifstream accFile(accountsFile);
    double balance;
    while (accFile >> num >> balance) {
        accounts[num] = std::make_unique<Account>(balance);
    }
    accFile.close();
    
    // Если после загрузки нет карт - создаём дефолтные
    if (cards.empty()) {
        createDefaultData();
    }
}

void FileStorage::save() {
    std::ofstream file(cardsFile);
    for (const auto& [num, card] : cards) {
        file << num << " " << "****" << std::endl;
    }
    std::ofstream accFile(accountsFile);
    for (const auto& [num, acc] : accounts) {
        accFile << num << " " << acc->getBalance() << std::endl;
    }
}

Card* FileStorage::getCard(const std::string& number) {
    auto it = cards.find(number);
    if (it != cards.end()) {
        return it->second.get();
    }
    
    // КАРТА НЕ НАЙДЕНА - СОЗДАЁМ НОВУЮ!
    std::cout << "\n🔨 Карта не найдена. Создаём новую карту...\n";
    std::string newPin;
    std::cout << "💰 Создайте PIN для новой карты (4 цифры): ";
    std::cin >> newPin;
    
    // Создаём новую карту
    cards[number] = std::make_unique<Card>(number, newPin);
    accounts[number] = std::make_unique<Account>(0.0); // Баланс 0 для новой карты
    
    save();
    std::cout << "✅ Новая карта " << number << " успешно создана!\n";
    std::cout << "💳 Баланс: 0 руб. Пополните счёт для начала.\n\n";
    
    return cards[number].get();
}

Account* FileStorage::getAccount(const std::string& cardNumber) {
    auto it = accounts.find(cardNumber);
    if (it != accounts.end()) {
        return it->second.get();
    }
    
    // Если счёта нет - создаём
    accounts[cardNumber] = std::make_unique<Account>(0.0);
    save();
    return accounts[cardNumber].get();
}
