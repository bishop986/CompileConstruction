#!/usr/bin/env bash

let pass=0
let error=0
let total=0

echo "**********************Test Begin**********************"

function check()
{
	local str=$1
	echo "$str" > ../input.txt
	./LessonProject ../input.txt 2>> "../output.txt" 1>> "../output.txt"

	if [ $? -eq ${2} ]; then
		local report
		report="[Pass] success with: "
		report=${report}${str}
		echo -e "\e[32m"${report}"\e[0m"
		echo ${report} >> "../output.txt"
		let pass=pass+1
	else
		local report 
		report="[Error] fail with: "
		report=${report}${str}
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
	rm input.txt
}

function init()
{
	if [ -f "./input.txt" ]; then
		rm ./input.txt
	fi

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

	if [ ! -x "./build/LessonProject" ]; then
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

check "(+ 1 1)" 0;
check "(* 1 1)" 0;
check "(- 1 1)" 0;

check "( 1 1)" 1;
check "+ 1 1)" 1;
check "(+ 1 1" 1;

check "(+ 1 (+ 1 1))" 0;
check "(+ 1 (* 1 1))" 0;
check "(+ 1 (- 1 1))" 0;

check "(+ 1 ( 1 1))" 1;
check "( 1 (+ 1 1))" 1;
check "(+ 1 + 1 1))" 1;
check "(+ 1 (+ 1 1)" 1;

check "(+ (+ 1 1) 1)" 0;
check "(+ (* 1 1) 1)" 0;
check "(+ (- 1 1) 1)" 0;

check "(+ (+ 1 1) (+ 1 1))" 0;
check "(+ (+ 1 1) (* 1 1))" 0;
check "(+ (+ 1 1) (- 1 1))" 0;

check "(+ (+ 1 1) (+ (* 1 23) 1))" 0;
check "(+ (+ 1 1) (* 1 (+ 32 1)))" 0;
check "(+ (+ 1 1) (- (+ 2 3) (+ 29 3)))" 0;

# Test Code End

finish;

exit

