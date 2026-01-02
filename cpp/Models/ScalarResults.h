#include "ScalarResults.h"

ScalarResults::~ScalarResults() = default;

std::optional<ScalarResult> ScalarResults::operator[](const std::string& tradeId) const {
    auto it = results_.find(tradeId);
    if (it != results_.end()) {
        return ScalarResult(it->first, it->second);
    }
    return std::nullopt;
}

bool ScalarResults::containsTrade(const std::string& tradeId) const {
    return results_.find(tradeId) != results_.end() ||
           errors_.find(tradeId) != errors_.end();
}

void ScalarResults::addResult(const std::string& tradeId, double result) {
    results_[tradeId] = result;
}

void ScalarResults::addError(const std::string& tradeId, const std::string& error) {
    errors_[tradeId] = error;
}

/* =======================
   Iterator implementation
   ======================= */

class ScalarResultsIteratorImpl {
public:
    explicit ScalarResultsIteratorImpl(
        std::map<std::string, double>::const_iterator it)
        : it_(it) {}

    std::map<std::string, double>::const_iterator it_;
};

ScalarResults::Iterator::Iterator(
    std::map<std::string, double>::const_iterator it)
    : impl_(std::make_shared<ScalarResultsIteratorImpl>(it)) {}

ScalarResults::Iterator& ScalarResults::Iterator::operator++() {
    ++impl_->it_;
    return *this;
}

ScalarResult ScalarResults::Iterator::operator*() const {
    return ScalarResult(impl_->it_->first, impl_->it_->second);
}

bool ScalarResults::Iterator::operator!=(const Iterator& other) const {
    return impl_->it_ != other.impl_->it_;
}

ScalarResults::Iterator ScalarResults::begin() const {
    return Iterator(results_.cbegin());
}

ScalarResults::Iterator ScalarResults::end() const {
    return Iterator(results_.cend());
}
