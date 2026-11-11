#include <cpppg/ConnectionPool.h>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>
#include <vector>

namespace db {
    struct ConnectionPool::Impl {
        std::vector<std::unique_ptr<Connection>> connections_;
        std::queue<Connection*> idle_;
        std::mutex mutex_;
        std::condition_variable cv_;
    };

    ConnectionGuard::ConnectionGuard(ConnectionGuard &&other) noexcept
            : conn_(other.conn_), pool_(other.pool_) {
        other.conn_ = nullptr;
        other.pool_ = nullptr;
    }

    ConnectionGuard & ConnectionGuard::operator=(ConnectionGuard &&other) noexcept {
        if (this != &other) {
            release();

            conn_ = other.conn_;
            pool_ = other.pool_;

            other.conn_ = nullptr;
            other.pool_ = nullptr;
        }
        return *this;
    }

    ConnectionGuard::~ConnectionGuard() {
        release();
    }

    void ConnectionGuard::release() {
        if (conn_ && pool_) {
            pool_->release(conn_);
            conn_ = nullptr;
            pool_ = nullptr;
        }
    }

    ConnectionPool::ConnectionPool(std::string_view conn_info, std::size_t size,
                                   ConnectionInitializer initialize_connection)
        : impl_(std::make_unique<Impl>()) {
        for (std::size_t i = 0; i < size; ++i) {
            impl_->connections_.emplace_back(
                std::unique_ptr<Connection>(new Connection(conn_info))
            );
            if (initialize_connection) {
                initialize_connection(*impl_->connections_.back());
            }
            impl_->idle_.push(impl_->connections_.back().get());
        }
    }

    ConnectionPool::~ConnectionPool() = default;

    ConnectionGuard ConnectionPool::acquire() {
        std::unique_lock lock(impl_->mutex_);

        impl_->cv_.wait(lock, [&] { return !impl_->idle_.empty(); });

        auto* conn = impl_->idle_.front();
        impl_->idle_.pop();

        return ConnectionGuard(conn, this);
    }

    void ConnectionPool::release(Connection *conn) {
        std::lock_guard lock(impl_->mutex_);
        impl_->idle_.push(conn);
        impl_->cv_.notify_one();
    }
}
