//
// Created by konstantin on 08.11.2026.
//

#include <cpppg/Transaction.h>
#include <cpppg/ConnectionPool.h>

#include "ConnectionImpl.h"
#include "ResultImpl.h"

#include <pqxx/pqxx>

namespace db {

    struct Transaction::Impl {
        explicit Impl(pqxx::connection& connection)
            : tx(connection) {}

        pqxx::work tx;
    };

    Transaction::Transaction(ConnectionGuard& connection)
        : impl_(std::make_unique<Impl>(connection->impl_->connection_)) {}

    Transaction::~Transaction() = default;

    Result Transaction::exec_prepared(
        std::string_view name, std::span<const Parameter> parameters) {
        pqxx::params converted{impl_->tx};
        converted.reserve(parameters.size());
        for (const auto& parameter : parameters) {
            std::visit([&converted](const auto& value) {
                converted.append(value);
            }, parameter);
        }

        return Result(std::make_unique<Result::Impl>(
            impl_->tx.exec(pqxx::prepped{name}, converted)));
    }

    void Transaction::commit() {
        impl_->tx.commit();
    }

}
