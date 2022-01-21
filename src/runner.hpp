#pragma once
#include "tsv.hpp"

#include <algorithm>
#include <atomic>
#include <execution>
#include <iostream>
#include <string_view>

template<class Container>
int run(const std::string_view& program, const std::string_view& tsvname, std::istream& tsvfile, const Container& chrs,
        std::ostream& result) {
    int error = 0;
    tsv_record tsvr;

    for(std::uintmax_t line = 1; tsvfile >> tsvr; ++line) {
        if(tsvr.size() < 7) {
            std::cerr << program << ": " << tsvname << ": Error on line " << line << '\n';
            error = 1;
        } else {
            // count hits
            std::atomic<std::uint32_t> count = 0;

            const auto needle_size = tsvr[6].size();
            // const std::boyer_moore_searcher searcher(tsvr[6].begin(), tsvr[6].end());
            const std::boyer_moore_horspool_searcher searcher(tsvr[6].begin(), tsvr[6].end());

            std::for_each(std::execution::par_unseq, chrs.begin(), chrs.end(),
                          [&searcher, &needle_size, &count](const auto& chromosome) {
                              for(auto cb = chromosome.begin();
                                  (cb = std::search(cb, chromosome.end(), searcher)) != chromosome.end(); cb += needle_size) {
                                  ++count;
                              };
                          });
            tsvr.emplace_back(std::to_string(count));
            result << tsvr << std::flush;
        }
    }

    return error;
}
