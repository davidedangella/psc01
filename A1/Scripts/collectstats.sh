for each in out.merged.genjob*; do cat $each; echo "  "; done | cut -d";" -f1,2,3,6,7,8
