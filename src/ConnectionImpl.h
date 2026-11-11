#pragma once

#include <cpppg/Connection.h>

#include <pqxx/connection>

#include <string>

namespace db {

    struct Connection::Impl {
        explicit Impl(std::string_view conn_info)
            : connection_(std::string{conn_info}) {}

        pqxx::connection connection_;
    };

}
