#!/bin/bash
#################################################
#
# Script for running the RTEMS testsuite
#
# Author: André Filipe Pereira Rocha
#         Stefan Hepp <stefan@stefant.org>
#
#################################################

cdlevel=0
partest=
sim=
simargs=()
bsp=
sourcedir=$(pwd)/testsuites
builddir=$(pwd)/../rtems-build


# defaults are set after options are read
resultsdir=
log=

tests=(itrontests libtests mptests psxtests samples sptests tmitrontests tmtests)
runtests=()
failtests=0
successtests=0
noexectests=0
noresulttests=0
resumeflag=0
resumetests=()
genflag=0

function usage() {
 cat <<EOT
 Usage: $0 [-h] [-c] [-r] [-g] [-p <sim args>] [-t <tests>] [-s <source dir>] [-b <build dir>] [-o <report dir>] [-l <log file>] [-m <simulator>] [-x <bsp>]
 
 -h				Display help contents
 -c				Clean files created during testsuite runs
 -r				Resume test execution
 -g				Regenerate .scn files
 -p <sim args>			Pass additional arguments to the simulator. Arguments should be passed between quotation marks
 -t <tests>			Tests to be executed (itrontests, libtests, mptests, psxtests, samples, sptests, tmitrontests, tmtests)
 -s <source dir>                Directory containing the testsuites sources
 -b <build dir>                 Build directory of the testsuites
 -o <report dir>                Report output directory
 -l <log file>			Override the default log file
 -m <simulator>			Override the default simulator (pasim)
 -x <bsp>			Override the default BSP (pasim)
 
EOT
}

function containsElement() {
 local e 
 for e in "${@:2}"; do [[ "$e" == "$1" ]] && return 1; done
 return 0
}

function cleanFiles() {
	rm -rf $resultsdir
}

function writeFile() {
	if [[ ! -f $1 ]]; then
		touch $1
	fi
	echo $2 >> $1
}

function addSimArg() {
	containsElement $1 "${simargs[@]}"		
	if [[ $? == 0 ]]; then 
		simargs=( "${simargs[@]}" $1 )
	fi
}

function checkDefaults() {
	if [[ "$resultsdir" == "" ]]; then
		resultsdir=$sourcedir/results
	fi
	if [[ "$log" == "" ]]; then
		log=$resultsdir/testsuite-log.txt
	else
		log=$resultsdir/$(basename $log)
	fi
	if [[ "$sim" == "" ]];then
		sim="pasim"
		addSimArg "--interrupt=1"
		addSimArg "--freq=5"
	fi
	if [[ "$bsp" == "" ]];then
		bsp="pasim"
	fi
}

function testsOnResume() {
	if [[ -f $1 ]]; then
		while read line
		do
			resumetests=( "${resumetests[@]}" $(echo "${line%%:*}"))
			local testresult=$(echo "${line##*:}")
			if [[ $testresult =~ "Failed" ]]; then				
				let failtests+=1
			elif [[ $testresult =~ "Passed" ]]; then
				let successtests+=1
			elif [[ $testresult =~ "check" ]]; then
				let noresulttests+=1
			elif [[ $testresult =~ "file not found" ]]; then
				let noexectests+=1
			fi
		done < $1
	fi
}

function runTest() {		
	local bin=$(find $testsuitedir -iname "$1.exe")	
	if [[ $bin ]]; then
		local retcode
		case $sim in 
		pasim|patex)		
			$sim "${simargs[@]}" -O $resultsdir/$1-tmp.txt $bin > $resultsdir/$1-stats.txt 2>&1
			retcode=$?
		;;
		tsim-leon3)
			echo -e "load $bin\ngo\nquit\n" >> tsim-script.txt
			$sim "${simargs[@]}" -c tsim-script.txt > $resultsdir/$1-tmp.txt
			retcode=$?
			sed -i '1,24d;$d' $resultsdir/$1-tmp.txt			
			rm -rf tsim-script.txt
		;;
		esac
		sed -i "s/\r//" $resultsdir/$1-tmp.txt
		if [[ $genflag == 1 ]]; then
			cp -f $resultsdir/$1-tmp.txt $1.scn
			writeFile $log "$3: generating file $1.scn"
			echo "$3: generating file $1.scn"
			rm -rf $resultsdir/$1-tmp.txt $resultsdir/$1-stats.txt
		else			
			if [[ $retcode != 0 ]]; then
				writeFile $log "$3: Test executed: Failed with return code $retcode!"
				echo "$(tput setaf 1)$3: Test executed: Failed with return code $retcode!$(tput setaf 7)"
				let failtests+=1
			elif [[ $(find -maxdepth 1 -iname "*.scn" ) ]]; then
				# TODO for some reasons, the pasim output has two empty lines at the beginning and windows newlines,
				#      should be fixed (?), for now we just ignore this
				diff --ignore-blank-lines $resultsdir/$1-tmp.txt $1.scn > $resultsdir/$1-log.txt
				if [[ -s $resultsdir/$1-log.txt ]]; then
					writeFile $log "$3: Test executed: Failed!"
					cp -f $resultsdir/$1-tmp.txt $resultsdir/$1-out.txt
					echo "$(tput setaf 1)$3: Test executed: Failed!$(tput setaf 7)"
					let failtests+=1
				else
					writeFile $log "$3: Test executed: Passed!"
					echo "$(tput setaf 2)$3: Test executed: Passed!$(tput setaf 7)"
					rm -rf $resultsdir/$1-log.txt
					let successtests+=1
				fi
			else
				writeFile $resultsdir/$2-log.txt "##### $3 #####"
				cat $resultsdir/$1-tmp.txt >> $resultsdir/$2-log.txt
				writeFile $resultsdir/$2-log.txt "##### $3 #####"
				writeFile $log "$3: Test executed: check $2-log.txt"
				echo "$3: Test executed: check $2-log.txt"	
				let noresulttests+=1
			fi
			rm -rf $resultsdir/$1-tmp.txt
		fi
	elif [[ $(find -maxdepth 1 -iname "*.scn" ) && $genflag == 0 ]]; then
			writeFile $log "$3: Test not executed: $1.exe file not found"
			echo "$(tput setaf 3)$3: Test not executed: $1.exe file not found$(tput setaf 7)"
			let noexectests+=1
	fi
}

function recurseDirs
{
	for f in "$@"
	do
		local testflag=1
		if [[ $cdlevel == 0 ]]; then	
			testdir="$f"
			containsElement "$f" "${runtests[@]}"
			if [[ $? == 0 && ${#runtests[@]} -gt 0 ]]; then
				testflag=0
			fi		
		fi
		if [[ -d "$f" && $testflag == 1 ]]; then
			cd "$f"
			let cdlevel+=1
			if [[ $cdlevel -gt 1 ]]; then
				testdir="$testdir/$f"
			fi			
			containsElement "$testdir" "${resumetests[@]}"
			if [[ $? == 0 || ${#resumetests[@]} == 0 ]]; then				
				runTest "$f" $(echo "${testdir%%/*}") "$testdir"				
			fi								
			recurseDirs $(ls -1)			
			if [[ $cdlevel != 1 ]]; then				
				testdir=$(echo "${testdir%/*}")
			fi			
			cd ..
			let cdlevel-=1
		fi
	done
}

while getopts ":hHp:P:t:T:l:L:cCrRb:B:s:S:o:O:m:M:x:X:gG" opt; do	
	case "$opt" in	
	h|H) 
		usage		
		exit 1
	;;
	p|P) 		
		addSimArg "$OPTARG"
	;;
	t|T)
		containsElement "$OPTARG" "${tests[@]}"		
		if [[ $? == 1 ]]; then
			runtests=( "${runtests[@]}" "$OPTARG" )
		else
			echo "Invalid test: $OPTARG"
			usage
			exit 1
		fi
	;;
	l|L)
		log="$OPTARG"
	;;
	c|C)
		checkDefaults
		cleanFiles
		exit 1
	;;
	r|R)
		resumeflag=1
		checkDefaults
		testsOnResume $log		
	;;
	s|S)
		sourcedir="$OPTARG"
	;;
	b|B)
		builddir="$OPTARG"
		builddir=${builddir/%"/"/}		
	;;
	o|O)
		resultsdir="$OPTARG"
	;;
	m|M)
		sim="$OPTARG"
		simargs=()
	;;
	x|X)
		bsp="$OPTARG"
	;;
	g|G)
		genflag=1
	;;
	\?) 		
		echo "Invalid option: -$OPTARG"
		usage
		exit 1
	;;
	:)
		echo "Option -$OPTARG requires an argument"
		usage
		exit 1
	;;
	esac	
done

checkDefaults
testsuitedir=$builddir/patmos-unknown-rtems/c/$bsp/testsuites
if [[ ! -d $sourcedir ]]; then
	echo "Invalid source dir. Go to RTEMS source dir or specify the testsuites source dir with -s."
	exit 1
fi
if [[ ! -d $testsuitedir ]]; then
	echo "Invalid testsuite dir. Check if the specified BSP is correct."
	exit 1
fi
cd $sourcedir

if [[ $resumeflag == 0 ]]; then
	rm -rf $log	
fi
if [[ ! -d $resultsdir ]]; then
	mkdir $resultsdir
fi

recurseDirs $(ls -1)

if [[ $genflag == 0 ]]; then
	let totaltests=$successtests+$failtests+$noexectests+$noresulttests
	successper=0
	failper=0
	noexecper=0
	noresultper=0
	if [[ $totaltests != 0 ]]; then
		let successper=$successtests*100/$totaltests
		let failper=$failtests*100/$totaltests
		let noexecper=$noexectests*100/$totaltests
		let noresultper=$noresulttests*100/$totaltests
	fi
	writeFile $log ""
	writeFile $log "---------- Results ----------"
	writeFile $log "Successful tests: $successtests ($successper%)"
	writeFile $log "Failed tests: $failtests ($failper%)"
	writeFile $log "Not executed tests: $noexectests ($noexecper%)"
	writeFile $log "No result tests: $noresulttests ($noresultper%)"
fi