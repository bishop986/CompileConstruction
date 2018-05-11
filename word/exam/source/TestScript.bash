#!/usr/bin/env bash

let pass=0
let error=0
let total=0

echo "**********************Test Begin**********************"

function check()
{
	echo $1 2>> "../output.txt" 1>> "output.txt"
	./thread "../$1" 2>> "../output.txt" 1>> "../output.txt"

	if [ $? -eq ${2} ]; then
		local report
		report="[Pass] success with: "
		report=${report}${1}
		echo -e "\e[32m"${report}"\e[0m"
		echo ${report} >> "../output.txt"
		let pass=pass+1
	else
		local report 
		report="[Error] fail with: "
		report=${report}${1}
		echo -e "\e[31m"${report}"\e[0m"
		echo ${report} >> "../output.txt"
		let error=error+1
	fi

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
	if [ -f "./output.txt" ]; then
		rm ./output.txt
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

check "test/test1.txt" 0;
check "test/test2.txt" 0;
check "test/test3.txt" 0;

# Test Code End

finish;

exit

