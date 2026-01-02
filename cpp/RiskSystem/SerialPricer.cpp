#include "SerialPricer.h"
#include "GovBondPricingEngine.h"
#include "CorpBondPricingEngine.h"
#include "FxPricingEngine.h"
#include <stdexcept>

SerialPricer::~SerialPricer() {
    for (auto& p : pricers_) {
        delete p.second;
    }
}

void SerialPricer::loadPricers() {
    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");

    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();

    for (const auto& configItem : pricerConfig) {
        const std::string& tradeType = configItem.getTradeType();

        if (pricers_.find(tradeType) != pricers_.end()) {
            continue; // already loaded
        }

        if (tradeType == "GovBond") {
            pricers_[tradeType] = new GovBondPricingEngine();
        }
        else if (tradeType == "CorpBond") {
            pricers_[tradeType] = new CorpBondPricingEngine();
        }
        else if (tradeType == "FxSpot" || tradeType == "FxFwd") {
            pricers_[tradeType] = new FxPricingEngine();
        }
        else {
            throw std::runtime_error("Unknown trade type: " + tradeType);
        }
    }
}
