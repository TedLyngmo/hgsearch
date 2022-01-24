#!bin/bash 

#sorting by chromosome (1st column of a bed file) 
sort -t$'\t' -k1 -n yourfile.bed
#sorting by chromosome (1st column of a bed file) 
#make sure to change "x" to whichever column your frequency count may be in, as number of columns between BED files may differ depending on the user. 
 
sort -t$'\t' -kx -nr yourfile.bed 