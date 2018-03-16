#!/bin/bash
#  running simulators

if [ $1 -eq -1 ]
then
	echo "Test changing Nodenum(8 10 12 14)" 
	Nodenum=(8 10 12 14)
	for n in ${Nodenum[@]}
	do
	echo "Node:"$n "Degraded:"$2 "trace:"$3 "Placement:"$4
	#time ./1.sh $j 1 $p $1 $i $d	
	done 
	exit
else
	if [ $# -lt 4 ]
	then 
		echo "Usage: run.sh [node_num:12] [degraded 0|1|all:2] [trace:u1|h2|t3|all:4] [placement: BA0|RA1|all:2]"
		exit
	fi
fi

echo "Node:"$1 "Degraded:"$2 "trace:"$3 "Placement:"$4

if [ $3 -eq 4 ]
then
	trace=(1 2 3) 
else
	trace=$3	
fi

if [ $2 -eq 2 ]
then
	degraded=(0 1) 
else
	degraded=$2	
fi


if [ $4 -eq 2 ]
then
	placement=(0 1) 
else
	placement=$4	
fi

for p in ${placement[@]}
do
	echo 
	for d in ${degraded[@]}
	do
		for i in ${trace[@]}
		do		
			if [ $i -eq 1 ]
			then
				for j in {4..5}
				do
				echo "CAFTL[desktop-ubuntu]-"$j "Nodes:"$1 "Degraded:"$d "Placement:"$p
				time ./1.sh $j 1 $p $1 $i $d
				done
				echo
			elif [ $i -eq 2 ]
			then
				for j in 2 3 6 {8..18} 20 21
				do
					echo "CAFTL[hivetpch]-"$j "Nodes:"$1 "Degraded:"$d "Placement:"$p
					time ./1.sh $j 1 $p $1 $i $d
				done
				echo
			elif [ $i -eq 3 ]
			then
				for j in {1..2}
				do
					echo "CAFTL[tpcc]-"$j "Nodes:"$1 "Degraded:"$d "Placement:"$p
					time ./1.sh $j 1 $p $1 $i $d
				done
				echo
			fi
		done
	done
done
 