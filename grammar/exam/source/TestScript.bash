#!/usr/bin/env bash

let pass=0
let error=0
let total=0

echo "**********************Test Begin**********************"

function check()
{
	local report
	report="[HINT] Scan file: "
	report=${report}${1}

	echo -e "\e[32m"${report}"\e[0m"
	echo ${report} >> "../tokenOut.txt"
	let pass=pass+1

	echo $1 2>> "../tokenOut.txt" 1>> "tokenOut.txt"
	./thread "../$1" 2>> "../tokenOut.txt" 1>> "../tokenOut.txt"

	echo -e "\n" >> "../tokenOut.txt"
	let total=total+1
}

function finish()
{
	echo "[HINT] Finish Check, Reseult saved to output.txt"
	echo -e "\e[33m**********************Summary************************\e[0m"
	echo "[SUM] Total:"${total}" test case"
	echo "[SUM] Pass:"${pass}" test case"
	echo "[SUM] Error:"${error}" test case"

	if [ ${error} -eq 0 ]; then
		echo -e "\e[32m[Success] All Clear\e[0m"
	else
		echo -e "\e[31m[Fail] Problem Happened\e[0m"
	fi

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

check "TestFile/mytest1.txt" ;
check "TestFile/test1.txt" ;
check "TestFile/test2.txt" ;
check "TestFile/test3.txt" ;

# Test Code End

finish;

exit

