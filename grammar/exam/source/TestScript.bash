#!/usr/bin/env bash

echo "**********************Test Begin**********************"

function check()
{
	local report
	report="[HINT] Scan file: "
	report=${report}${1}

	echo -e "\e[32m"${report}"\e[0m"

	./thread "../$1"

}

function finish()
{
	echo "[HINT] Finish Check, Reseult saved to tokenOut.txt and SyntaxOut.txt"

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

	if [ -f "./SyntaxOut.txt" ]; then
		rm ./SyntaxOut.txt
	fi

	cd ./build
}


# Main Process 

init;

# Test Code Put here

check "TestFile/test1.txt" ;
check "TestFile/test2.txt" ;
check "TestFile/test3.txt" ;

# Test Code End

finish;

exit

