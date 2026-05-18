#include "transaction.h"
#include <sstream>
#include <iomanip>
std::string Transaction::toString() const {
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S");
    ss << " | " << (type == DEPOSIT ? "Пополнение" : "Снятие") << " | " << amount << " руб";
    return ss.str();
}
