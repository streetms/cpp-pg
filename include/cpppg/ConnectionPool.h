#pragma once
#include <cstddef>
#include <functional>
#include <memory>
#include <string_view>

#include "Connection.h"
namespace db {
    class ConnectionPool;

    class ConnectionGuard {
    public:
        ConnectionGuard() = default;

        ConnectionGuard(Connection* conn, ConnectionPool* pool)
            : conn_(conn), pool_(pool) {}
        ConnectionGuard(const ConnectionGuard&) = delete;
        ConnectionGuard& operator=(const ConnectionGuard&) = delete;
        ConnectionGuard(ConnectionGuard&& other) noexcept ;

        ConnectionGuard& operator=(ConnectionGuard&& other) noexcept;
        ~ConnectionGuard();

        Connection& operator*() { return *conn_; }
        Connection* operator->() { return conn_; }

        explicit operator bool() const { return conn_ != nullptr; }

    private:
        void release();
        Connection* conn_ = nullptr;
        ConnectionPool* pool_ = nullptr;
    };
    class ConnectionPool {
    public:
        using ConnectionInitializer =
            std::function<void(Connection&)>;
        ConnectionPool(std::string_view conn_info, std::size_t size,
                       ConnectionInitializer initialize_connection = {});
        ~ConnectionPool();

        ConnectionPool(const ConnectionPool&) = delete;
        ConnectionPool& operator=(const ConnectionPool&) = delete;
        ConnectionPool(ConnectionPool&&) = delete;
        ConnectionPool& operator=(ConnectionPool&&) = delete;

        ConnectionGuard acquire();

    private:
        void release(Connection* conn);
        struct Impl;
        std::unique_ptr<Impl> impl_;

        friend class ConnectionGuard;
    };

}
