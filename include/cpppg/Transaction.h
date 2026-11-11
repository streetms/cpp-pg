//
// Created by konstantin on 08.11.2026.
//

#pragma once

#include "Parameter.h"
#include "Result.h"

#include <memory>
#include <span>
#include <string_view>

namespace db {

    class ConnectionGuard;
    class Database;

    class Transaction {
    public:
        Transaction(const Transaction&) = delete;
        Transaction& operator=(const Transaction&) = delete;

        ~Transaction();

        Result exec_prepared(
            std::string_view name,
            std::span<const Parameter> parameters = {});

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;

        explicit Transaction(ConnectionGuard& connection);
        void commit();

        friend class Database;
    };

}
