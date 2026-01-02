#include "ScreenResultPrinter.h"
#include <iostream>

void ScreenResultPrinter::printResults(ScalarResults& results) {
    for (const auto& result : results) {
        const std::string& tradeId = result.getTradeId();

        bool hasResult = true;
        bool hasError = result.hasError();

        std::cout << tradeId;

        if (hasResult) {
            std::cout << " : " << result.getValue();
        }

        if (hasError) {
            std::cout << " : " << result.getError();
        }

        std::cout << std::endl;
    }
}











