#!/usr/bin/env zsh

let pass=0
let error=0
let total=0

echo "**********************Test Begin**********************"

function check()
{
	local str=$1
	echo "$str" > ../TestTmpFile
	./LessonProject ../TestTmpFile 2>> "../report.log" 1>> "../report.log"

	if [ $? -eq ${2} ]; then
		local report
		report="[Pass] success with: "
		report=${report}${str}
		echo "\e[32m"${report}"\e[0m"
		echo ${report} >> "../report.log"
		let pass=pass+1
	else
		local report 
		report="[Error] fail with: "
		report=${report}${str}
		echo "\e[31m"${report}"\e[0m"
		echo ${report} >> "../report.log"
		let error=error+1
	fi

	let total=total+1
}

function finish()
{
	echo "[HINT] Finish Check, Reseult saved to report.log"
	echo "\e[33m**********************Summary************************\e[0m"
	echo "[SUM] Total:"${total}" test case"
	echo "[SUM] Pass:"${pass}" test case"
	echo "[SUM] Error:"${error}" test case"

	if [ ${error} -eq 0 ]; then
		echo "\e[32m[Success] All Clear\e[0m"
	else
		echo "\e[31m[Fail] Problem Happened\e[0m"
	fi

	echo "**********************Test End***********************"
	cd ..
	rm build -rf
	rm TestTmpFile
}

function init()
{

	if [ ! -d "./build" ]; then
		mkdir ./build
		cd ./build
		cmake -DCMAKE_BUILD_TYPE=Debug ..
		make 
		cd ..
	fi

	if [ ! -x "./build/LessonProject" ]; then
		cd ./build
		cmake -DCMAKE_BUILD_TYPE=Debug ..
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

# Test Code End

finish;

exit

