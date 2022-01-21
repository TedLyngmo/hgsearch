// A memory mapping hgsearch version in case the hg19 file is too big to fit in memory

#include "main.hpp"
#include "runner.hpp"
#include "tsv.hpp"

#include <boost/iostreams/device/mapped_file.hpp>

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

// a function to return an index to all the lines in a memory mapped file
std::vector<std::string_view> make_line_index(boost::iostreams::mapped_file_source& mfs) {
    std::vector<std::string_view> lines;
    auto first = mfs.data();
    auto end = mfs.data() + mfs.size();
    while(first < end) {
        auto last = first;
        // find line ending
        for(; last != end && *last != '\n'; ++last) {
        }
        // store a view over this line
        lines.emplace_back(first, last);
        first = last + 1;
    }
    return lines;
}

int cppmain(const std::string_view& program, std::vector<std::string_view> args) {
    if(args.size() != 3) {
        std::cerr << "USAGE: " << program
                  << " tsv-bed-file hg19-fasta-file tsv-output-file(or '-' for "
                     "stdout)\n";
        return 1;
    }
    // "tsv_file_KLM117.bed"
    std::ifstream tsvfile(args[0].data());
    if(!tsvfile) return error(program, args[0]);

    // "hg19.fa"
    boost::iostreams::mapped_file_source mfs(args[1].data());
    if(!mfs.is_open()) return error(program, args[1]);

    // where to save the output?
    std::ofstream out;
    std::ostream& res = args[2] == "-" ? std::cout : (out.open(args[2].data()), out);
    if(!res) return error(program, args[2]);

    std::cerr << program << ": Indexing " << args[1] << " ..." << std::flush;
    auto hgindex = make_line_index(mfs);
    std::cerr << " indexed " << hgindex.size() << " lines.\n";

    tsv_record::max_in_cols = 7;

    return run(program, args[0], tsvfile, hgindex, res);
}
