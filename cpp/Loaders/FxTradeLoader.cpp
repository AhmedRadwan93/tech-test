#include "FxTradeLoader.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <stdexcept>

static const char separator = '¬';

FxTrade* createTradeFromLine(const std::string& line) {
    std::vector<std::string> items;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, separator)) {
        items.push_back(item);
    }

    if (items.size() != 9) {
        throw std::runtime_error("Invalid FX trade line");
    }

    const std::string& type = items[0];
    FxTrade* trade = nullptr;

    if (type == "FxSpot") {
        trade = new FxSpotTrade(items[8]);
    }
    else if (type == "FxFwd") {
        trade = new FxFwdTrade(items[8]);
    }
    else {
        throw std::runtime_error("Unknown FX trade type: " + type);
    }

    // Trade date
    std::tm tm = {};
    std::istringstream tradeDateStream(items[1]);
    tradeDateStream >> std::get_time(&tm, "%Y-%m-%d");
    trade->setTradeDate(
        std::chrono::system_clock::from_time_t(std::mktime(&tm))
    );

    // Instrument = Ccy1 + Ccy2
    trade->setInstrument(items[2] + items[3]);

    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));

    // Value date
    std::tm vtm = {};
    std::istringstream valueDateStream(items[6]);
    valueDateStream >> std::get_time(&vtm, "%Y-%m-%d");
    trade->setValueDate(
        std::chrono::system_clock::from_time_t(std::mktime(&vtm))
    );

    trade->setCounterparty(items[7]);

    return trade;
}

std::vector<ITrade*> FxTradeLoader::loadTrades() {
    if (dataFile_.empty()) {
        throw std::invalid_argument("Data file not set");
    }

    std::ifstream file(dataFile_);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + dataFile_);
    }

    std::vector<ITrade*> trades;
    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        // Skip file title, header, and END row
        if (lineCount < 2 || line.rfind("END", 0) == 0) {
            lineCount++;
            continue;
        }

        trades.push_back(createTradeFromLine(line));
        lineCount++;
    }

    return trades;
}

std::string FxTradeLoader::getDataFile() const {
    return dataFile_;
}

void FxTradeLoader::setDataFile(const std::string& file) {
    dataFile_ = file;
}
