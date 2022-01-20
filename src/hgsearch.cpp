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

class tsv_record {
public:
    const std::string& operator[](size_t idx) { return data[idx]; }
    size_t size() const { return data.size(); }

    template<class... Args>
    decltype(auto) emplace_back(Args&&... args) {
        return data.emplace_back(std::forward<Args>(args)...);
    };

    inline static unsigned max_in_cols = 0; // 0 read all columns

    friend std::istream& operator>>(std::istream& is, tsv_record& r);
    friend std::ostream& operator<<(std::ostream& os, const tsv_record& r);

private:
    std::vector<std::string> data;
};

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

int run(const std::string_view& program, const std::string_view& tsvname, std::istream& tsvfile,
        const std::vector<std::string_view>& hgindex, std::ostream& result) {
    int error = 0;
    tsv_record tsvr;

    for(std::uintmax_t line = 1; tsvfile >> tsvr; ++line) {
        if(tsvr.size() < 7) {
            std::cerr << program << ": " << tsvname << ": Error on line " << line << '\n';
            error = 1;
        } else {
            // count hits
            std::atomic<std::uint32_t> count = 0;

            std::for_each(std::execution::par_unseq, hgindex.begin(), hgindex.end(),
                          [&sub = tsvr[6], &count](const std::string_view& line) {
                              if(line.find(sub) != std::string_view::npos) ++count;
                          });

            tsvr.emplace_back(std::to_string(count));
            result << tsvr << std::flush;
        }
    }

    return error;
}

int error(const std::string_view& program, const std::string_view& file) {
    std::cerr << program << ": " << file << ": " << std::strerror(errno) << '\n';
    return 1;
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

int main(int argc, char* argv[]) {
    return cppmain(argv[0], {argv + 1, argv + argc});
}
