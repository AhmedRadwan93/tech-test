#ifndef SCALARRESULT_H
#define SCALARRESULT_H

#include <string>
#include <optional>
#include <stdexcept>
#include <memory>

class ScalarResult {
public:
    ScalarResult(const std::string& tradeId, const std::optional<double>& result, const std::optional<std::string>& error)
        : tradeId_(tradeId), result_(result), error_(error) {
        if (tradeId.empty()) {
            throw std::invalid_argument("A non null, non empty trade id must be provided");
        }
    }
    
    std::string getTradeId() const { return tradeId_; }
    std::optional<double> getResult() const { return result_; }
    std::optional<std::string> getError() const { return error_; }
    
private:
    std::string tradeId_;
    std::optional<double> result_;
    std::optional<std::string> error_;
};


class Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = ScalarResult;
    using difference_type = std::ptrdiff_t;
    using pointer = ScalarResult*;
    using reference = ScalarResult&;

    Iterator() = default;

    Iterator& operator++();
    ScalarResult operator*() const;
    bool operator!=(const Iterator& other) const;

private:
    friend class ScalarResults;
    explicit Iterator(std::map<std::string, double>::const_iterator it);

    std::shared_ptr<class ScalarResultsIteratorImpl> impl_;
};
#endif // SCALARRESULT_H

