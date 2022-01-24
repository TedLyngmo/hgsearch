###### Benchmarks

This table is a comparison of the different solutions, with timings to finish
different amount of tsv-bed records with the full `hg19.fa` file (with
consecutive `N`:s replaced by `N+`). `hg19.fa` contained 93 chromosomes at the
time these timings were performed.

| search \ beds |     1     |      2     |    100    |    1000    |   10000   |  417943  |
|---------------|----------:|-----------:|----------:|-----------:|----------:|---------:|
| awk           | 1m0.606s  | 17m19.899s |     -     |      -     |     -     |          |
| perl          |  13.263s  |    15.618s | 4m48.751s |   48m27s   |     -     |          |
| bash/grep     |   2.088s  |     3.670s | 3m27.378s |   34m41s   |     -     |          |
| hgsearch      |   3.738s  |     3.727s |    9.148s |      59s   |   9m24s   |          |
| hgsearchmm    |   1.220s  |     1.241s |    6.679s |      56s   |   9m18s   | 6h27m21s |
