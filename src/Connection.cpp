#include "ConnectionImpl.h"

namespace db {

    Connection::Connection(std::string_view conn_info)
        : impl_(std::make_unique<Impl>(conn_info)) {}

    Connection::~Connection() = default;

    void Connection::prepare(std::string_view name, std::string_view sql) {
        impl_->connection_.prepare(name, sql);
    }

}
