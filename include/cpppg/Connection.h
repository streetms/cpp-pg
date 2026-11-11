#pragma once

#include <memory>
#include <string_view>

namespace db {

    class ConnectionPool;
    class Transaction;

    class Connection {
    public:
        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
        ~Connection();

        void prepare(std::string_view name, std::string_view sql);

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;

        explicit Connection(std::string_view conn_info);

        friend class ConnectionPool;
        friend class Transaction;
    };

}
