#!/bin/sh

#set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-build}
if [ -z "$DEBUG" ];then
    BUILD_TYPE=${BUILD_TYPE:-release}
else
    BUILD_TYPE=${BUILD_TYPE:-debug}
fi

if [ ! -z "$CLANG" ];then
    export CC=clang
    export CXX=clang++
fi

[ ! -z "$CXX" ] && echo "CXX is set as $CXX"

mkdir -p $BUILD_DIR/$BUILD_TYPE \
  && cd $BUILD_DIR/$BUILD_TYPE \
  && cmake \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           $SOURCE_DIR \
  && make $*

[ $? -eq 0 ] && cd $SOURCE_DIR && ./gen_readme.py


