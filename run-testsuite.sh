#!/bin/bash
#################################################
#
# Script for running the RTEMS testsuite
#
# Author: André Rocha <afpr@gmv.com>
#         Stefan Hepp <stefan@stefant.org>
#
#################################################

# files and dirs
curdir=$(pwd)
sourcedir=$curdir/testsuites
builddir=$curdir/../rtems-build
patmosdir=~/t-crest/build-rtems/patmos
ise_ds_path="/opt/Xilinx/14.7/ISE_DS"
comport=/dev/ttyUSB1
makefile=

# script flow helpers
cdlevel=0
javatoolsflag=0
xilinxfpgaflag=0
toolsflag=1
valuesflag=1
bitflag=0
resumeflag=0
genflag=0
cleanflag=0
partest=
sim=
simargs=()
bsp=

# script lists
tests=(itrontests libtests mptests psxtests samples sptests tmitrontests tmtests)
simulators=(ML605 pasim patex tsim-leon3)
runtests=()
resumetests=()

# test counters
failtests=0
successtests=0
noexectests=0
noresulttests=0

# defaults are set after options are read
bitfile=
biturl=
resultsdir=
log=
# timeout 360s = 6 mins
timeout=360

# string helpers
checklog=" [check log for more information]"
logseparator=">--------------------------------------------------------------------------<\n"

function usage() {
 cat <<EOT
 Usage: $0 [-h] [-c] [-r] [-g] [-j] [-f] [-p <sim args>] [-t <tests>] [-s <source dir>] [-b <build dir>] [-i <patmos dir>] [-o <report dir>] [-l <log file>] [-m <simulator>] [-x <bsp>] [-u <com port>] [-d <bit file>] [-e <bit file>] [-v <path Xilinx>] [-z <timeout>]
 
 -h				Display help contents
 -c				Clean files created during testsuite runs
 -r				Resume test execution
 -g				Regenerate .scn files
 -j				Build javatools
 -f				Configure Xilinx ML605 FPGA 
 -p <sim args>			Pass additional arguments to the simulator. Arguments should be passed between quotation marks
 -t <tests>			Tests to be executed (itrontests, libtests, mptests, psxtests, samples, sptests, tmitrontests, tmtests)
 -s <source dir>                Directory containing the testsuites sources
 -b <build dir>                 Build directory of the testsuites
 -i <patmos dir>                Directory containing the patmos repository
 -o <report dir>                Report output directory
 -l <log file>			Override the default log file
 -m <simulator>			Override the default simulator (ML605), available simulators are: ML605, pasim, patex, tsim-leon3
 -x <bsp>			Override the default BSP (tcrest)
 -u <com port>			Specify the communication port
 -d <bit file>			Download Xilinx bitfile
 -e <bit file>			Path to Xilinx bitfile
 -v <path Xilinx>		Path to Xilinx ISE DS folder
 -z <timeout>			Timeout, in seconds, for downloading elf files to FPGA (360s)
 
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
	echo -e $2 >> $1
}

function addSimArg() {
	containsElement $1 "${simargs[@]}"		
	if [[ $? == 0 ]]; then 
		simargs=( "${simargs[@]}" $1 )
	fi
}

function checkUserValues() {
	if [[ $valuesflag == 0 ]]; then
		exit 1
	fi
}

# function arguments:
#	1 - makefile target
function useMakefile() {
	curdir=$(pwd)
	cd $(dirname $makefile)
	make COM_PORT=$comport $1 >> $log 2>&1
	cd $curdir
}

# function arguments:
#	1 - comment on configuration
function configureXilinxFPGA() {
	local tmp_script=tmp-impact.sh
	rm -rf $tmp_script
	touch $tmp_script
	echo -e "source $ise_ds_path/settings32.sh\nexport LD_PRELOAD=/opt/libusb/libusb-driver.so\nimpact -batch <<EOF\ncleancablelock\nsetMode -bs\nsetCable -port auto\nIdentify -inferir\nidentifyMPM\nassignfile -p 2 -file $bitfile\nprogram -p 2\nexit\nEOF" >> $tmp_script
	chmod 777 $tmp_script	
	echo "Configuring ML605...$1$checklog"
	writeFile $log $logseparator
	./$tmp_script >> $log 2>&1
	writeFile $log $logseparator
	rm -rf tmp-impact.sh
	rm -rf _impactbatch.log
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
		local tmplog=$resultsdir/$1-tmp.txt		
		local exelog=$resultsdir/$1-exe.txt
		local testlog=$resultsdir/$1-log.txt
		case $sim in 
		pasim|patex)		
			$sim "${simargs[@]}" -O $tmplog $bin > $exelog 2>&1
			retcode=$?
		;;
		tsim-leon3)
			echo -e "load $bin\ngo\nquit\n" >> tsim-script.txt
			$sim "${simargs[@]}" -c tsim-script.txt > $exelog
			retcode=$?
			sed -e '1,24d;$d' $exelog > $tmplog			
			rm -rf tsim-script.txt		
		;;
		ML605)
			local patserdow=$(find $patmosdir -iname "patserdow" -type f -executable | head -1)
			if [[ $patserdow == "" ]]; then
				"Error: patserdow java script not found"
				exit 1
			fi			
			timeout --foreground $timeout"s" $patserdow -v $comport $bin > $exelog 2>&1
			retcode=$?
			sed -i "/\[..........\]/d" $exelog			
			local tmp=$resultsdir/$1-tmp-tmp.txt			
			sed -ne '/\<Entry point:\(.*\)\>/,/\<EXIT 0\>/{/\<Entry point:\(.*\)\>/{h;d};H;/\<EXIT 0\>/{x;p}}' $exelog > $tmplog
			tail -n +3 $tmplog | head -n -2 > $tmp && mv $tmp $tmplog
		;;
		esac
		sed -i "s/\r//" $tmplog
		if [[ $genflag == 1 ]]; then
			cp -f $tmplog $1.scn
			writeFile $log "$3: generating file $1.scn"
			echo "$3: generating file $1.scn"
			rm -rf $tmplog $exelog
		else			
			if [[ $retcode != 0 ]]; then				
				writeFile $log "$3: Test executed: Failed with return code $retcode!"
				echo "$(tput setaf 1)$3: Test executed: Failed with return code $retcode!$(tput setaf 7)"
				let failtests+=1
				
				#timeout, kill processes hanging port and reconfigure FPGA
				if [[ $sim == "ML605" && $retcode == 124 ]]; then
					lsof | grep $comport | awk 'NR!=1 {print $3}' | xargs kill
					configureXilinxFPGA " [reconfiguration due to timeout]"
				fi
			
			else
				if [[ $sim == "pasim" ]]; then					
					mv -f $exelog $resultsdir/$1-stats.txt
				fi
				
				if [[ $(find -maxdepth 1 -iname "*.scn" ) ]]; then
					# TODO for some reasons, the pasim output has two empty lines at the beginning and windows newlines,
					#      should be fixed (?), for now we just ignore this
					diff --ignore-space-change $tmplog $1.scn > $testlog
					if [[ -s $testlog ]]; then
						writeFile $log "$3: Test executed: Failed!"
						if [[ $sim == "ML605" || $sim == "tsim-leon3" ]]; then
							mv -f $exelog $resultsdir/$1-out.txt
						else
							mv -f $tmplog $resultsdir/$1-out.txt
						fi
						echo "$(tput setaf 1)$3: Test executed: Failed!$(tput setaf 7)"
						let failtests+=1
					else
						writeFile $log "$3: Test executed: Passed!"
						echo "$(tput setaf 2)$3: Test executed: Passed!$(tput setaf 7)"					
						let successtests+=1
						rm -rf $testlog
					fi
				else
					local noresultlog=$resultsdir/$2-log.txt
					writeFile $noresultlog "##### $3 #####"
					cat $tmplog >> $noresultlog
					writeFile $noresultlog "##### $3 #####"
					writeFile $log "$3: Test executed: check $2-log.txt"
					echo "$3: Test executed: check $2-log.txt"	
					let noresulttests+=1
				fi
				rm -rf $exelog
			fi
			rm -rf $tmplog
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

while getopts ":hHp:P:t:T:l:L:cCrRb:B:s:S:o:O:m:M:x:X:gGjJfFu:U:e:E:d:D:v:V:i:I:z:Z:" opt; do	
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
		cleanflag=1		
	;;
	r|R)
		resumeflag=1				
	;;
	s|S)
		sourcedir=${OPTARG%/}
	;;
	b|B)		
		builddir=${OPTARG/%"/"/}		
	;;
	o|O)
		resultsdir=${OPTARG%/}
	;;
	m|M)
		containsElement "$OPTARG" "${simulators[@]}"		
		if [[ $? == 1 ]]; then
			sim="$OPTARG"
			simargs=()
		else
			echo "Invalid simulator: $OPTARG"
			usage
			exit 1
		fi		
	;;
	x|X)
		bsp="$OPTARG"
	;;
	g|G)
		genflag=1
	;;
	j|J)
		javatoolsflag=1
	;;
	f|F)
		xilinxfpgaflag=1
	;;
	u|U)
		comport="$OPTARG"
	;;
	e|E)		
		bitfile="$OPTARG"
	;;
	d|D)
		biturl="$OPTARG"
		bitflag=1
	;;
	v|V)
		ise_ds_path=${OPTARG%/}		
	;;
	i|I)
		patmosdir=${OPTARG%/}	
	;;
	z|Z)		
		timeout="$OPTARG"
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

# check user-specified and default values

if [[ ! -d $sourcedir ]]; then
	echo "Error: $sourcedir not found. Go to RTEMS source dir or specify the testsuites source dir with -s."
	valuesflag=0
fi

if [[ ! -d $builddir ]]; then
	echo "Error: $sourcedir not found. Go to RTEMS source dir or specify the testsuites source dir with -s."
	valuesflag=0
fi

checkUserValues

if [[ "$resultsdir" == "" ]]; then
	resultsdir=$sourcedir/results
fi

# check if user chose to clean results folder
if [[ $cleanflag == 1 ]]; then
	cleanFiles
	exit 1
fi

# make results dir if it does not exist
if [[ ! -d $resultsdir ]]; then	
	mkdir -p $resultsdir
fi

# get absolute path of results dir
curdir=$(pwd)
cd $resultsdir
resultsdir=$(pwd)
cd $curdir

# update log file
if [[ "$log" == "" ]]; then
	log=$resultsdir/testsuite-log.txt
else
	log=$resultsdir/$(basename $log)
fi

if [[ "$bsp" == "" ]]; then
	bsp="tcrest"
fi

# get absolute path of results dir
curdir=$(pwd)
cd $builddir
builddir=$(pwd)
cd $curdir

case $bsp in
	leon*|erc32)
	testsuitedir=$builddir/sparc-rtems4.10/c/$bsp/testsuites
	;;
	*)
	testsuitedir=$builddir/patmos-unknown-rtems/c/$bsp/testsuites
	;;
esac

makefile=$patmosdir/Makefile

if [[ "$bitfile" == "" ]]; then
	bitfile=$resultsdir/t-crest.bit		
fi

if [[ "$sim" == "" ]]; then
	sim="ML605"
elif [[ "$sim" == "pasim" ]]; then
	addSimArg "--interrupt=1"
	addSimArg "--freq=5"
	addSimArg "-v"
fi

if [[ $resumeflag == 0 ]]; then
	rm -rf $log
else
	testsOnResume $log
fi

# check if files and directories exist
if [[ ! -d $testsuitedir ]]; then
	echo "Error: $testsuitedir not found. Check if the specified BSP is correct."
	valuesflag=0
fi

if [[ ($sim == "ML605" || $javatoolsflag == 1) && ! -d $patmosdir ]]; then
	echo "Error: $patmosdir not found. Specify the correct patmos dir with -i."
	valuesflag=0
fi

if [[ ($sim == "ML605" || $xilinxfpgaflag == 1) && ! -d $ise_ds_path ]]; then
	echo "Error: $ise_ds_path not found. Specify the correct path to Xilinx ISE DS folder with -v."
	valuesflag=0
fi

if [[ $sim == "ML605" && ! -c $comport ]]; then
	echo "Error: $comport not found. Specify the correct communication port with -u."
	valuesflag=0
fi

if [[ ($sim == "ML605" || $xilinxfpgaflag == 1) && ! -f $bitfile && $bitflag == 0 ]]; then
	echo "Error: $bitfile not found. Specify the correct path to bitfile or download new bitfile with -d."
	valuesflag=0
fi

# check if timeout is number and is positive or zero
if [[ $sim == "ML605" && (! $timeout =~ ^[\-0-9]+$ ||  $timeout -lt 0) ]]; then
	echo "Error: $timeout not valid for timeout. Specify zero or a positive integer with option -z."
	valuesflag=0
fi

checkUserValues

# make javatools for downloading elf files to fpga
if [[ $javatoolsflag == 1 ]]; then
	
	# check if Makefile exists
	if [[ ! -f $makefile ]]; then
		echo "Error: $makefile not found. Check if the specified patmos dir is correct."
		exit 1
	fi
		
	echo "Making javatools..."
	writeFile $log $logseparator
	useMakefile "javatools scripttools"	
	if [[ $? != 0 ]]; then		
		echo "Error: making javatools$checklog"
		toolsflag=0
	fi
	writeFile $log $logseparator
fi

# download specified bitfile
if [[ $bitflag == 1 ]]; then	
	echo "Downloading bitfile..."
	rm -rf $bitfile
	writeFile $log $logseparator
	wget $biturl -O $bitfile >> $log 2>&1
	if [[ $? != 0 ]]; then		
		echo "Error: downloading bitfile$checklog"
		toolsflag=0
	fi
	writeFile $log $logseparator
fi

if [[ $toolsflag == 0 ]]; then
	exit 1
fi

if [[ $xilinxfpgaflag == 1 || $sim == "ML605" ]]; then
	configureXilinxFPGA		
fi

cd $sourcedir
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