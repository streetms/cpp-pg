#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace db {

    using Bytes = std::vector<std::byte>;
    using StringArray = std::vector<std::string>;

    using Parameter = std::variant<
        std::nullptr_t,
        std::int64_t,
        std::uint64_t,
        std::string,
        Bytes,
        StringArray
    >;

}
