cmake_minimum_required(VERSION 3.25)

find_package(Boost REQUIRED)
find_package(libpqxx REQUIRED)

add_library(cpppg
    src/ConnectionPool.cpp
)

target_compile_features(cpppg PUBLIC cxx_std_23)
target_include_directories(cpppg
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
if(TARGET Boost::headers)
    target_link_libraries(cpppg PUBLIC Boost::headers)
elseif(TARGET Boost::boost)
    target_link_libraries(cpppg PUBLIC Boost::boost)
endif()

target_link_libraries(cpppg
    PUBLIC
        libpqxx::pqxx
)
