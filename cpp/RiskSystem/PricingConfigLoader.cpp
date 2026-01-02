#include "PricingConfigLoader.h"
#include <tinyxml2.h>
#include <stdexcept>

using namespace tinyxml2;

PricingEngineConfig PricingConfigLoader::loadConfig() {
    if (configFile_.empty()) {
        throw std::invalid_argument("Config file not set");
    }

    XMLDocument doc;
    XMLError result = doc.LoadFile(configFile_.c_str());
    if (result != XML_SUCCESS) {
        throw std::runtime_error("Failed to load pricing config file");
    }

    XMLElement* root = doc.FirstChildElement("PricingEngines");
    if (!root) {
        throw std::runtime_error("Missing PricingEngines root element");
    }

    PricingEngineConfig config;

    for (XMLElement* engine = root->FirstChildElement("Engine");
         engine != nullptr;
         engine = engine->NextSiblingElement("Engine")) {

        const char* tradeType = engine->Attribute("tradeType");
        const char* assembly = engine->Attribute("assembly");
        const char* pricingEngine = engine->Attribute("pricingEngine");

        if (!tradeType || !assembly || !pricingEngine) {
            throw std::runtime_error("Invalid Engine element in pricing config");
        }

        config.emplace_back(
            tradeType,
            assembly,
            pricingEngine
        );
    }

    return config;
}
