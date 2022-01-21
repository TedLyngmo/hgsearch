# hgsearch
Tools for searching for sequences in genome files. in particular, these tools are extracting FASTA sequences (strings of DNA) and how frequently each string of DNA appears in the human genome (in particular, the human reference genome hg19/GRCh37).

Some resources on both file formats for non-bioinformaticians: 
- FASTA files: https://zhanggroup.org/FASTA/
- BED files: https://genome.ucsc.edu/FAQ/FAQformat.html#format1


Some are made in scripting languages like `bash`, `awk` and `perl` and others in C++.

### `hgsearch` and `hgsearchmm`

These are two tools written in C++20 and they need support for execution policies (via Intel TBB or similar).

* `hgsearch` will read the full `hg19.fa` file into memory before the searching starts.
* `hgsearchmm` creates a memory map over the file before the searching starts. For this tool, you also need `boost` (the `boost_iostreams` library ) for memory mapping of the genome file.

### Recommended tools

`pbzip2` for parallel BZIP2 compression/decompression of big genome files etc.
