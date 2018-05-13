#!/usr/bin/env bash

echo "**********************Test Begin**********************"

function check()
{
	local report
	report="[HINT] Scan file: "
	report=${report}${1}

	echo -e "\e[32m"${report}"\e[0m"
	echo ${report} >> "../tokenOut.txt"

	./thread "../$1"

	echo -e "\n" >> "../tokenOut.txt"
	let total=total+1
}

function finish()
{
	echo "[HINT] Finish Check, Reseult saved to tokenOut.txt"

	echo "**********************Test End***********************"
	cd ..
	rm build -rf
}

function init()
{
	if [ -f "./tokenOut.txt" ]; then
		rm ./tokenOut.txt
	fi

	if [ ! -d "./build" ]; then
		mkdir ./build
		cd ./build
		cmake ..
		make 
		cd ..
	fi

	if [ ! -x "./build/thread" ]; then
		cd ./build
		cmake ..
		make
		cd ..
	fi

	cd ./build
}


# Main Process 

init;

# Test Code Put here

check "test/test1.txt" ;
check "test/test2.txt" ;
check "test/test3.txt" ;

# Test Code End

finish;

exit

