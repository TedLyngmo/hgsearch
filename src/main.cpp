#include "main.hpp"
#include "cppmain.hpp"

#include <cerrno>
#include <cstring>
#include <iostream>

int error(const std::string_view& program, const std::string_view& file) {
    std::cerr << program << ": " << file << ": " << std::strerror(errno) << '\n';
    return 1;
}

int main(int argc, char* argv[]) {
    return cppmain(argv[0], {argv + 1, argv + argc});
}
