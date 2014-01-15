#!/bin/bash
#####################################################################################################################################
#
#script for building the patmos toolchain, and changing the patmos target from patmos-unknown-unknown-elf to patmos-unkwnown-rtems
#
#####################################################################################################################################

misc_dir=misc
build_dir=build-rtems
rtems_dir=rtems/rtems-src
rtems__rel_dir=../rtems-src
rtems_build_dir=../rtems-build
rtems_install_dir=../rtems-install
old_target=patmos-unknown-unknown-elf
new_target=patmos-unknown-rtems
build_args=
build_rtems=false
build_emulator=false

function abspath() {
    local path=$1
    local pwd_restore="$(pwd)"

    # readlink -f does not work on OSX, so we do this manually
    cd $(dirname "$path")
    path=$(basename "$path")
    # follow chain of symlinks
    while [ -L "$path" ]; do
        path=$(readlink "$path")
        cd $(dirname "$path")
        path=$(basename "$path")
    done
    echo "$(pwd -P)/$path"
    cd "$pwd_restore"
}

function add2bashrc(){
	echo $1 >> ~/.bashrc
}

# physical location of this script, and the config
self=$(abspath $0)
CFGFILE=$(dirname $self)/build.cfg

if [ -f $CFGFILE ]; then
  source $CFGFILE
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
	*)
		build_args="$build_args $i"
	;;
	esac
done

sed -e 's/ROOT_DIR=$(pwd)/ROOT_DIR='$(pwd | awk '{gsub("/", "\\/"); print}')'\/'$build_dir'/g' $misc_dir/build.cfg.dist > $misc_dir/build.cfg
sed -i 's/TARGET=\"'$old_target'\"/TARGET=\"'$new_target'\"/g' $misc_dir/build.cfg
sed -i 's/BUILD_EMULATOR=true/BUILD_EMULATOR='$build_emulator'/g' $misc_dir/build.cfg
sed -i 's/#GOLD_CXXFLAGS/GOLD_CXXFLAGS/g' $misc_dir/build.cfg

$misc_dir/./build.sh $build_args
if [[ $new_target=="patmos-unknown-rtems" ]]; then
	./target-change.sh $(pwd)/$build
fi

grep -Fxq "export PATH=\${PATH}:"$(pwd)"/"$build_dir"/local/bin" ~/.bashrc
if [[ $? = 1 ]]; then
	add2bashrc "export PATH=\${PATH}:"$(pwd)"/"$build_dir"/local/bin"
	export PATH=${PATH}":"$(pwd)"/"$build_dir"/local/bin"
fi

grep -Fxq "export PATH=\${PATH}:"$(pwd)"/"$build_dir"/patmos/simulator/build/src" ~/.bashrc
if [[ $? = 1 ]]; then
	add2bashrc "export PATH=\${PATH}:"$(pwd)"/"$build_dir"/patmos/simulator/build/src"
	export PATH=${PATH}":"$(pwd)"/"$build_dir"/patmos/simulator/build/src"
fi	

if [[ $build_rtems == "true" ]]; then
	cd $rtems_dir
	./bootstrap -p
	./bootstrap
	mkdir $rtems_build_dir
	cd $rtems_build_dir
	$rtems__rel_dir/configure --target=patmos-unknown-rtems --enable-posix --disable-networking --disable-cxx --enable-tests --enable-rtemsbsp=pasim --prefix=$(pwd)"/"$rtems_dir"/"$rtems_install_dir
	make install
	grep -Fxq "export RTEMS_MAKEFILE_PATH="$(pwd)"/"$rtems_dir"/"$rtems_install_dir"/patmos-unknown-rtems/pasim" ~/.bashrc
	if [[ $? == 1  ]]; then
		add2bashrc "export RTEMS_MAKEFILE_PATH="$(pwd)"/"$rtems_dir"/"$rtems_install_dir"/patmos-unknown-rtems/pasim"
		export RTEMS_MAKEFILE_PATH"="$(pwd)"/"$rtems_dir"/"$rtems_install_dir"/patmos-unknown-rtems/pasim"
	fi
fi