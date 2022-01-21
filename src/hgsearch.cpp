#include "main.hpp"
#include "runner.hpp"
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
    std::ifstream hgfile(args[1].data());
    if(!hgfile) return error(program, args[1]);

    std::cerr << "Reading genome data..." << std::flush;
    std::vector<std::string> chrs(std::istream_iterator<std::string>(hgfile), std::istream_iterator<std::string>{});
    std::cerr << " read " << chrs.size() << " lines.\n";

    // where to save the output?
    std::ofstream out;
    std::ostream& res = args[2] == "-" ? std::cout : (out.open(args[2].data()), out);
    if(!res) return error(program, args[2]);

    tsv_record::max_in_cols = 7;

    return run(program, args[0], tsvfile, chrs, res);
}
