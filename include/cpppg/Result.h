#pragma once

#include <memory>
#include <optional>
#include <string_view>

namespace db {

    class Transaction;

    class Result {
    public:
        Result(const Result&) = delete;
        Result& operator=(const Result&) = delete;
        Result(Result&&) noexcept;
        Result& operator=(Result&&) noexcept;
        ~Result();
        std::size_t size() const noexcept;
        std::optional<std::string_view> get(
            std::size_t row, std::string_view column) const;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;

        explicit Result(std::unique_ptr<Impl> impl) noexcept;

        friend class Transaction;
    };

}
