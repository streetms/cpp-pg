#pragma once
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <pqxx/pqxx>

namespace db {
    class ConnectionPool;

    class ConnectionGuard {
    public:
        ConnectionGuard() = default;

        ConnectionGuard(pqxx::connection* conn, ConnectionPool* pool)
            : conn_(conn), pool_(pool) {}
        ConnectionGuard(const ConnectionGuard&) = delete;
        ConnectionGuard& operator=(const ConnectionGuard&) = delete;
        ConnectionGuard(ConnectionGuard&& other) noexcept ;

        ConnectionGuard& operator=(ConnectionGuard&& other) noexcept;
        ~ConnectionGuard();

        pqxx::connection& operator*() { return *conn_; }
        pqxx::connection* operator->() { return conn_; }

        explicit operator bool() const { return conn_ != nullptr; }

    private:
        void release();
        pqxx::connection* conn_ = nullptr;
        ConnectionPool* pool_ = nullptr;
    };
    class ConnectionPool {
    public:
        using ConnectionInitializer = std::function<void(pqxx::connection&)>;

        ConnectionPool(std::string_view conn_info, size_t size,
                       ConnectionInitializer initialize_connection = {});

        ConnectionGuard acquire();

    private:
        void release(pqxx::connection* conn);
        std::vector<std::unique_ptr<pqxx::connection>> connections_;
        std::queue<pqxx::connection*> idle_;

        std::mutex mutex_;
        std::condition_variable cv_;

        friend class ConnectionGuard;
    };
}
