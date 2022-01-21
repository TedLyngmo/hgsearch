# hgsearch
Tools for searching for sequences in genome files

Some are made in scripting languages like `bash`, `awk` and `perl` and others in C++.

### `hgsearch` and `hgsearchmm`

These are two tools written in C++20 and they need support for execution policies (via Intel TBB or similar).

* `hgsearch` will read the full `hg19.fa` file into memory before the searching starts.
* `hgsearchmm` creates a memory map over the file before the searching starts. For this tool, you also need `boost` (the `boost_iostreams` library ) for memory mapping of the genome file.

### Recommended tools

`pbzip2` for parallel BZIP2 compression/decompression of big genome files etc.
