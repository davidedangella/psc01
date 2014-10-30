
#!/bin/bash

	
id=1

declare -a aopt=("-O1" "-O3")
declare -a avec=("-no-vec" "" "-xhost")
declare -a ainput=("text cojack.dat" "text tjunc.dat" "bin cojack.bin" "bin tjunc.bin")
#declare -a adefs=("-DMISSES" "")
#declare -a adefs=("")


## now loop through the above array

for input in "${ainput[@]}"
do
for vec in "${avec[@]}"
do
for opt in "${aopt[@]}"
do

#for defs in "${adefs[@]}"
#do


	echo "measurements $id ****************************************************************************************"
	echo "measurements $id: $opt $vec $input"
	echo "measurements $id ****************************************************************************************"

	make clean

	echo "!call make EXTRAFLAGS=$opt $vec $defs"
	make EXTRAFLAGS="$opt $vec $defs" OUT=gccg_$id

#	msg="$id: $opt $vec $defs $input\;L2 Miss Rate\; L3 Miss Rate\;"
	msg="$id: $opt $vec $defs $input\;Execution Time\; MFlops\;Reading Time\;"
	echo $msg
	sed -e "s;%ID%;$id;g" -e "s;%MSG%;$msg;g" -e "s;%INPUT%;$input;g" job.template > genjob$id.cmd

	llsubmit genjob$id.cmd
id_old=$id
id=$((id+1))

	make clean

	echo "!call make EXTRAFLAGS=$opt $vec $defs -DMISSES"
	make EXTRAFLAGS="$opt $vec $defs -DMISSES" OUT=gccg_$id

	msg="id: $opt $vec $defs $input\;L2 Miss Rate\; L3 Miss Rate\; Reading Time\;"
#	msg="$id: $opt $vec $defs $input\;Execution Time\; MFlops\;"
	echo $msg
	sed -e "s;%ID%;$id;g" -e "s;%MSG%;$msg;g" -e "s;%INPUT%;$input;g" -e "s;%JOINF%;out.genjob$id_old;g" job.template.merge > genjob$id.cmd

	llsubmit genjob$id.cmd


id=$((id+1))



#done
done
done
done





	

