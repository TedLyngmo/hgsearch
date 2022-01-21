###### Benchmarks

This table is a comparison of the different solutions, with timings to finish different amount of tsv/bed records with the full `hg19.fa` file (excluding the records containing only `N`:s). `hg19.fa` contains 57'946'726 such records.

| search \ beds |     1     |      2     |    100    |    1000    |
|---------------|----------:|-----------:|----------:|-----------:|
| awk           | 1m0.606s  | 17m19.899s |     -     |      -     |
| perl          |  13.263s  |    15.618s | 4m48.751s | 48m27.267s |
| bash/grep     |   2.088s  |     3.670s | 3m27.378s | 34m41.129s |
| hgsearch      |   8.776s  |     9.425s |   15.618s |  3m56.508s |
| hgsearchmm    |   1.942s  |     2.146s |   21.715s |            |
