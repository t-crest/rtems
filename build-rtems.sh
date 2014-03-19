#!/bin/bash
#####################################################################################################################################
#
#script for building the patmos toolchain, and changing the patmos target from patmos-unknown-unknown-elf to patmos-unkwnown-rtems
#
#####################################################################################################################################

pwd=$(pwd)
tcrest_dir=~/t-crest
misc_dir=$tcrest_dir/patmos-misc
build_dir=$tcrest_dir/build-rtems
rtems_dir=$tcrest_dir/rtems
rtems_src_dir=$rtems_dir/rtems-src
rtems_build_dir=$rtems_dir/rtems-build
rtems_install_dir=$rtems_dir/rtems-install
rtems_examples_dir=$rtems_dir/rtems-examples

old_target=patmos-unknown-unknown-elf
new_target=patmos-unknown-rtems
build_args=
build_rtems=false
build_toolchain=false
build_emulator=false
build_rtems_examples=false
build_llvm_release=true
get_rtems=true

function add2bashrc(){
	echo $1 >> ~/.bashrc
}

function usage(){
 cat <<EOT
 Usage: $0 [-m] [-t] [-e] [-r] [-h] [-rt]
 
 -h	Display help contents
 -m	Checkout patmos-misc repository
 -t	Build patmos toolchain except the emulator
 -e	Build patmos toolchain including the emulator
 -r	Build rtems for patmos
 -rt 	Download rtems examples
 -rs	Build rtems for patmos without cloning rtems-src
 
EOT
}

if [[ ! -d $tcrest_dir ]]; then
	mkdir $tcrest_dir
fi

for i in ${@:1}
  do
	case "$i" in
	"-m" | "-M")
		rm -rf $misc_dir
		git  clone  https://github.com/t-crest/patmos-misc.git  $misc_dir
	;;
	"-r" | "-R")
		build_rtems=true
	;;
	"-rs" | "-RS")
		build_rtems=true
		get_rtems=false		
	;;
	"-rt" | "-RT")
		build_rtems_examples=true
	;;
	"-t" | "-T")
		build_toolchain=true
	;;
	"-e" | "-E")
		build_emulator=true
		build_toolchain=true
	;;
	"-h" | "-H")
		usage
		exit 1
	;;
	*)
		build_args="$build_args $i"
	;;
	esac
done

if [[ $build_toolchain == "true" ]]; then

	if [[ ! -f $misc_dir/build.sh ]]; then
		echo "error: "$(basename $misc_dir)" not found"
		exit 1
	fi
		
	CFGFILE=$misc_dir/build.cfg			
	
	sed -e 's/ROOT_DIR=$(pwd)/ROOT_DIR='$(echo $build_dir | awk '{gsub("/", "\\/"); print}')'/g' $misc_dir/build.cfg.dist > $CFGFILE
	sed -i 's/TARGET=\"'$old_target'\"/TARGET=\"'$new_target'\"/g' $CFGFILE
	sed -i 's/BUILD_EMULATOR=true/BUILD_EMULATOR='$build_emulator'/g' $CFGFILE
	sed -i 's/#GOLD_CXXFLAGS/GOLD_CXXFLAGS/g' $CFGFILE

	if [[ $build_llvm_release == "true" ]]; then
		sed -i 's/LLVM_BUILD_TYPE=Debug/LLVM_BUILD_TYPE=Release/g' $CFGFILE		
	else
		sed -i 's/LLVM_BUILD_TYPE=Release/LLVM_BUILD_TYPE=Debug/g' $CFGFILE
	fi
	
	if [ -f $CFGFILE ]; then
	  source $CFGFILE
	fi

	$misc_dir/./build.sh $build_args
	if [[ $new_target == "patmos-unknown-rtems" ]]; then
		./target-change.sh $build_dir
	fi
	
	cd $build_dir	
	build_dir=$(pwd)
	cd $pwd
	
	grep -Fxq "export PATH=\${PATH}:"$build_dir"/local/bin" ~/.bashrc
	if [[ $? = 1 ]]; then
		add2bashrc "export PATH=\${PATH}:"$build_dir"/local/bin"
		export PATH=${PATH}":"$build_dir"/local/bin"
	fi

	grep -Fxq "export PATH=\${PATH}:"$build_dir"/patmos/simulator/build/src" ~/.bashrc
	if [[ $? = 1 ]]; then
		add2bashrc "export PATH=\${PATH}:"$build_dir"/patmos/simulator/build/src"
		export PATH=${PATH}":"$build_dir"/patmos/simulator/build/src"
	fi
fi

if [[ $build_rtems == "true" ]]; then

	if [[ $get_rtems == "true" ]]; then
		if [[ -d $rtems_src_dir ]]; then
			echo "error: "$(basename $rtems_src_dir)" not empty directory"
			exit 1
		fi
		git clone https://github.com/t-crest/rtems.git $rtems_src_dir
		cd $rtems_src_dir
		./bootstrap -p
		./bootstrap
	fi
	
	rm -rf $rtems_build_dir
	mkdir $rtems_build_dir
	
	rm -rf $rtems_install_dir
	mkdir $rtems_install_dir
	cd $rtems_install_dir
	rtems_install_dir=$(pwd)	
	
	cd $rtems_build_dir
	$rtems_src_dir/configure --target=patmos-unknown-rtems --enable-posix --disable-networking --disable-cxx --enable-tests --enable-rtemsbsp=pasim --prefix=$rtems_install_dir
	make install
	grep -Fxq "export RTEMS_MAKEFILE_PATH="$rtems_install_dir"/patmos-unknown-rtems/pasim" ~/.bashrc
	if [[ $? == 1  ]]; then
		add2bashrc "export RTEMS_MAKEFILE_PATH="$rtems_install_dir"/patmos-unknown-rtems/pasim"
		export RTEMS_MAKEFILE_PATH"="$rtems_install_dir"/patmos-unknown-rtems/pasim"
	fi
	#rm -rf $pwd
fi

if [[ $build_rtems_examples == "true" ]]; then
	git clone https://github.com/RTEMS/examples-v2 $rtems_examples_dir
fi