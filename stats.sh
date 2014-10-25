
#!/bin/bash

	
id=1

declare -a aopt=("-O1" "-O3")
declare -a avec=("-no-vec" "-vec -vec-report3" "-xhost -vec-report3")
declare -a ainput=("cojack.dat" "tjunc.dat")
#declare -a adefs=("-DMISSES" "")
declare -a adefs=("")


## now loop through the above array
for input in "${ainput[@]}"
do
for vec in "${avec[@]}"
do
for opt in "${aopt[@]}"
do
for defs in "${adefs[@]}"
do


	echo "measurements $id ****************************************************************************************"
	echo "measurements $id: $opt $vec $defs $input"
	echo "measurements $id ****************************************************************************************"

	make clean

	echo "!call make EXTRAFLAGS=$opt $vec $defs"
	make EXTRAFLAGS="$opt $vec $defs" OUT=gccg_$id

#	msg="$id: $opt $vec $defs $input\;L2 Miss Rate\; L3 Miss Rate\;"
msg="$id: $opt $vec $defs $input\;Execution Time\; MFlops\;"
	echo $msg
	sed -e "s;%ID%;$id;g" -e "s;%MSG%;$msg;g" -e "s;%INPUT%;$input;g" job.template > genjob$id.cmd

	llsubmit genjob$id.cmd

id=$((id+1))



done
done
done
done





	
