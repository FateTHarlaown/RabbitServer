#! /bin/bash
SCRIPT=$(readlink -f "$0")
ROOTDIR=$(dirname "$SCRIPT")
cd $ROOTDIR

if [ ! -d net ] || [ ! -d base ] || [ ! -d HTTP ] || [ ! -d test ]
then
	echo "$ROOTDIR is not a valid dir"
	echo "please run this script in regular position"
	exit -1
fi

rm -rf bin
mkdir bin
rm -rf build
mkdir build
cd build
cmake ../test
make


