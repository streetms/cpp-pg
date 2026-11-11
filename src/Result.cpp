#include "ResultImpl.h"

#include <pqxx/pqxx>

#include <stdexcept>
#include <utility>

namespace db {

    Result::Result(std::unique_ptr<Impl> impl) noexcept
        : impl_(std::move(impl)) {}

    Result::Result(Result&&) noexcept = default;
    Result& Result::operator=(Result&&) noexcept = default;
    Result::~Result() = default;

    std::size_t Result::size() const noexcept {
        return impl_ ? impl_->result_.size() : 0;
    }

    std::optional<std::string_view> Result::get(
        std::size_t row, std::string_view column) const {
        if (row >= size()) {
            throw std::out_of_range("Result row index is out of range");
        }

        const auto& result = impl_->result_;
        for (pqxx::row_size_type index = 0; index < result.columns(); ++index) {
            if (column == result.column_name(index)) {
                const auto field = result.at(static_cast<pqxx::result::size_type>(row), index);
                if (field.is_null()) {
                    return std::nullopt;
                }
                return std::string_view{field.c_str(), field.size()};
            }
        }

        throw std::out_of_range("Result column was not found");
    }

}
