#!/bin/bash

# Boost install

##############
# Config
##############

# Boost version
VERSION="1_65_1"

# In this folder there will be folders include, lib, src and boost archive
DESTINATION=ExternalLibs/boost_$VERSION

# Comment next line to install everything
LIBS="program_options"

##########################
# Implementation
##########################
DOTVERSION=$(echo $VERSION | sed -e 's/_/./g')
for LIB in $LIBS; do
    WITH="$WITH --with-$LIB"
done

set -x && \
cd .. && \
mkdir -p $DESTINATION && \
cd $DESTINATION && \
rm -rf * && \
wget https://downloads.sourceforge.net/project/boost/boost/$DOTVERSION/boost_$VERSION.tar.gz && \
tar -xvzf boost_$VERSION.tar.gz && \
mv boost_$VERSION src && \
cd src && \
mkdir build && \
./bootstrap.sh && \
./b2 --prefix=.. --build-dir=build --layout=tagged $WITH threading=multi install
