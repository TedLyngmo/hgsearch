#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

class tsv_record {
public:
    const std::string& operator[](size_t idx) const;
    size_t size() const;

    template<class... Args>
    decltype(auto) emplace_back(Args&&... args) {
        return data.emplace_back(std::forward<Args>(args)...);
    }

    inline static unsigned max_in_cols = 0; // 0 read all columns

    friend std::istream& operator>>(std::istream& is, tsv_record& r);
    friend std::ostream& operator<<(std::ostream& os, const tsv_record& r);

private:
    std::vector<std::string> data;
};
