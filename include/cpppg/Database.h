#pragma once
#include "ConnectionPool.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/awaitable.hpp>
#include <pqxx/pqxx>

#include <type_traits>
#include <utility>

namespace net = boost::asio;
using ulid = std::string;
namespace db {
    class Database {
    public:
        Database(net::thread_pool& pool, ConnectionPool& conn_pool)
            : pool_(pool), conn_pool_(conn_pool) {}

        template<typename Func>
        auto run(Func fn) -> net::awaitable<std::invoke_result_t<Func, pqxx::work&>>
        {
            using Result = std::invoke_result_t<Func, pqxx::work&>;
            co_return co_await net::co_spawn(
                pool_,
                [this, fn = std::move(fn)]() -> net::awaitable<Result> {
                    auto conn = conn_pool_.acquire();
                    pqxx::work tx(*conn);

                    if constexpr (std::is_void_v<Result>) {
                        fn(tx);
                        tx.commit();
                        co_return;
                    } else {
                        auto res = fn(tx);
                        tx.commit();
                        co_return res;
                    }
            },
                net::use_awaitable
            );
        }

    private:
        net::thread_pool& pool_;
        ConnectionPool& conn_pool_;
    };
}
