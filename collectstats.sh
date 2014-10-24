for each in out.genjob*; do cat $each; echo "  "; done | cut -d";" -f1,2,3,4
