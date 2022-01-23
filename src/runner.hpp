#pragma once
#include "tsv.hpp"

#include <algorithm>
#include <atomic>
#include <execution>
#include <iostream>
#include <iterator>
#include <shared_mutex>
#include <stdexcept>
#include <string_view>
#include <vector>

std::vector<tsv_record> get_basepairs(std::istream& tsvfile) {
    return std::vector<tsv_record>(std::istream_iterator<tsv_record>(tsvfile), std::istream_iterator<tsv_record>{});
}

template<class Container>
int run([[maybe_unused]] const std::string_view& program, [[maybe_unused]] const std::string_view& tsvname, std::istream& tsvfile,
        const Container& chrs, std::ostream& result) {
    auto basepairs = get_basepairs(tsvfile);
    std::shared_mutex mtx_result;

    std::for_each(std::execution::par_unseq, basepairs.begin(), basepairs.end(), [&chrs, &result, &mtx_result](auto& bpline) {
        // count hits
        std::atomic<std::uint32_t> count = 0;
        auto& bp = bpline[6];

#ifdef USE_MEMMEM
        std::for_each(std::execution::par_unseq, chrs.begin(), chrs.end(), [&bp, &count](const auto& chromosome) {
            const char* chr_b = &chromosome[0];
            const char* chr_e = &chromosome[0] + chromosome.size();
            for(; chr_b != chr_e && (chr_b = (const char*)memmem(chr_b, chr_e - chr_b, &bp[0], bp.size())) != nullptr;
                chr_b += bp.size()) {
                ++count;
            }
        });
#else
        // const std::boyer_moore_searcher searcher(bp.begin(), bp.end());
        const std::boyer_moore_horspool_searcher searcher(bp.begin(), bp.end());

        std::for_each(std::execution::par_unseq, chrs.begin(), chrs.end(), [&searcher, &bp, &count](const auto& chromosome) {
            for(auto cb = chromosome.begin(); (cb = std::search(cb, chromosome.end(), searcher)) != chromosome.end();
                cb += bp.size()) {
                ++count;
            };
        });
#endif

        bpline.emplace_back(std::to_string(count));
        std::lock_guard lock(mtx_result);
        result << bpline;
    });

    return 0;
}
