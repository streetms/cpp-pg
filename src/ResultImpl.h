#pragma once

#include <cpppg/Result.h>

#include <pqxx/result>

#include <utility>

namespace db {

    struct Result::Impl {
        explicit Impl(pqxx::result result)
            : result_(std::move(result)) {}

        pqxx::result result_;
    };

}
