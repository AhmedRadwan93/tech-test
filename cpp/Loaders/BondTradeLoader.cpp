#include "BondTradeLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>
BondTrade* BondTradeLoader::createTradeFromLine(std::string line) {
    std::vector<std::string> items;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, separator)) {
        items.push_back(item);
    }

    if (items.size() != 7) {
        throw std::runtime_error("Invalid line format");
    }

    const std::string& type = items[0];
    BondTrade* trade = nullptr;

    if (type == "GovBond" || type == "Supra") {
        trade = new GovBondTrade(items[6]);
    }
    else if (type == "CorpBond") {
        trade = new CorpBondTrade(items[6]);
    }
    else {
        throw std::runtime_error("Unknown bond type: " + type);
    }

    // Trade date
    std::tm tm = {};
    std::istringstream dateStream(items[1]);
    dateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setTradeDate(timePoint);

    trade->setInstrument(items[2]);
    trade->setCounterparty(items[3]);
    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));

    return trade;
}
