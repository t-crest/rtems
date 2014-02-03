#!/bin/bash
#################################################################################
#
#script for changing the patmos target to patmos-unkwnown-rtems
#
#################################################################################


list=(addr2line\
	llc\
	llvm-readobj\
	ar\
	lli\
	llvm-rtdyld\
	arcmt-test\
	llvm-ar\
	llvm-size\
	bugpoint\
	llvm-as\
	llvm-as\
	llvm-stress\
	c-arcmt-test\
	llvm-bcanalyzer\
	llvm-tblgen\
	c++filt\
	llvm-config\
	macho-dump\
	c-index-test\
	llvm-cov\
	nm\
	clang\
	llvm-diff\
	not\
	clang++\
	llvm-dis\
	obj2yaml\
	clang-3.3\
	llvm-dwarfdump\
	objcopy\
	clang-check\
	llvm-extract\
	objdump\
	clang-tblgen\
	llvm-link\
	opt\
	count\
	llvm-lit\
	ranlib\
	diagtool\
	llvm-mc\
	readelf\
	elfedit\
	llvm-mcmarkup\
	size\
	FileCheck\
	llvm-nm\
	strings\
	FileUpdate\
	llvm-objdump\
	strip\
	ld\
	llvm-prof\
	yaml2obj\
	ld.gold\
	llvm-ranlib\
	yaml-bench)
	
pwd=$(pwd)

if [[ $1 == "" || $1 == "-help" || $1 == "--help" ]]; then
	echo "Usage : "$0" <ROOT DIR>"
elif [[ ! -d $1 ]]; then
	echo "Error: $1 not found..."
else
	echo 'Changing names of executables...'	
	cd $1	
	bindir=$(pwd)/local/bin
	cd $pwd
	
	for cmd in "${list[@]}"
	do
		ofile=$bindir/patmos-$cmd
		lfile=$bindir/patmos-unknown-rtems-$cmd
		if [[ -f $ofile ]]; then
			if [[ -f $lfile ]]; then
				echo 'Warning: link already exists...'
			else
				ln -s $ofile $lfile
			fi
		else
			echo 'Warning: '$ofile' not found...'
		fi 
	done
	echo '...finished'	
fi
