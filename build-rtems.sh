#!/bin/bash
#####################################################################################################################################
#
#script for building the patmos toolchain, and changing the patmos target from patmos-unknown-unknown-elf to patmos-unkwnown-rtems
#
#####################################################################################################################################

misc=misc
build=build-rtems
old_target=patmos-unknown-unknown-elf
new_target=patmos-unknown-rtems

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

git  clone  https://github.com/t-crest/patmos-misc.git  $misc
sed -e 's/ROOT_DIR=$(pwd)/ROOT_DIR='$(pwd | awk '{gsub("/", "\\/"); print}')'\/'$build'/g' $misc/build.cfg.dist > $misc/build.cfg
sed -i 's/TARGET=\"'$old_target'\"/TARGET=\"'$new_target'\"/g' $misc/build.cfg
sed -i 's/#GOLD_CXXFLAGS/GOLD_CXXFLAGS/g' $misc/build.cfg

sed -i 's/TARGET="'$old_target'"/TARGET="'$new_target'"/g' $misc/build.sh
sed -i 's/GOLD_ARGS=\"$GOLD_ARGS --target='$old_target'\"/GOLD_ARGS=\"$GOLD_ARGS --target='$new_target'\"/g' $misc/build.sh
sed -i 's/#GOLD_CXXFLAGS/GOLD_CXXFLAGS/g' $misc/build.sh

$misc/./build.sh
./target-change.sh $(pwd)/$build