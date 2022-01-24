#!/usr/bin/perl
use warnings;
use strict;

#When running this script from command line, use "perl this_script.pl tsv.bed hg19.fasta > new-tsv.bed"

my ($bed_file, $fasta_file) = @ARGV;
open my $bed,   '<', $bed_file   or die "$bed_file: $!";
open my $fasta, '<', $fasta_file or die "$fasta_file: $!";


my $seq;
while (<$fasta>) {
    $seq .= "\n", next if /^>/;
    chomp;
    $seq .= $_;
}

while (<$bed>) {
    chomp;
    my $short_seq = (split /\s+/, $_)[-1];
    my $count = () = $seq =~ /\Q$short_seq\E/g;
    print "$_\t$count\n";
}
