#!/bin/bash

bedfile="$1"
hgfile="$2"

exec awk '
    v==1 {a[$7];next}            # Get the pattern from first file into the array a
    v==2 {                       # For each line of the second file
        for(i in a){             # Loop though all patterns
            a[i]=split($0,b,i)-1 # Get the number of pattern match in the line
        }
    }
    v==3 {print $0,a[$7]}        # Re-read first file to add the number of pattern matches
' v=1 "$bedfile" v=2 "$hgfile" v=3 "$bedfile"
