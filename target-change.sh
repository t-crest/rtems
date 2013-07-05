#!/bin/bash
###############################################################################
#
#script for changing the patmos target to patmos-unkwnown-rtems
#
###############################################################################

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

# physical location of this script, and the config
self=$(abspath $0)
CFGFILE=$(dirname $self)/build.cfg

if [ -f $CFGFILE ]; then
  source $CFGFILE
fi

cd $ROOT_DIR/local/bin
echo 'Changing names of executables...'

ln -s patmos-addr2line patmos-unknown-rtems-addr2line
ln -s patmos-llc patmos-unknown-rtems-llc
ln -s patmos-llvm-readobj patmos-unknown-rtems-llvm-readobj
ln -s patmos-ar patmos-unknown-rtems-ar
ln -s patmos-lli patmos-unknown-rtems-lli
ln -s patmos-llvm-rtdyld patmos-unknown-rtems-llvm-rtdyld
ln -s patmos-arcmt-test patmos-unknown-rtems-arcmt-test
ln -s patmos-llvm-ar patmos-unknown-rtems-llvm-ar
ln -s patmos-llvm-size patmos-unknown-rtems-llvm-size
ln -s patmos-bugpoint patmos-unknown-rtems-bugpoint
ln -s patmos-llvm-as patmos-unknown-rtems-llvm-as
ln -s patmos-llvm-as patmos-unknown-rtems-as
ln -s patmos-llvm-stress patmos-unknown-rtems-llvm-stress
ln -s patmos-c-arcmt-test patmos-unknown-rtems-c-arcmt-test
ln -s patmos-llvm-bcanalyzer patmos-unknown-rtems-llvm-bcanalyzer
ln -s patmos-llvm-tblgen patmos-unknown-rtems-llvm-tblgen
ln -s patmos-c++filt patmos-unknown-rtems-c++filt
ln -s patmos-llvm-config patmos-unknown-rtems-llvm-config
ln -s patmos-macho-dump patmos-unknown-rtems-macho-dump
ln -s patmos-c-index-test patmos-unknown-rtems-c-index-test
ln -s patmos-llvm-cov patmos-unknown-rtems-llvm-cov
ln -s patmos-nm patmos-unknown-rtems-nm
ln -s patmos-clang patmos-unknown-rtems-clang
ln -s patmos-llvm-diff patmos-unknown-rtems-llvm-diff
ln -s patmos-not patmos-unknown-rtems-not
ln -s patmos-clang++ patmos-unknown-rtems-clang++
ln -s patmos-llvm-dis patmos-unknown-rtems-llvm-dis
ln -s patmos-obj2yaml patmos-unknown-rtems-obj2yaml
ln -s patmos-clang-3.2 patmos-unknown-rtems-clang-3.2
ln -s patmos-llvm-dwarfdump patmos-unknown-rtems-llvm-dwarfdump
ln -s patmos-objcopy patmos-unknown-rtems-objcopy
ln -s patmos-clang-check patmos-unknown-rtems-clang-check
ln -s patmos-llvm-extract patmos-unknown-rtems-llvm-extract
ln -s patmos-objdump patmos-unknown-rtems-objdump
ln -s patmos-clang-tblgen patmos-unknown-rtems-clang-tblgen
ln -s patmos-llvm-link patmos-unknown-rtems-llvm-link
ln -s patmos-opt patmos-unknown-rtems-opt
ln -s patmos-count patmos-unknown-rtems-count
ln -s patmos-llvm-lit patmos-unknown-rtems-llvm-lit
ln -s patmos-ranlib patmos-unknown-rtems-ranlib
ln -s patmos-diagtool patmos-unknown-rtems-diagtool
ln -s patmos-llvm-mc patmos-unknown-rtems-llvm-mc
ln -s patmos-readelf patmos-unknown-rtems-readelf
ln -s patmos-elfedit patmos-unknown-rtems-elfedit
ln -s patmos-llvm-mcmarkup patmos-unknown-rtems-llvm-mcmarkup
ln -s patmos-size patmos-unknown-rtems-size
ln -s patmos-FileCheck patmos-unknown-rtems-FileCheck
ln -s patmos-llvm-nm patmos-unknown-rtems-llvm-nm
ln -s patmos-strings patmos-unknown-rtems-strings
ln -s patmos-FileUpdate patmos-unknown-rtems-FileUpdate
ln -s patmos-llvm-objdump patmos-unknown-rtems-llvm-objdump
ln -s patmos-strip patmos-unknown-rtems-strip
ln -s patmos-ld patmos-unknown-rtems-ld
ln -s patmos-llvm-prof patmos-unknown-rtems-llvm-prof
ln -s patmos-yaml2obj patmos-unknown-rtems-yaml2obj
ln -s patmos-ld.gold patmos-unknown-rtems-ld.gold
ln -s patmos-llvm-ranlib patmos-unknown-rtems-llvm-ranlib
ln -s patmos-yaml-bench patmos-unknown-rtems-yaml-bench

echo '...finished'