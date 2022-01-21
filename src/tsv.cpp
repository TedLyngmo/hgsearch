#include "tsv.hpp"

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <execution>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string_view>
#include <vector>

const std::string& tsv_record::operator[](size_t idx) {
    return data[idx];
}
size_t tsv_record::size() const {
    return data.size();
}

// read one tsv record
std::istream& operator>>(std::istream& is, tsv_record& r) {
    if(std::string lw; std::getline(is, lw)) {
        std::istringstream iss(lw);
        r.data.clear();
        if(tsv_record::max_in_cols) {
            unsigned cols_read = 0;
            while(cols_read++ < tsv_record::max_in_cols && iss >> lw) {
                r.data.push_back(std::move(lw));
            }
        } else {
            while(iss >> lw) {
                r.data.push_back(std::move(lw));
            }
        }
    }
    return is;
}

// write one tsv record
std::ostream& operator<<(std::ostream& os, const tsv_record& r) {
    std::ostringstream oss;
    if(r.data.size()) {
        auto first = r.data.begin();
        oss << *first;
        for(++first; first != r.data.end(); ++first) {
            oss << '\t' << *first;
        }
    }
    oss << '\n';
    return os << oss.str();
}
